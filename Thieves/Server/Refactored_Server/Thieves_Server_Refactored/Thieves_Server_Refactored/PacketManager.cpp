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

#define SendToSession(S_ID, PCK_TYP, PCK) SessionManager::GetInst().GetSession(S_ID)->SendPacket(PCK_TYP, PCK);

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

	SendToSession(sessionId, SC_PACKET_SIGN_IN_OK, p);
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
				Hit(sessionId, pl, room);
			}
		}

		broadcast_targets.assign(room->GetObjList().begin(), room->GetObjList().end());
	}

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

		player->SetPos(MapManager::GetInst().checkCollision(PlayerBox, oldPos));

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
						
						SendToSession(sessionId, SC_PACKET_STUN_END, stunEndPkt);

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

									SendToSession(pl->GetID(), SC_PACKET_GAME_END, gameEndPkt);
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

								SendToSession(pl->GetID(), SC_PACKET_GAME_END, gameEndPkt);
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
	// ※ 이미 호출자가 room->m_state_lock을 잡고 있는 상태
	auto victim = MoveObjManager::GetInst().GetPlayer(victimId);
	if (!victim) return;

	victim->SetAttacked(true);

	// 스턴 처리 등 내부 로직 
	// 다이아몬드 스틸 로직
	if (victim->GetHasDiamond()) {
		auto attacker = MoveObjManager::GetInst().GetPlayer(attackerId);
		if (attacker) {
			victim->SetHasDiamond(false);
			attacker->SetHasDiamond(true);
		}
	}
}

void PacketManager::ProcessGameStart(int sessionId, cs_packet_game_start* packet)
{
	DEBUG_LOG("[" << sessionId << "]" << "Send Game Start.");

	auto player = MoveObjManager::GetInst().GetPlayer(sessionId);

	if (player->GetRoomID() == -1) return;
	Room* room = RoomManager::GetInst().GetRoom(player->GetRoomID());
	room->SetGameStart();

	for (auto pl : room->GetObjList())
	{
		if (auto session = SessionManager::GetInst().GetSession(pl)) {
			auto cpl = MoveObjManager::GetInst().GetPlayer(pl);
			cpl->state_lock.lock();
			cpl->SetState(P_STATE::ST_INGAME);
			cpl->SetIsActive(true);
			cpl->state_lock.unlock();
		}
	}

	StartGame(room->GetRoomID());

}

