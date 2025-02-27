#include "pch.h"

#include "server/main/network.h"
#include "server/main/network_obj_manager.h"
#include "server/main/network_move_object.h"

#include "Engine/server/thieves_server/thieves_packet_manager.h"
#include "server/packet/packet_helper.h"
#include "server/thieves_server/thieves_message_event_info.h"
#include "server/object/player.h"

#include "PlayerInput.h"
#include "Transform.h"
#include "Engine.h"
#include "Input.h"
#include "SceneManager.h"
#include "Scene.h"
#include "RoomScene.h"
#include "LobbyScene.h"
#include "InGameScene.h"
#include "GameObject.h"
#include "NetworkSystem.h"
#include "MapItem.h"
#include "ParticleSystem.h"
#include "Timer.h"

using namespace std;
using namespace client_fw;

void ThievesPacketManager::Init()
{
	m_obj_map = unordered_map<int, shared_ptr<NetworkMoveObj>>();
	
	RegisterRecvFunction(SC_PACKET_MOVE, [this](int c_id, unsigned char* p) {ProcessMove(c_id, p); });
	RegisterRecvFunction(SC_PACKET_OBJ_INFO, [this](int c_id, unsigned char* p) {ProcessObjInfo(c_id, p); });
	RegisterRecvFunction(SC_PACKET_OBJ_INFO_END, [this](int c_id, unsigned char* p) {ProcessObjInfoEnd(c_id, p); });
	RegisterRecvFunction(SC_PACKET_SIGN_IN_OK, [this](int c_id, unsigned char* p) {ProcessSignin(c_id, p); });
	RegisterRecvFunction(SC_PACKET_REMOVE_OBJECT, [this](int c_id, unsigned char* p) {ProcessRemoveObj(c_id, p); });
	RegisterRecvFunction(SC_PACKET_ROOMS_DATA_FOR_LOBBY, [this](int c_id, unsigned char* p) {ProcessRoomsData(c_id, p); });
	RegisterRecvFunction(SC_PACKET_ROOMS_DATA_FOR_LOBBY_END, [this](int c_id, unsigned char* p) {ProcessRoomsDataEnd(c_id, p); });
	RegisterRecvFunction(SC_PACKET_ROOMS_DATA_FOR_ROOM, [this](int c_id, unsigned char* p) {ProcessInRoomData(c_id, p); });
	RegisterRecvFunction(SC_PACKET_ROOMS_DATA_FOR_ROOM_END, [this](int c_id, unsigned char* p) {ProcessInRoomDataEnd(c_id, p); });
	RegisterRecvFunction(SC_PACKET_LEAVE_ROOM, [this](int c_id, unsigned char* p) {ProcessPlayerLeft(c_id, p); });
	RegisterRecvFunction(SC_PACKET_ENTER_ROOM, [this](int c_id, unsigned char* p) {ProcessPlayerJoin(c_id, p); });
	RegisterRecvFunction(SC_PACKET_PLAYER_READY, [this](int c_id, unsigned char* p) {ProcessPlayerReady(c_id, p); });
	RegisterRecvFunction(SC_PACKET_PLAYER_CANCLE_READY, [this](int c_id, unsigned char* p) {ProcessPlayerCancleReady(c_id, p); });
	RegisterRecvFunction(SC_PACKET_ENTER_ROOM_OK, [this](int c_id, unsigned char* p) {ProcessEnterRoomOk(c_id, p); });
	RegisterRecvFunction(SC_PACKET_ERROR, [this](int c_id, unsigned char* p) {ProcessError(c_id, p); });
	RegisterRecvFunction(SC_PACKET_GAME_START, [this](int c_id, unsigned char* p) {ProcessGameStart(c_id, p); });
	RegisterRecvFunction(SC_PACKET_ALL_PLAYER_LOAD_END, [this](int c_id, unsigned char* p) {ProcessAllPlayerLoadend(c_id, p); });
	RegisterRecvFunction(SC_PACKET_BULLET, [this](int c_id, unsigned char* p) {ProcessBullet(c_id, p); });
	RegisterRecvFunction(SC_PACKET_PHASE, [this](int c_id, unsigned char* p) {ProcessPhaseChange(c_id, p); });
	RegisterRecvFunction(SC_PACKET_ATTACK, [this](int c_id, unsigned char* p) {ProcessAttack(c_id, p); });
	RegisterRecvFunction(SC_PACKET_STUN, [this](int c_id, unsigned char* p) {ProcessHit(c_id, p); });
	RegisterRecvFunction(SC_PACKET_GET_ITEM, [this](int c_id, unsigned char* p) {ProcessGetItem(c_id, p); });
	RegisterRecvFunction(SC_PACKET_STUN_END, [this](int c_id, unsigned char* p) {ProcessStunEnd(c_id, p); });
	RegisterRecvFunction(SC_PACKET_ITEM_INFO, [this](int c_id, unsigned char* p) {ProcessItemInfo(c_id, p); });
	RegisterRecvFunction(SC_PACKET_TIMER_START, [this](int c_id, unsigned char* p) {ProcessTimerStart(c_id, p); });
	RegisterRecvFunction(SC_PACKET_ACTIVE_ESCAPE, [this](int c_id, unsigned char* p) {ProcessActiveEscape(c_id, p); });
	RegisterRecvFunction(SC_PACKET_ACTIVE_SPECIAL_ESCAPE, [this](int c_id, unsigned char* p) {ProcessActiveSpecialEscape(c_id, p); });
	RegisterRecvFunction(SC_PACKET_OPEN_SAFE, [this](int c_id, unsigned char* p) {ProcessOpenSafe(c_id, p); });
	RegisterRecvFunction(SC_PACKET_INVINCIBLE, [this](int c_id, unsigned char* p) {ProcessInvincible(c_id, p); });
	RegisterRecvFunction(SC_PACKET_INVINCIBLE_END, [this](int c_id, unsigned char* p) {ProcessInvincibleEnd(c_id, p); });
	RegisterRecvFunction(SC_PACKET_ITEM_USE, [this](int c_id, unsigned char* p) {ProcessItemUse(c_id, p); });
	RegisterRecvFunction(SC_PACKET_GAME_TIMER_START, [this](int c_id, unsigned char* p) {ProcessGameTimerStart(c_id, p); });
	RegisterRecvFunction(SC_PACKET_INTERACTION, [this](int c_id, unsigned char* p) {ProcessInteract(c_id, p); });
	RegisterRecvFunction(SC_PACKET_DIAMOND_OWNER_CHANGE, [this](int c_id, unsigned char* p) {ProcessDiamondOwnerChange(c_id, p); });
	RegisterRecvFunction(SC_PACKET_WIN, [this](int c_id, unsigned char* p) {ProcessWin(c_id, p); });
	RegisterRecvFunction(SC_PACKET_DEFEAT, [this](int c_id, unsigned char* p) {ProcessDefeat(c_id, p); });
}

