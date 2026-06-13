#include "PacketManager.h"
#include "SessionManager.h"
#include "RoomManager.h"
#include "TimerManager.h"
#include "MapManager.h"
#include "MoveObjManager.h"
#include "RayCasting.h"
#include "TimerManager.h"
#include <span>
#include <iostream>


void PacketManager::Init()
{
	MoveObjManager::GetInst().InitPlayer();
	MoveObjManager::GetInst().InitNPC();
	RoomManager::GetInst().InitRoom();

	TimerManager::GetInst();

	MapManager::GetInst().LoadMap();
	MapManager::GetInst().LoadSpawnArea();
	MapManager::GetInst().LoadEscapePoint();
	MapManager::GetInst().LoadItemSpawnPoint();
	MapManager::GetInst().LoadPlayerSpawnArea();
	MapManager::GetInst().LoadSpecialEscapePoint();
}

void PacketManager::ProcessPacket(int sessionId, std::span<char> packet)
{
	if (packet.size() < 2) return;

	unsigned char packetType = packet[1];

	if (handlers[packetType])
	{
		handlers[packetType](sessionId, packet);
	}
	else
	{
		//std::cout << "Unknown packet type: " << (int)packetType << "\n";
	}
}

void PacketManager::ProcessSignIn(int sessionId, cs_packet_sign_in* packet)
{
	sc_packet_sign_in_ok p = {
		.size = sizeof(p),
		.type = SC_PACKET_SIGN_IN_OK,
		.id = sessionId
	};

	SessionManager::GetInst().GetSession(sessionId)->SendPacket(SC_PACKET_SIGN_IN_OK, p);
}

void PacketManager::ProcessSignUp(int sessionId, cs_packet_sign_up* packet)
{
	// DB 연결이 없으므로, 일단 넘기기.
}

void PacketManager::ProcessAttack(int sessionId, cs_packet_attack* packet)
{
	auto player = MoveObjManager::GetInst().GetPlayer(sessionId);
	if (!player) return;

	Room* room = RoomManager::GetInst().GetRoom(player->GetRoomID());
	if (!room) return;

	std::vector<int> broadcast_targets;
	std::vector<int> hit_targets;

	// 🔒 [락 스코프 시작]
	{
		std::lock_guard<std::recursive_mutex> lock(room->m_state_lock);

		Vector3 AttackPos = player->GetPos();
		AttackPos.x += player->GetRotX() * 150.f;
		AttackPos.z += player->GetRotZ() * 150.f;

		for (int pl : room->GetObjList()) {
			if (pl == sessionId || !MoveObjManager::GetInst().IsPlayer(pl)) continue;

			auto target = MoveObjManager::GetInst().GetPlayer(pl);
			if (target && target->GetPos().Dist(AttackPos) < 50.f) {
				hit_targets.push_back(pl);
				Hit(sessionId, pl, room); // 이 안에서 안전하게 락 의존 로직 처리
			}
		}

		broadcast_targets.assign(room->GetObjList().begin(), room->GetObjList().end());
	}
	// 🔓 [락 스코프 종료]

	sc_packet_attack atkPkt{ SC_PACKET_ATTACK, sizeof(atkPkt) };
	for (int pl : broadcast_targets) {
		if (auto session = SessionManager::GetInst().GetSession(pl)) {
			session->SendPacket(SC_PACKET_ATTACK, atkPkt);
		}
	}
}