void PacketManager::StartGame(int room_id)
{
	Room* room = RoomManager::GetInst().GetRoom(room_id);
	
	Vector3 NPC_init_pos{ 0.0f, 300.0f, 0.0f };

	std::random_device rd;
	std::default_random_engine dre(rd());
	auto playerSpawnPos = MapManager::GetInst().GetPlayerSpawnPos();
	std::shuffle(playerSpawnPos.begin(), playerSpawnPos.end(), dre);

	auto itemPos = MapManager::GetInst().GetItemPos();
	std::shuffle(itemPos.begin(), itemPos.end(), dre);

	auto escapePos = MapManager::GetInst().GetEscapePos();
	std::shuffle(escapePos.begin(), escapePos.end(), dre);

	auto specialEscapePos = MapManager::GetInst().GetSpecialEscapePos();
	std::shuffle(specialEscapePos.begin(), specialEscapePos.end(), dre);

	auto npcPos = MapManager::GetInst().GetNPCSpawnPos();
	std::shuffle(npcPos.begin(), npcPos.end(), dre);

	// NPC 루프 생략
	/*
	for (int i = NPC_ID_START; i < NPC_ID_START + 8; ++i)
	{
		auto e = MoveObjManager::GetInst().GetEnemy(i);

		if (false == e->)
		{
			e->in_use = true;
			e->SetRoomID(room_id);
			obj_list.push_back(e->GetID());
		}

		room->EnterEnemyRoom(i);
	}
	*/
	int i = 0;
	for (auto c_id : room->GetObjList())
	{
		if (auto session = SessionManager::GetInst().GetSession(c_id))
		{
			MoveObjManager::GetInst().GetPlayer(c_id)->SetPos(MapManager::GetInst().GetPlayerSpawnPos().at(i));
			i++;
		}
	}

	for (int i = 0; i < MAX_ITEM; ++i) {
		if (room->GetItem(i) == nullptr) continue;

		Item* it = room->GetItem(i);

		if (i > MAX_ITEM / 2)
		{
			it->SetItemCode(ITEM_NUM_TRAP);
		}
		else
		{
			it->SetItemCode(ITEM_NUM_GUN);
		}

		it->SetState(ITEM_STATE::ST_SPAWNED);
		it->SetPos(MapManager::GetInst().GetItemPos().at(i));

	}

	// 탈출 위치

	for (int i = 0; i < 3; ++i)
	{
		room->SetEscapePos(i, MapManager::GetInst().GetEscapePos().at(i));
	}

	// 특별 탈출 위치

	room->SetSpecialEscapePos(MapManager::GetInst().GetSpecialEscapePos().at(0));

	sc_packet_game_start gsPck{
		.size = sizeof(sc_packet_game_start),
		.type = SC_PACKET_GAME_START
	};

	for (auto c_id : room->GetObjList())
	{
		if (auto session = SessionManager::GetInst().GetSession(c_id))
		{
			session->SendPacket(SC_PACKET_GAME_START, gsPck);
		}
		
	}

	for (auto c_id : room->GetObjList())
	{
		if (auto session = SessionManager::GetInst().GetSession(c_id))
		{
			SendObjInfo(c_id, c_id);	// 자기자신
			for (auto other_id : room->GetObjList())
			{
				//if (false == MoveObjManager::GetInst()->IsPlayer(other_id))
				//	continue;
				if (c_id == other_id) continue;
				SendObjInfo(c_id, other_id);
			}
		}
	}

	for (auto c_id : room->GetObjList())
	{
		if (auto session = SessionManager::GetInst().GetSession(c_id))
		{
			for (int i = 0; i < MAX_ITEM; ++i)
			{
				sc_packet_item_info ipck{
					.size = sizeof(sc_packet_item_info),
					.type = SC_PACKET_ITEM_INFO,
					.id = room->GetItem(i)->GetID(),
					.x = room->GetItem(i)->GetPosX(),
					.y = room->GetItem(i)->GetPosY(),
					.z = room->GetItem(i)->GetPosZ(),
					.item_type = static_cast<char>(room->GetItem(i)->GetItemCode())
				};

				SendToSession(c_id, SC_PACKET_ITEM_INFO, ipck);
			}
		}
	}

	for (auto c_id : room->GetObjList())
	{
		if (auto session = SessionManager::GetInst().GetSession(c_id))
		{
			sc_packet_obj_info_end endPck{
				.size = sizeof(sc_packet_obj_info_end),
				.type = SC_PACKET_OBJ_INFO_END
			};

			SendToSession(c_id, SC_PACKET_OBJ_INFO_END, endPck);
		}
	}
}

void PacketManager::SendObjInfo(int c_id, int obj_id)
{
	auto obj = MoveObjManager::GetInst().GetMoveObj(obj_id);

	sc_packet_obj_info pck{
		.size = sizeof(sc_packet_obj_info),
		.type = SC_PACKET_OBJ_INFO,
		.id = obj_id,
		.x = obj->GetPosX(),
		.y = obj->GetPosY(),
		.z = obj->GetPosZ(),
		.start = true,
		.object_type = static_cast<char>(obj->GetType()),
	};

	obj->SetPosZ(pck.z);

	DEBUG_LOG("[" << c_id << "] Send Obj Info : " << pck.x << ", " << pck.y << ", " << pck.z);

	SendToSession(c_id, SC_PACKET_OBJ_INFO, pck);
}



void PacketManager::ProcessGetItem(int sessionId, cs_packet_get_item* packet)
{

}

void PacketManager::ProcessUseItem(int sessionId, cs_packet_use_item* packet)
{

}

void PacketManager::ProcessLoadProgressing(int sessionId, cs_packet_load_progressing* packet)
{
	
	auto player = MoveObjManager::GetInst().GetPlayer(sessionId);

	if (player->GetRoomID() == -1) return;
	Room* room = RoomManager::GetInst().GetRoom(player->GetRoomID());

	player->state_lock.lock();
	player->SetLoadProgressed(packet->progressed);
	player->state_lock.unlock();

	for (auto pl : room->GetObjList())
	{
		if (auto session = SessionManager::GetInst().GetSession(pl))
		{
			sc_packet_load_progress_percent pck{
				.size = sizeof(sc_packet_load_progress_percent),
				.type = SC_PACKET_LOAD_PROGRESS_PERCENT,
				.id = sessionId,
				.percent = packet->progressed
			};

			session->SendPacket(SC_PACKET_LOAD_PROGRESS_PERCENT, pck);
		}
	}
}