void ThievesPacketManager::ProcessMove(int c_id, unsigned char* p)
{
	sc_packet_move* packet = reinterpret_cast<sc_packet_move*>(p);
	int mid = packet->id;
	auto mover = m_obj_map.find(packet->id);

	if (mover != m_obj_map.end())
	{
		//if (mover->second->GetIsActive() == false)return;
		if (isnan(packet->posX) ||  isnan(packet->posZ)) return;
		
		
		mover->second->SetPosition(Vec3(packet->posX, packet->posY, packet->posZ));
		
		//������ rot ���� look vector �̹Ƿ� �� �κ��� Rotation ���� ������ ������ �־�� ��.
		mover->second->SetRotation(Vec3(0.0f, atan2(packet->rotX, packet->rotZ), 0.0f));
		mover->second->SetActionType(packet->action_type);
	}
}

void ThievesPacketManager::ProcessSignin(int c_id, unsigned char* p)
{
	sc_packet_sign_in_ok* packet = reinterpret_cast<sc_packet_sign_in_ok*>(p);
	m_game_info.SetNetworkID(packet->id);


	GET_SINGLE(SceneManager)->SetCheckChangeScene(true);
	GEngine->SetChangeScene(L"Lobby");
	// Ŭ���
}

void ThievesPacketManager::ProcessObjInfo(int c_id, unsigned char* p)
{
	sc_packet_obj_info* packet = reinterpret_cast<sc_packet_obj_info*>(p);
	//NetworkObj* obj = NULL;
	Network::matching_end = true;
	NW_OBJ_TYPE nw_type;
	
	// �̰� ������ �ʹ� ������. �̷� ������� �����ָ� ������ -> ��������
	// 
	// ��Ŷ ���̵� ���� ������ ��Ʈ��ũ ���̵�� ���ٸ� ������Ʈ Ÿ���� ���� �÷��̾� Ÿ������, �ƴ϶�� ��Ŷ���� ���� ������Ʈ Ÿ������ �Ѵ�.
	
	
	if (0 <= packet->id && packet->id < MAX_USER)
	{
		packet->id == m_game_info.GetNetworkID() ?
			nw_type = NW_OBJ_TYPE::OT_MY_PLAYER : nw_type = NW_OBJ_TYPE::OT_PLAYER;
	}
	else if ((NPC_ID_START <= packet->id) && (packet->id < NPC_ID_END))
	{
		nw_type = NW_OBJ_TYPE::OT_NPC;
	}
	bool isNewData = true;
	if (m_obj_map.contains(packet->id)) isNewData = false;

	// �ش� �����͸� ��Ʈ��ũ�� id�� Ű�� �ؼ� ��Ʈ��ũ ������Ʈ�� �ִ´�.
	auto res = m_obj_map.try_emplace(packet->id, CreateSPtr<NetworkMoveObj>(
		packet->id,
		nw_type,
		packet->x,
		packet->y,
		packet->z
	));


	if (!isNewData) return;

	// ���� �̹� ������ ����ǰ� �ִ� ���¶�� ���Ӱ� ������ �־�� ��.
	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::GAME)
	{
		if (m_game_info.GetNetworkID() == packet->id) return;

		auto& objects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();


		for (auto& obj : objects)
		{
			auto nsys = obj->GetNetworkSystem();

			if (nsys)
			{
				if (nsys->GetNetworkingType() == NetworkType::NONE)
				{

					nsys->SetNetworkId(packet->id);

					switch (nw_type)
					{
						case NW_OBJ_TYPE::OT_PLAYER:
						{
							nsys->SetNetworkingType(NetworkType::OTHER_PLAYER);
						}
						break;
						case NW_OBJ_TYPE::OT_NPC:
						{
							nsys->SetNetworkingType(NetworkType::NPC);
						}
						// ���� OT_PLAYER �̿ܿ��� ���ͼ��� �ȵ�
						case NW_OBJ_TYPE::OT_MY_PLAYER:
						case NW_OBJ_TYPE::OT_NONE:
						default:
							break;
					}

					break;

				}
			}

		}
	}

	// ������Ʈ spqwn �� m_objmap[pakcet->id] �����Ͽ� id������ ����
}