void PacketManager::ProcessMove(int sessionId, cs_packet_move* packet)
{
	auto player = MoveObjManager::GetInst().GetPlayer(sessionId);
	if (!player) return;

	Room* room = RoomManager::GetInst().GetRoom(player->GetRoomID());
	if (!room) return;

	sc_packet_move movePacket;
	movePacket.type = SC_PACKET_MOVE;
	movePacket.id = sessionId;

	std::vector<int> broadcast_targets;

	bool isItemAcquired = false;
	int acquiredItemId = -1;

	{
		std::lock_guard<std::recursive_mutex> lock(room->m_state_lock);

		if (room->IsGameEnd() || player->GetAttacked() || player->GetState() != P_STATE::ST_INGAME)
			return;

		Vector3 oldPos = player->GetPos();

		Vector3 look(packet->vecX, 0.0f, packet->vecZ);
		Vector3 up(0.0f, 1.0f, 0.0f);
		Vector3 right = look.Cross(up);

		Vector3 moveDir(0.0f, 0.0f, 0.0f);

		if (packet->direction & 8) moveDir += look;
		if (packet->direction & 2) moveDir -= look;
		if (packet->direction & 4) moveDir += right;
		if (packet->direction & 1) moveDir -= right;


		if (moveDir.VectorScale() > 0.001f) // divisionzero 에러 방지
		{
			moveDir = moveDir.Normalize();
		}

		Vector3 newPos = player->GetPos();
		newPos.x += moveDir.x * _speed * packet->deltaTime;
		newPos.z += moveDir.z * _speed * packet->deltaTime;
		player->SetPos(newPos);

		if ((packet->direction & 16) == 16 && !player->GetJump())
		{
			player->SetJump(true);
			player->SetUpVelocity(1250.0f);
		}

		player->SetRotX(packet->vecX);
		player->SetRotZ(packet->vecZ);
		player->SetAnimationNumber(packet->action_type);

		if (player->GetJump())
		{
			player->SetUpVelocity(player->GetUpVelocity() + (packet->deltaTime * -3000.0f));
			player->SetPosY(player->GetPosY() + (player->GetUpVelocity() * packet->deltaTime));

			if (player->GetPosY() < 0.f)
			{
				player->SetPosY(0.0f);
				player->SetJump(false);
			}
		}

		CBox PlayerBox;

		Vector3 PlayerRight = Vector3(player->GetRotX(), 0.f, player->GetRotZ()).Cross(Vector3(0.0f, 1.0f, 0.0f));

		// center
		PlayerBox.center[0] = player->GetPosX();
		PlayerBox.center[1] = player->GetPosY() + 75.f;
		PlayerBox.center[2] = player->GetPosZ();

		// extent
		PlayerBox.extent[0] = 25.f;
		PlayerBox.extent[1] = 75.f;
		PlayerBox.extent[2] = 25.f;

		// right axis
		PlayerBox.axis[0][0] = PlayerRight.x;
		PlayerBox.axis[0][1] = PlayerRight.y;
		PlayerBox.axis[0][2] = PlayerRight.z;

		// up axis
		PlayerBox.axis[1][0] = 0.0f;
		PlayerBox.axis[1][1] = 0.1f; 
		PlayerBox.axis[1][2] = 0.0f;

		// look axis
		PlayerBox.axis[2][0] = player->GetRotX();
		PlayerBox.axis[2][1] = 0.f;
		PlayerBox.axis[2][2] = player->GetRotZ();

		// translation
		PlayerBox.translation[0] = player->GetPosX() - oldPos.x;
		PlayerBox.translation[1] = player->GetPosY() - oldPos.y;
		PlayerBox.translation[2] = player->GetPosZ() - oldPos.z;

		movePacket.posX = player->GetPosX();
		movePacket.posY = player->GetPosY();
		movePacket.posZ = player->GetPosZ();
		movePacket.rotX = player->GetRotX();
		movePacket.rotZ = player->GetRotZ();
		movePacket.action_type = player->GetAnimationNumber();
		// 금고(SafeZone) 상호작용 검사
		bool isSafeZone = RayCasting::GetInst().CheckSafe(player->GetPos(), Vector3(player->GetRotX(), 0.f, player->GetRotZ()));

		// 아이템 충돌 및 덫(Trap) 판정
		for (int i = 0; i < MAX_ITEM; ++i)
		{
			Item* it = room->GetItem(i);
			if (!it) continue;

			if (it->GetState() == ITEM_STATE::ST_SPAWNED)
			{
				if (player->GetItem() == -1 && it->GetPos().Dist(player->GetPos()) < 30.f)
				{
					it->SetState(ITEM_STATE::ST_OCCUPIED);
					player->SetItem(i);
					isItemAcquired = true;
					acquiredItemId = i;
				}
			}
			else if (it->GetState() == ITEM_STATE::ST_SET)
			{
				// 설치된 덫에 걸린 경우
				if (it->GetPos().Dist(player->GetPos()) < 50.f)
				{
					player->SetAttacked(true);

					sc_packet_stun stunPkt{ sizeof(stunPkt), SC_PACKET_STUN, i, sessionId, true };

					for (auto other_pl : room->GetObjList()) {
						if (auto session = SessionManager::GetInst().GetSession(other_pl)) {
							session->SendPacket(SC_PACKET_STUN, stunPkt);
						}
					}

					// 다이아몬드 드롭 처리
					if (player->GetHasDiamond())
					{
						player->SetHasDiamond(false);
						sc_packet_diamond_owner_change diaPkt{ sizeof(diaPkt), SC_PACKET_DIAMOND_OWNER_CHANGE, -1 };
						for (auto other_pl : room->GetObjList()) {
							if (auto session = SessionManager::GetInst().GetSession(other_pl)) {
								session->SendPacket(SC_PACKET_DIAMOND_OWNER_CHANGE, diaPkt);
							}
						}
					}

					it->SetState(ITEM_STATE::ST_NOTUSED);

					TimerManager::GetInst().Push(3000, [player, sessionId]() {

						player->SetAttacked(false);
						sc_packet_stun_end stunEndPkt{
							.size = sizeof(sc_packet_stun_end),
							.type = SC_PACKET_STUN_END,
							.obj_id = sessionId,
						};
						
						SessionManager::GetInst().GetSession(sessionId)->SendPacket(SC_PACKET_STUN_END, stunEndPkt);

						});
				}
			}
		}

		// --- 6. 다이아몬드 탈출 판정 ---
		if (player->GetHasDiamond())
		{
			if (room->GetIsEscapeActive())
			{
				for (int i = 0; i < 3; ++i)
				{
					if (room->GetEscapePos(i).Dist(player->GetPos()) < 100.f)
					{
						TimerManager::GetInst().Push(5000, [room]() {

							if (!room->IsGameEnd()) return;

							for (auto obj_id : room->GetObjList()) {
								if (MoveObjManager::GetInst().IsPlayer(obj_id))
								{
									std::shared_ptr<Player> pl = MoveObjManager::GetInst().GetPlayer(obj_id);
									pl->SetAttacked(false);
									pl->SetHasDiamond(false);
									pl->SetItem(-1);
									pl->SetIsReady(false);

									sc_packet_game_end gameEndPkt{
										.size = sizeof(sc_packet_game_end),
										.type = SC_PACKET_GAME_END
									};

									SessionManager::GetInst().GetSession(pl->GetID())->SendPacket(SC_PACKET_GAME_END, gameEndPkt);
								}

								room->LeaveRoom(obj_id);
							}
						});

						room->SetGameEnd(true);
					}
				}
			}

			if (room->GetIsSpecialEscapeActive())
			{
				if (room->GetSpecialEscapePos().Dist(player->GetPos()) < 50.f)
				{
					TimerManager::GetInst().Push(5000, [room]() {

						if (!room->IsGameEnd()) return;

						for (auto obj_id : room->GetObjList()) {
							if (MoveObjManager::GetInst().IsPlayer(obj_id))
							{
								std::shared_ptr<Player> pl = MoveObjManager::GetInst().GetPlayer(obj_id);
								pl->SetAttacked(false);
								pl->SetHasDiamond(false);
								pl->SetItem(-1);
								pl->SetIsReady(false);

								sc_packet_game_end gameEndPkt{
									.size = sizeof(sc_packet_game_end),
									.type = SC_PACKET_GAME_END
								};

								SessionManager::GetInst().GetSession(pl->GetID())->SendPacket(SC_PACKET_GAME_END, gameEndPkt);
							}

							room->LeaveRoom(obj_id);
						}
					});

					room->SetGameEnd(true);
				}
			}
		}

		// 락 풀리기 전에 방 안의 플레이어 목록 복사해오기.
		// 방 인원이 8명이라 복사 비용은 거의 0임.
		broadcast_targets.assign(room->GetObjList().begin(), room->GetObjList().end());
	}

	for (auto other_id : broadcast_targets)
	{
		if (other_id == sessionId) continue;
		if (auto session = SessionManager::GetInst().GetSession(other_id))
		{
			session->SendPacket(SC_PACKET_MOVE, movePacket);
		}
	}

	if (isItemAcquired)
	{
		sc_packet_get_item getPkt;
		getPkt.type = SC_PACKET_GET_ITEM;
		getPkt.obj_id = acquiredItemId;
		getPkt.player = sessionId;

		for (auto pl : broadcast_targets) {
			if (auto session = SessionManager::GetInst().GetSession(pl)) {
				session->SendPacket(SC_PACKET_GET_ITEM, getPkt);
			}
		}
	}
}