void PacketManager::ProcessLoadEnd(int sessionId, cs_packet_load_end* packet)
{

	auto player = MoveObjManager::GetInst().GetPlayer(sessionId);

	if (player->GetRoomID() == -1) return;
	Room* room = RoomManager::GetInst().GetRoom(player->GetRoomID());

	if (player->GetState() == P_STATE::ST_INGAME)
	{

		player->state_lock.lock();
		player->SetLoadProgressed(100);
		player->state_lock.unlock();

		for (auto pl : room->GetObjList())
		{
			if (pl == sessionId) continue;
			if (auto session = SessionManager::GetInst().GetSession(pl))
			{
				sc_packet_load_end loadEndPck{
					.size = sizeof(sc_packet_load_end),
					.type = SC_PACKET_LOAD_END,
					.id = sessionId
				};

				session->SendPacket(SC_PACKET_LOAD_END, loadEndPck);
			}
		}

		// 모두 끝났는지 체크해보자

		for (auto pl : room->GetObjList())
		{
			if (auto session = SessionManager::GetInst().GetSession(pl))
			{

				MoveObjManager::GetInst().GetPlayer(pl)->state_lock.lock();
				if (MoveObjManager::GetInst().GetPlayer(pl)->GetLoadProgressed() != 100) {
					MoveObjManager::GetInst().GetPlayer(pl)->state_lock.unlock();
					return;
				}
				MoveObjManager::GetInst().GetPlayer(pl)->state_lock.unlock();
			}

		}

	}

	for (auto pl : room->GetObjList())
	{
		if (auto session = SessionManager::GetInst().GetSession(pl))
		{
			sc_packet_all_player_load_end allEndPck{
				.size = sizeof(sc_packet_all_player_load_end),
				.type = SC_PACKET_ALL_PLAYER_LOAD_END
			};

			session->SendPacket(SC_PACKET_ALL_PLAYER_LOAD_END, allEndPck);
		}
	}

	room->SetRoundStartTime();

	for (auto pl : room->GetObjList())
	{
		if (auto session = SessionManager::GetInst().GetSession(pl))
		{
			sc_packet_timer_start tsPck{
				.size = sizeof(sc_packet_timer_start),
				.type = SC_PACKET_TIMER_START,
				.start_time = room->GetRoundStartTime()
			};

			session->SendPacket(SC_PACKET_TIMER_START, tsPck);
		}
	}

	// 타이머 관련 나중에 하기.
	/*
	m_Timer->AddTimer(room->GetRoomID(), room->GetRoundStartTime() + 5s, EVENT_TYPE::EV_TIMER_START); // objID를 룸 넘버로 사용하면 될 것으로 보임.
	m_Timer->AddTimer(room->GetRoomID(), room->GetRoundStartTime() + 65s, EVENT_TYPE::EV_OPEN_SAFE);
	m_Timer->AddTimer(room->GetRoomID(), room->GetRoundStartTime() + 125s, EVENT_TYPE::EV_OPEN_ESCAPE_AREA);
	m_Timer->AddTimer(room->GetRoomID(), room->GetRoundStartTime() + 185s, EVENT_TYPE::EV_OPEN_SPECIAL_ESCAPE_AREA);
	*/

	TimerManager::GetInst().Push(5000, [room]() {

		for (int pl : room->GetObjList())
		{
			if (auto session = SessionManager::GetInst().GetSession(pl))
			{
				sc_packet_game_timer_start gtsPck{
					.size = sizeof(sc_packet_game_timer_start),
					.type = SC_PACKET_GAME_TIMER_START
				};

				session->SendPacket(SC_PACKET_GAME_TIMER_START, gtsPck);
			}
		}
		});

	TimerManager::GetInst().Push(65000, [room]() {

		for (int pl : room->GetObjList())
		{
			if (auto session = SessionManager::GetInst().GetSession(pl))
			{
				sc_packet_open_safe osPck{
					.size = sizeof(sc_packet_open_safe),
					.type = SC_PACKET_OPEN_SAFE
				};

				session->SendPacket(SC_PACKET_OPEN_SAFE, osPck);
			}
		}
		});

	TimerManager::GetInst().Push(125000, [room]() {

		room->SetIsEscapeActive(true);

		for (int pl : room->GetObjList())
		{
			if (auto session = SessionManager::GetInst().GetSession(pl))
			{
				sc_packet_active_escape aePck{
					.size = sizeof(sc_packet_active_escape),
					.type = SC_PACKET_ACTIVE_ESCAPE
				};

				session->SendPacket(SC_PACKET_ACTIVE_ESCAPE, aePck);
			}
		}
		});

	TimerManager::GetInst().Push(185000, [room]() {

		room->SetIsSpecialEscapeActive(true);
		for (int pl : room->GetObjList())
		{
			if (auto session = SessionManager::GetInst().GetSession(pl))
			{
				sc_packet_active_special_escape asePck{
					.size = sizeof(sc_packet_active_special_escape),
					.type = SC_PACKET_ACTIVE_SPECIAL_ESCAPE
				};

				session->SendPacket(SC_PACKET_ACTIVE_SPECIAL_ESCAPE, asePck);
			}
		}
		});
}