void ThievesPacketManager::ProcessObjInfoEnd(int c_id, unsigned char* p)
{
	sc_packet_obj_info_end* packet = reinterpret_cast<sc_packet_obj_info_end*>(p);
	
	while (true) {
		if (GET_SINGLE(SceneManager)->GetCurrentLoadProgressScene() == CURRENT_SCENE::GAME) {
			shared_ptr<InGameScene> gScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetLoadProgressScene());
			gScene->SetAllObjInfo(true);
			break;
		}
		Sleep(0);
	}
}

void ThievesPacketManager::ProcessRemoveObj(int c_id, unsigned char* p)
{
	sc_packet_remove_object* packet = reinterpret_cast<sc_packet_remove_object*>(p);

	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::ROOM) {
		shared_ptr<RoomScene> rScene = static_pointer_cast<RoomScene>(GET_SINGLE(SceneManager)->GetActiveScene());
		rScene->RemovePlayerData(c_id);
	}

	m_obj_map.erase(packet->id);

	auto& objects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();

	for (auto& obj : objects) {

		auto nsys = obj->GetNetworkSystem();

		if (nsys)
		{
			if ((nsys->GetNetworkingType() != NetworkType::NONE) && (nsys->GetNetworkId() == packet->id))
			{
				nsys->SetNetworkId(-1);
				nsys->SetNetworkingType(NetworkType::NONE);
				nsys->GetTransform()->SetLocalPosition(Vec3(0.0f, -300.0f, 0.0f));
			}
		}
	}

}

void ThievesPacketManager::ProcessRoomsData(int c_id, unsigned char* p)
{
	sc_packet_rooms_data_for_lobby* packet = reinterpret_cast<sc_packet_rooms_data_for_lobby*>(p);


	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::LOBBY) {
		shared_ptr<LobbyScene> lScene = static_pointer_cast<LobbyScene>(GET_SINGLE(SceneManager)->GetActiveScene());
		lScene->InsertRoomData(packet->roomID, packet->playerNum, packet->isInGame);
	}
	else if (GET_SINGLE(SceneManager)->GetCurrentLoadProgressScene() == CURRENT_SCENE::LOBBY) {
		shared_ptr<LobbyScene> lScene = static_pointer_cast<LobbyScene>(GET_SINGLE(SceneManager)->GetLoadProgressScene());
		lScene->InsertRoomData(packet->roomID, packet->playerNum, packet->isInGame);
	}
	else {
		return;
	}
}

void ThievesPacketManager::ProcessRoomsDataEnd(int c_id, unsigned char* p)
{
	sc_packet_rooms_data_for_lobby_end* packet = reinterpret_cast<sc_packet_rooms_data_for_lobby_end*>(p);

	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::LOBBY) {
		shared_ptr<LobbyScene> lScene = static_pointer_cast<LobbyScene>(GET_SINGLE(SceneManager)->GetActiveScene());
		lScene->SetRecvDataEnd(true);
	}
	else if (GET_SINGLE(SceneManager)->GetCurrentLoadProgressScene() == CURRENT_SCENE::LOBBY) {
		shared_ptr<LobbyScene> lScene = static_pointer_cast<LobbyScene>(GET_SINGLE(SceneManager)->GetLoadProgressScene());
		lScene->SetRecvDataEnd(true);
	}
	else {
		return;
	}
}