void PacketManager::Hit(int attackerId, int victimId, Room* room)
{
	// ※ 이미 호출자가 room->m_state_lock을 잡고 있는 상태예요!
	auto victim = MoveObjManager::GetInst().GetPlayer(victimId);
	if (!victim) return;

	victim->SetAttacked(true);

	// 스턴 처리 등 내부 로직 (전송은 밖으로 빼는 것이 좋지만, 
	// Hit가 너무 복잡해질 경우 Room 락을 잠시 풀어주는 기법을 쓸 수도 있어요...)
	// 다이아몬드 스틸 로직
	if (victim->GetHasDiamond()) {
		auto attacker = MoveObjManager::GetInst().GetPlayer(attackerId);
		if (attacker) {
			victim->SetHasDiamond(false);
			attacker->SetHasDiamond(true);
			// ... 타이머 이벤트 추가 등 ...
		}
	}
}

void PacketManager::ProcessGameStart(int sessionId, cs_packet_game_start* packet)
{

}

void PacketManager::ProcessGetItem(int sessionId, cs_packet_get_item* packet)
{

}

void PacketManager::ProcessUseItem(int sessionId, cs_packet_use_item* packet)
{

}

void PacketManager::ProcessLoadProgressing(int sessionId, cs_packet_load_progressing* packet)
{

}