void PacketManager::ProcessEnterRoom(int sessionId, cs_packet_enter_room * packet)
{

	auto player = MoveObjManager::GetInst().GetPlayer(sessionId);

	Room* room;

	if (packet->room_id == -1)
	{
		auto newRoomNum = RoomManager::GetInst().GetEmptyRoom();
		if (newRoomNum.has_value())
		{
			room = RoomManager::GetInst().GetRoom(newRoomNum.value());
		}
		else
		{
			return;
		}
	}
	else
	{
		room = RoomManager::GetInst().GetRoom(packet->room_id);

		if (room->GetState() == ROOM_STATE::RT_FREE) {

			sc_packet_error pk{
				.size = sizeof(sc_packet_error),
				.type = SC_PACKET_ERROR,
				.err_type = ERROR_ROOM_NOT_EXIST,
				.err_val = -1
			};

			SendToSession(sessionId, SC_PACKET_ERROR, pk);
		}
		else if (room->GetState() == ROOM_STATE::RT_INGAME) {

			sc_packet_error pk{
				.size = sizeof(sc_packet_error),
				.type = SC_PACKET_ERROR,
				.err_type = ERROR_GAME_IN_PROGRESS,
				.err_val = -1
			};

			SendToSession(sessionId, SC_PACKET_ERROR, pk);
		}
		else if (room->GetNumberOfPlayer() >= USER_NUM)	{

			sc_packet_error pk{
				.size = sizeof(sc_packet_error),
				.type = SC_PACKET_ERROR,
				.err_type = ERROR_ROOM_IS_FULL,
				.err_val = -1
			};

			SendToSession(sessionId, SC_PACKET_ERROR, pk);
		}
	}

	player->state_lock.lock();
	player->SetRoomID(room->GetRoomID());
	player->state_lock.unlock();

	room->m_state_lock.lock();
	room->EnterRoom(sessionId);
	room->m_state_lock.unlock();

	sc_packet_enter_room_ok enterRoomOk{
		.size = sizeof(sc_packet_enter_room_ok),
		.type = SC_PACKET_ENTER_ROOM_OK,
		.room_id = room->GetRoomID()
	};

	SendToSession(sessionId, SC_PACKET_ENTER_ROOM_OK, enterRoomOk);

	std::vector<int> broadcast_targets;

	broadcast_targets.assign(room->GetObjList().begin(), room->GetObjList().end());

	sc_packet_enter_room enterRoom{
		.size = sizeof(sc_packet_enter_room),
		.type = SC_PACKET_ENTER_ROOM,
		.id = sessionId,
		.userName = "test"
	};

	for (auto pl : broadcast_targets) {
		if (auto session = SessionManager::GetInst().GetSession(pl)) {
			session->SendPacket(SC_PACKET_ENTER_ROOM, enterRoom);
		}
	}

}

void PacketManager::ProcessLeaveRoom(int sessionId, cs_packet_leave_room * packet)
{
	auto player = MoveObjManager::GetInst().GetPlayer(sessionId);

	if (player->GetRoomID() == -1) return;

	Room* room = RoomManager::GetInst().GetRoom(player->GetRoomID());

	if (!room->GetObjList().contains(sessionId)) return;

	player->state_lock.lock();
	player->SetRoomID(-1);
	player->SetState(P_STATE::ST_LOGIN);
	player->state_lock.unlock();

	room->LeaveRoom(sessionId);

	sc_packet_leave_room leaveRoomPck{
		.size = sizeof(sc_packet_leave_room),
		.type = SC_PACKET_LEAVE_ROOM,
		.id = sessionId,
		.master_id = room->GetRoomMasterId()
	};

	if (room->GetState() != ROOM_STATE::RT_FREE)
	{
		for (auto pl : room->GetObjList())
		{
			if (auto session = SessionManager::GetInst().GetSession(pl)) {
				session->SendPacket(SC_PACKET_LEAVE_ROOM, leaveRoomPck);
			}
		}
	}
}