void ThievesPacketManager::ProcessInRoomData(int c_id, unsigned char* p)
{
	sc_packet_rooms_data_for_room* packet = reinterpret_cast<sc_packet_rooms_data_for_room*>(p);
	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::ROOM) {
		shared_ptr<RoomScene> rScene = static_pointer_cast<RoomScene>(GET_SINGLE(SceneManager)->GetActiveScene());
		rScene->InsertPlayerData(packet->userId, packet->userName);
		rScene->SetPlayerReady(packet->userId, packet->isReady);

	}
	else if (GET_SINGLE(SceneManager)->GetCurrentLoadProgressScene() == CURRENT_SCENE::ROOM) {
		shared_ptr<RoomScene> rScene = static_pointer_cast<RoomScene>(GET_SINGLE(SceneManager)->GetLoadProgressScene());
		rScene->InsertPlayerData(packet->userId, packet->userName);
		rScene->SetPlayerReady(packet->userId, packet->isReady);
	}
	else {
		return;
	}

}

void ThievesPacketManager::ProcessInRoomDataEnd(int c_id, unsigned char* p)
{
	sc_packet_rooms_data_for_room_end* packet = reinterpret_cast<sc_packet_rooms_data_for_room_end*>(p);

	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::ROOM) {
		shared_ptr<RoomScene> rScene = static_pointer_cast<RoomScene>(GET_SINGLE(SceneManager)->GetActiveScene());
		rScene->SetRecvDataEnd(true);
		rScene->SetRoomMasterId(packet->master_id);
	}
	else if (GET_SINGLE(SceneManager)->GetCurrentLoadProgressScene() == CURRENT_SCENE::ROOM) {
		shared_ptr<RoomScene> rScene = static_pointer_cast<RoomScene>(GET_SINGLE(SceneManager)->GetLoadProgressScene());
		rScene->SetRecvDataEnd(true);
		rScene->SetRoomMasterId(packet->master_id);
	}
	else {
		return;
	}
}

void ThievesPacketManager::ProcessPlayerLeft(int c_id, unsigned char* p)
{
	sc_packet_leave_room* packet = reinterpret_cast<sc_packet_leave_room*>(p);
	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::ROOM) {
		shared_ptr<RoomScene> rScene = static_pointer_cast<RoomScene>(GET_SINGLE(SceneManager)->GetActiveScene());
		rScene->RemovePlayerData(packet->id);
		if (packet->master_id != -1) {
			rScene->SetRoomMasterId(packet->master_id);
		}
	}
	else if (GET_SINGLE(SceneManager)->GetCurrentLoadProgressScene() == CURRENT_SCENE::ROOM) {
		shared_ptr<RoomScene> rScene = static_pointer_cast<RoomScene>(GET_SINGLE(SceneManager)->GetLoadProgressScene());
		rScene->RemovePlayerData(packet->id);
		if (packet->master_id != -1) {
			rScene->SetRoomMasterId(packet->master_id);
		}
	}
	else {
		return;
	}
}

void ThievesPacketManager::ProcessPlayerJoin(int c_id, unsigned char* p)
{
	sc_packet_enter_room* packet = reinterpret_cast<sc_packet_enter_room*>(p);
	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::ROOM) {
		shared_ptr<RoomScene> rScene = static_pointer_cast<RoomScene>(GET_SINGLE(SceneManager)->GetActiveScene());
		rScene->InsertPlayerData(packet->id, packet->userName);
		rScene->SetPlayerReady(packet->id, false);
	}
	else if (GET_SINGLE(SceneManager)->GetCurrentLoadProgressScene() == CURRENT_SCENE::ROOM) {
		shared_ptr<RoomScene> rScene = static_pointer_cast<RoomScene>(GET_SINGLE(SceneManager)->GetLoadProgressScene());
		rScene->InsertPlayerData(packet->id, packet->userName);
		rScene->SetPlayerReady(packet->id, false);
	}
	else {
		return;
	}
}