void PacketManager::ProcessLoadEnd(int sessionId, cs_packet_load_end* packet)
{}

void PacketManager::ProcessEnterRoom(int sessionId, cs_packet_enter_room * packet)
{}

void PacketManager::ProcessLeaveRoom(int sessionId, cs_packet_leave_room * packet)
{}

void PacketManager::ProcessPlayerReady(int sessionId, cs_packet_player_ready * packet)
{}

void PacketManager::ProcessPlayerCancleReady(int sessionId, cs_packet_player_cancle_ready * packet)
{}

void PacketManager::ProcessLogOut(int sessionId, cs_packet_player_log_out * packet)
{}

void PacketManager::ProcessRoomsDataInLobby(int sessionId, cs_packet_request_rooms_data_for_lobby * packet)
{
	
	for (auto room : RoomManager::GetInst().GetRooms())
	{
		if (room->GetState() == ROOM_STATE::RT_FREE) continue;

		sc_packet_rooms_data_for_lobby dPacket{
			.size = sizeof(sc_packet_rooms_data_for_lobby),
			.type = SC_PACKET_ROOMS_DATA_FOR_LOBBY,
			.roomID = room->GetRoomID(),
			.playerNum = static_cast<char>(room->GetNumberOfPlayer()),
			.isInGame = room->GetState() == ROOM_STATE::RT_INGAME ? true : false
		};

		SessionManager::GetInst().GetSession(sessionId)->SendPacket(SC_PACKET_ROOMS_DATA_FOR_LOBBY, dPacket);
	}

	sc_packet_rooms_data_for_lobby_end endPacket{
		.size = sizeof(sc_packet_rooms_data_for_lobby_end),
		.type = SC_PACKET_ROOMS_DATA_FOR_LOBBY_END
	};

	SessionManager::GetInst().GetSession(sessionId)->SendPacket(SC_PACKET_ROOMS_DATA_FOR_LOBBY_END, endPacket);
}

void PacketManager::ProcessRoomsDataInRoom(int sessionId, cs_packet_request_rooms_data_for_room * packet)
{}

void PacketManager::ProcessDamageCheat(int sessionId)
{}

void PacketManager::ProcessBullet(int sessionId, cs_packet_bullet * packet)
{}