void PacketManager::ProcessPlayerReady(int sessionId, cs_packet_player_ready * packet)
{
	auto player = MoveObjManager::GetInst().GetPlayer(sessionId);

	Room* room = RoomManager::GetInst().GetRoom(player->GetRoomID());

	DEBUG_LOG("[" << sessionId << "]" << "Player Ready is " << room->IsPlayerReady(sessionId) << ". This Process is PlayerReady.");

	if (false == room->IsPlayerReady(sessionId)) {
		
		room->m_state_lock.lock();
		room->PlayerReady(sessionId);
		room->m_state_lock.unlock();

		player->state_lock.lock();
		player->SetState(P_STATE::ST_INROOMREDDY);
		player->state_lock.unlock();
	}

	sc_packet_player_ready readyPck{
		.size = sizeof(sc_packet_player_ready),
		.type = SC_PACKET_PLAYER_READY,
		.id = sessionId
	};


	for (auto pl : room->GetObjList())
	{
		if (auto session = SessionManager::GetInst().GetSession(pl)) {
			session->SendPacket(SC_PACKET_PLAYER_READY, readyPck);
		}
	}
	
}

void PacketManager::ProcessPlayerCancleReady(int sessionId, cs_packet_player_cancle_ready * packet)
{

	auto player = MoveObjManager::GetInst().GetPlayer(sessionId);

	Room* room = RoomManager::GetInst().GetRoom(player->GetRoomID());

	DEBUG_LOG("[" << sessionId << "]" << "Player Ready is " << room->IsPlayerReady(sessionId) << ". This Process is PlayerCancleReady.");

	if (room->IsPlayerReady(sessionId)) {
		room->m_state_lock.lock();
		room->PlayerCancleReady(sessionId);
		room->m_state_lock.unlock();

		player->state_lock.lock();
		player->SetState(P_STATE::ST_INROOM);
		player->state_lock.unlock();
	}

	sc_packet_player_cancle_ready readyPck{
		.size = sizeof(sc_packet_player_cancle_ready),
		.type = SC_PACKET_PLAYER_CANCLE_READY,
		.id = sessionId,
	};


	for (auto pl : room->GetObjList())
	{
		if (auto session = SessionManager::GetInst().GetSession(pl)) {
			session->SendPacket(SC_PACKET_PLAYER_CANCLE_READY, readyPck);
		}
	}

}

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

		SendToSession(sessionId, SC_PACKET_ROOMS_DATA_FOR_LOBBY, dPacket);
	}

	sc_packet_rooms_data_for_lobby_end endPacket{
		.size = sizeof(sc_packet_rooms_data_for_lobby_end),
		.type = SC_PACKET_ROOMS_DATA_FOR_LOBBY_END
	};

	SendToSession(sessionId, SC_PACKET_ROOMS_DATA_FOR_LOBBY_END, endPacket);
}

void PacketManager::ProcessRoomsDataInRoom(int sessionId, cs_packet_request_rooms_data_for_room * packet)
{

	auto player = MoveObjManager::GetInst().GetPlayer(sessionId);

	Room* room = RoomManager::GetInst().GetRoom(player->GetRoomID());

	std::vector<int> broadcast_targets;
	broadcast_targets.assign(room->GetObjList().begin(), room->GetObjList().end());

	for (auto pl : broadcast_targets) {

		if (auto session = SessionManager::GetInst().GetSession(pl)) {
			sc_packet_rooms_data_for_room data{
				.size = sizeof(sc_packet_rooms_data_for_room),
				.type = SC_PACKET_ROOMS_DATA_FOR_ROOM,
				.userId = MoveObjManager::GetInst().GetPlayer(session->GetId())->GetID(),
				.isReady = MoveObjManager::GetInst().GetPlayer(session->GetId())->GetIsReady(),
			};

			char* name = MoveObjManager::GetInst().GetPlayer(session->GetId())->GetName();

			for (int i = 0; i < MAX_NAME_SIZE; ++i)
			{
				data.userName[i] = name[i];
				if (name[i] == '\0') break; 
			}

			session->SendPacket(SC_PACKET_ROOMS_DATA_FOR_ROOM, data);
		}
	}

	sc_packet_rooms_data_for_room_end endData{
		.size = sizeof(sc_packet_rooms_data_for_room_end),
		.type = SC_PACKET_ROOMS_DATA_FOR_ROOM_END,
		.master_id = room->GetRoomMasterId()
	};

	SendToSession(sessionId, SC_PACKET_ROOMS_DATA_FOR_ROOM_END, endData);

}

void PacketManager::ProcessDamageCheat(int sessionId)
{}

void PacketManager::ProcessBullet(int sessionId, cs_packet_bullet * packet)
{}