void ThievesPacketManager::ProcessPlayerReady(int c_id, unsigned char* p)
{
	sc_packet_player_ready* packet = reinterpret_cast<sc_packet_player_ready*>(p);

	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::ROOM) {
		shared_ptr<RoomScene> rScene = static_pointer_cast<RoomScene>(GET_SINGLE(SceneManager)->GetActiveScene());
		rScene->SetPlayerReady(packet->id, true);
	}
	else if (GET_SINGLE(SceneManager)->GetCurrentLoadProgressScene() == CURRENT_SCENE::ROOM) {
		shared_ptr<RoomScene> rScene = static_pointer_cast<RoomScene>(GET_SINGLE(SceneManager)->GetLoadProgressScene());
		rScene->SetPlayerReady(packet->id, true);
	}
	else {
		return;
	}
}

void ThievesPacketManager::ProcessPlayerCancleReady(int c_id, unsigned char* p)
{
	sc_packet_player_cancle_ready* packet = reinterpret_cast<sc_packet_player_cancle_ready*>(p);

	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::ROOM) {
		shared_ptr<RoomScene> rScene = static_pointer_cast<RoomScene>(GET_SINGLE(SceneManager)->GetActiveScene());
		rScene->SetPlayerReady(packet->id, false);
	}
	else if (GET_SINGLE(SceneManager)->GetCurrentLoadProgressScene() == CURRENT_SCENE::ROOM) {
		shared_ptr<RoomScene> rScene = static_pointer_cast<RoomScene>(GET_SINGLE(SceneManager)->GetLoadProgressScene());
		rScene->SetPlayerReady(packet->id, false);
	}
	else {
		return;
	}
}

void ThievesPacketManager::ProcessEnterRoomOk(int c_id, unsigned char* p)
{
	sc_packet_enter_room_ok* packet = reinterpret_cast<sc_packet_enter_room_ok*>(p);
	
	GET_SINGLE(SceneManager)->SetRoomNum(packet->room_id);

	GET_SINGLE(SceneManager)->SetCheckChangeScene(true);
	GEngine->SetChangeScene(L"Room");
}

void ThievesPacketManager::ProcessAllPlayerLoadend(int c_id, unsigned char* p)
{
	sc_packet_all_player_load_end* packet = reinterpret_cast<sc_packet_all_player_load_end*>(p);

	shared_ptr<InGameScene> iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetLoadProgressScene());
	iScene->SetAllPlayerLoaded(true);
}

void ThievesPacketManager::ProcessError(int c_id, unsigned char* p)
{
	sc_packet_error* packet = reinterpret_cast<sc_packet_error*>(p);

	switch (packet->err_type)
	{
	case ERROR_ROOM_NOT_EXIST:

	case ERROR_GAME_IN_PROGRESS:
		break;
	case ERROR_ROOM_IS_FULL:
		break;
	case ERROR_PLAYER_NOT_READY:
		break;
	default:
		break;
	}

}

void ThievesPacketManager::ProcessGameStart(int c_id, unsigned char* p)
{
	sc_packet_game_start* packet = reinterpret_cast<sc_packet_game_start*>(p);

	GET_SINGLE(SceneManager)->SetCheckChangeScene(true);
	GEngine->SetChangeScene(L"Game");
}


void ThievesPacketManager::ProcessBullet(int c_id, unsigned char* p)
{
	sc_packet_bullet* packet = reinterpret_cast<sc_packet_bullet*>(p);

	// �ڵ� �߰�
	// �浹�� ��ǥ�� �޾Ƽ� �Ѿ� ���� �׸��� �Լ� ȣ��
	// 
}

void ThievesPacketManager::ProcessPhaseChange(int c_id, unsigned char* p)
{
	sc_packet_phase_change* packet = reinterpret_cast<sc_packet_phase_change*>(p);
	
	SetDiamondPlayer(packet->diamond_player);

	shared_ptr<InGameScene> iScene;
	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::GAME)
	{
		iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetActiveScene());
		iScene->SetPhaseChanged(true);
		iScene->SetPhaseChangedTime(std::chrono::system_clock::now());
	}
	else if (GET_SINGLE(SceneManager)->GetCurrentLoadProgressScene() == CURRENT_SCENE::GAME)
	{
		iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetLoadProgressScene());
		iScene->SetPhaseChanged(true);
		iScene->SetPhaseChangedTime(std::chrono::system_clock::now());
	}
	
	//페이즈 변경 시 빛 처리

}

void ThievesPacketManager::ProcessAttack(int c_id, unsigned char* p)
{
	sc_packet_attack* packet = reinterpret_cast<sc_packet_attack*>(p);
	
}

void ThievesPacketManager::ProcessHit(int c_id, unsigned char* p)
{
	sc_packet_stun* packet = reinterpret_cast<sc_packet_stun*>(p);

	if (packet->stun_by_item)
	{
		m_item_map.find(packet->attack_player)->second->SetState(ITEM_STATE::IT_NONE);
	}

	m_obj_map.find(packet->stun_player)->second->SetIsStun(true);

}

void ThievesPacketManager::ProcessStunEnd(int c_id, unsigned char* p)
{
	sc_packet_stun_end* packet = reinterpret_cast<sc_packet_stun_end*>(p);
	
	m_obj_map.find(c_id)->second->SetIsStun(false);
}

void ThievesPacketManager::ProcessGetItem(int c_id, unsigned char* p)
{
	sc_packet_get_item* packet = reinterpret_cast<sc_packet_get_item*>(p);

	shared_ptr<InGameScene> iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetActiveScene());
	
	m_item_map.find(packet->obj_id)->second->SetState(ITEM_STATE::IT_OCCUPIED);

	if (packet->player == m_game_info.GetNetworkID())
	{
		iScene->SetItemNum(packet->obj_id);


		//// 아이템 획득시 UI
		//if (m_item_map.find(packet->obj_id)->second->GetItemType() == ITEM_NUM_GUN)
		//{
		//	particleCheck = true;
		//	for (auto& GameObject : iScene->GetGameObjects())
		//	{
		//		if (GameObject->GetName() == L"ItemBox_Gun")
		//		{
		//			Vec3 pos = GameObject->GetTransform()->GetLocalPosition();
		//			pos.z = 499.f;
		//			GameObject->GetTransform()->SetLocalPosition(pos);
		//		}
		//		else if (GameObject->GetName() == L"ParticleGetItem")
		//		{
		//			GameObject->GetParticleSystem()->UseParticle(true);
		//		}
		//	}

		//}
		//else if (m_item_map.find(packet->obj_id)->second->GetItemType() == ITEM_NUM_TRAP)
		//{
		//	particleCheck = true;
		//	for (auto& GameObject : iScene->GetGameObjects())
		//	{
		//		if (GameObject->GetName() == L"ItemBox_Trap")
		//		{
		//			Vec3 pos = GameObject->GetTransform()->GetLocalPosition();
		//			pos.z = 499.f;
		//			GameObject->GetTransform()->SetLocalPosition(pos);
		//		}
		//		else if (GameObject->GetName() == L"ParticleGetItem")
		//		{
		//			GameObject->GetParticleSystem()->UseParticle(true);
		//		}
		//	}

		//}


		//if (particleCheck == true)
		//{
		//	particleTime += DELTA_TIME;

		//	if (particleTime > 2.f)
		//	{
		//		for (auto& GameObject : iScene->GetGameObjects())
		//		{
		//			if (GameObject->GetName() == L"ParticleGetItem")
		//			{
		//				GameObject->GetParticleSystem()->UseParticle(false);
		//			}
		//		}
		//		particleTime = 0.f;
		//		particleCheck = false;
		//	}
		//}

	}
}

void ThievesPacketManager::ProcessItemUse(int c_id, unsigned char* p)
{
	sc_packet_item_use* packet = reinterpret_cast<sc_packet_item_use*>(p);
	
	auto mItem = m_item_map.find(packet->obj_id)->second;
	mItem->SetState(ITEM_STATE::IT_SET);
	mItem->SetPosition(Vec3(packet->x, packet->y, packet->z));


}

void ThievesPacketManager::ProcessItemInfo(int c_id, unsigned char* p)
{
	sc_packet_item_info* packet = reinterpret_cast<sc_packet_item_info*>(p);

	const char* name;

	if (packet->item_type == ITEM_NUM_TRAP)
	{
		name = "Trap";
	}
	else if (packet->item_type == ITEM_NUM_GUN)
	{
		name = "Gun";
	}
	else
	{
		name = "Another";
	}

	m_item_map.try_emplace(packet->id, make_shared<MapItem>(
		packet->id,
		name,
		packet->x,
		packet->y,
		packet->z,
		packet->item_type,
		ITEM_STATE::IT_SPAWN
		));
}

void ThievesPacketManager::ProcessTimerStart(int c_id, unsigned char* p)
{
	sc_packet_timer_start* packet = reinterpret_cast<sc_packet_timer_start*>(p);
	
	shared_ptr<InGameScene> iScene;
	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::GAME)
	{
		iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetActiveScene());
		iScene->SetStartTime(std::chrono::system_clock::now());
		iScene->SetIsGetTime(true);
	}
	else if (GET_SINGLE(SceneManager)->GetCurrentLoadProgressScene() == CURRENT_SCENE::GAME)
	{
		iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetLoadProgressScene());
		iScene->SetStartTime(std::chrono::system_clock::now());
		iScene->SetIsGetTime(true);
	}
 
}

void ThievesPacketManager::ProcessActiveEscape(int c_id, unsigned char* p)
{
	sc_packet_active_escape* packet = reinterpret_cast<sc_packet_active_escape*>(p);
	
	shared_ptr<InGameScene> iScene;
	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::GAME)
	{
		iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetLoadProgressScene());
		iScene->SetIsActiveEscape(true);
		iScene->SetActiveEscapeTime(std::chrono::system_clock::now());

		for (auto& go : iScene->GetGameObjects())
		{
			if (go->GetName() == L"EscapeZone1")
			{
				go->GetTransform()->SetLocalPosition(Vec3(packet->x[0], packet->y[0], packet->z[0]));
			}
			else if (go->GetName() == L"EscapeZone2")
			{
				go->GetTransform()->SetLocalPosition(Vec3(packet->x[1], packet->y[1], packet->z[1]));
			}
			else if (go->GetName() == L"EscapeZone3")
			{
				go->GetTransform()->SetLocalPosition(Vec3(packet->x[2], packet->y[2], packet->z[2]));
			}
			else
			{
				continue;
			}
		}
	}
	else if (GET_SINGLE(SceneManager)->GetCurrentLoadProgressScene() == CURRENT_SCENE::GAME)
	{
		iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetActiveScene());
		iScene->SetIsActiveEscape(true);
		iScene->SetActiveEscapeTime(std::chrono::system_clock::now());

		for (auto& go : iScene->GetGameObjects())
		{
			if (go->GetName() == L"EscapeZone1")
			{
				go->GetTransform()->SetLocalPosition(Vec3(packet->x[0], packet->y[0], packet->z[0]));
			}
			else if (go->GetName() == L"EscapeZone2")
			{
				go->GetTransform()->SetLocalPosition(Vec3(packet->x[1], packet->y[1], packet->z[1]));
			}
			else if (go->GetName() == L"EscapeZone3")
			{
				go->GetTransform()->SetLocalPosition(Vec3(packet->x[2], packet->y[2], packet->z[2]));
			}
			else
			{
				continue;
			}
		}
	}
	
	// 오브젝트 활성화 처리 해야함.

}


void ThievesPacketManager::ProcessActiveSpecialEscape(int c_id, unsigned char* p)
{
	sc_packet_active_special_escape* packet = reinterpret_cast<sc_packet_active_special_escape*>(p);

	shared_ptr<InGameScene> iScene;
	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::GAME)
	{
		iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetLoadProgressScene());
		iScene->SetIsActiveSpecialEscape(true);
		iScene->SetActiveSpecialEscapeTime(std::chrono::system_clock::now());

		for (auto& go : iScene->GetGameObjects())
		{
			if (go->GetName() == L"SpecialEscapeZone")
			{
				go->GetTransform()->SetLocalPosition(Vec3(packet->x, packet->y, packet->z));
				break;
			}
		}
	}
	else if (GET_SINGLE(SceneManager)->GetCurrentLoadProgressScene() == CURRENT_SCENE::GAME)
	{
		iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetActiveScene());
		iScene->SetIsActiveEscape(true);
		iScene->SetActiveSpecialEscapeTime(std::chrono::system_clock::now());

		for (auto& go : iScene->GetGameObjects())
		{
			if (go->GetName() == L"SpecialEscapeZone")
			{
				go->GetTransform()->SetLocalPosition(Vec3(packet->x, packet->y, packet->z));
				break;
			}
		}
	}

	// 오브젝트 활성활 처리 해야함.
}

void ThievesPacketManager::ProcessOpenSafe(int c_id, unsigned char* p)
{
	sc_packet_open_safe* packet = reinterpret_cast<sc_packet_open_safe*>(p);


	shared_ptr<InGameScene> iScene;
	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::GAME)
	{
		iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetLoadProgressScene());
		iScene->SetIsOpenSafe(true);
		iScene->SetOpenSafeTime(std::chrono::system_clock::now());
	}
	else if (GET_SINGLE(SceneManager)->GetCurrentLoadProgressScene() == CURRENT_SCENE::GAME)
	{
		iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetActiveScene());
		iScene->SetIsOpenSafe(true);
		iScene->SetOpenSafeTime(std::chrono::system_clock::now());
	}
	
	auto& objects = GET_SINGLE(SceneManager)->GetActiveScene()->GetGameObjects();

	for (auto& obj : objects)
	{
		if (obj->GetName() == L"Safe")
		{
			obj->GetTransform()->SetLocalPosition(Vec3(0.f, -4000.f, 0.f));
		}
		else if (obj->GetName() == L"OpenedSafe")
		{
			obj->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0.f));
		}
		else {
			continue;
		}
	}
}

void ThievesPacketManager::ProcessInvincible(int c_id, unsigned char* p)
{
	sc_packet_invincible* packet = reinterpret_cast<sc_packet_invincible*>(p);

	m_obj_map.find(packet->player)->second->SetIsInvincible(true);
}

void ThievesPacketManager::ProcessInvincibleEnd(int c_id, unsigned char* p)
{
	sc_packet_invincible_end* packet = reinterpret_cast<sc_packet_invincible_end*>(p);
	m_obj_map.find(packet->player)->second->SetIsInvincible(false);
}

void ThievesPacketManager::ProcessGameTimerStart(int c_id, unsigned char* p)
{
	
	shared_ptr<InGameScene> iScene;
	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::GAME)
	{
		iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetLoadProgressScene());
		iScene->SetIsTimerStart(true);
	}
	else if (GET_SINGLE(SceneManager)->GetCurrentLoadProgressScene() == CURRENT_SCENE::GAME)
	{
		iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetActiveScene());
		iScene->SetIsTimerStart(true);
	}
}

void ThievesPacketManager::ProcessInteract(int c_id, unsigned char* p)
{
	sc_packet_interaction* packet = reinterpret_cast<sc_packet_interaction*>(p);
	
	shared_ptr<InGameScene> iScene;
	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::GAME)
	{
		iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetLoadProgressScene());
		iScene->SetIsInteractOn(packet->interaction_on);
	}
	else if (GET_SINGLE(SceneManager)->GetCurrentLoadProgressScene() == CURRENT_SCENE::GAME)
	{
		iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetActiveScene());
		iScene->SetIsTimerStart(packet->interaction_on);
	}
}

void ThievesPacketManager::ProcessDiamondOwnerChange(int c_id, unsigned char* p)
{
	sc_packet_diamond_owner_change* packet = reinterpret_cast<sc_packet_diamond_owner_change*>(p);

	SetDiamondPlayer(packet->new_owner);
}

void ThievesPacketManager::ProcessWin(int c_id, unsigned char* p)
{
	shared_ptr<InGameScene> iScene;
	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::GAME)
	{
		iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetLoadProgressScene());
		iScene->SetIsGameEnd(true);
		iScene->SetIsPlayerWin(true);
	}
	else if (GET_SINGLE(SceneManager)->GetCurrentLoadProgressScene() == CURRENT_SCENE::GAME)
	{
		iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetActiveScene());
		iScene->SetIsGameEnd(true);
		iScene->SetIsPlayerWin(true);
	}
}

void ThievesPacketManager::ProcessDefeat(int C_id, unsigned char* p)
{
	shared_ptr<InGameScene> iScene;
	if (GET_SINGLE(SceneManager)->GetCurrentScene() == CURRENT_SCENE::GAME)
	{
		iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetLoadProgressScene());
		iScene->SetIsGameEnd(true);
		iScene->SetIsPlayerWin(false);
	}
	else if (GET_SINGLE(SceneManager)->GetCurrentLoadProgressScene() == CURRENT_SCENE::GAME)
	{
		iScene = static_pointer_cast<InGameScene>(GET_SINGLE(SceneManager)->GetActiveScene());
		iScene->SetIsGameEnd(true);
		iScene->SetIsPlayerWin(false);
	}
}

////----------------------수정이 필요
// timer
void ThievesPacketManager::ProcessTime(int c_id, unsigned char* p)
{
	sc_packet_time* packet = reinterpret_cast<sc_packet_time*>(p);
	int t = packet->send_time;
	//
}
// NPC
void ThievesPacketManager::ProcessNpcAttack(int c_id, unsigned char* p)
{
	sc_packet_npc_attack* packet = reinterpret_cast<sc_packet_npc_attack*>(p);
	auto target = m_obj_map.find(packet->target_id);
	if (target != m_obj_map.end()) {
		if (target->second->GetIsActive() == false)return;

		Vec3 attack_pos{ target->second->GetPosition().x,target->second->GetPosition().y + 80.0f,target->second->GetPosition().z };



	}
}


void ThievesPacketManager::Reset()
{
	//for (auto& obj : m_obj_map)
	//{
	//	if (obj.second->GetIsActive() == true) {
	//		if (m_connected_actor_map.find(id) != m_connected_actor_map.cend()
	//					&& m_connected_actor_map[id] != nullptr)
	//				{
	//					SPtr<Actor> actor = m_connected_actor_map[id];
	//					actor->ConnectServer(false);
	//					m_connected_actor_map[id] = nullptr;
	//					return actor;
	//				}
	//				else
	//				{
	//					//LOG_WARN("ID : {0} is already disconnected", id);
	//					return nullptr;
	//				}
	//	}
	//		
	//	obj.second = nullptr;

	//}
	//m_obj_map.clear();
}