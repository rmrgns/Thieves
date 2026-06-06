#pragma once
#include "protocol.hpp"
#include <span>
#include <array>
#include <functional>
#include <iostream>

class PacketManager
{
// 싱글톤 구현.
private:
	using HandlerFunc = std::function<void(int, std::span<char>)>;
	std::array<HandlerFunc, 100> handlers; // 100은 최대 패킷 수.

	PacketManager() {
		// 일일이 레지스터 하는거 너무 힘드니까, 그냥 BIND_PACKET 이라는 걸 미리 정의 한뒤에, 나중에 전부 레지스터로 만들어 버리도록 하는 X-Macro 방식을 채용.
#define BIND_PACKET(PKT_ID, PKT_TYPE, HANDLER_FUNC)\
	Register<PKT_TYPE>(PKT_ID, [this](int id, PKT_TYPE* p) {HANDLER_FUNC(id, p); });

#include "PacketList.h"


#undef BIND_PACKET

		std::cout << "PacketManager Created and Registered Functions.\n";
	};

	~PacketManager() = default;

	// 복사 생성 안 되도록 다른거 가져와도 그냥 끝내버리도록 만들기.
	PacketManager(const PacketManager& other) = delete; // 원래는 {}로 아무것도 안 써서 막았는데, C++11 이후부터는 delete로 그냥 차단 가능.
	PacketManager& operator=(const PacketManager& other) = delete;

public:
	static PacketManager& GetInst()
	{
		// Meyers Singleton
		// C++ 11이상에서는 static 변수가 Thread Safe 함.
		static PacketManager s;
		return s;
	}

	void Init();
	void ProcessPacket(int sessionId, std::span<char> packet);

private:
	template <typename T>
	void Register(int packetType, std::function<void(int, T*)> processFunc)
	{
		handlers[packetType] = [processFunc](int sessionId, std::span<char> packet)	{

				if (packet.size() < sizeof(T)) {
					// 패킷 크기가 엄청 크다? 뭔가 문제가 있는 거임
					return;
				}

				T* pPacket = reinterpret_cast<T*>(packet.data());
				processFunc(sessionId, pPacket);
		};

	}

	void ProcessSignIn(int sessionId, cs_packet_sign_in* packet);

	void ProcessSignUp(int sessionId, cs_packet_sign_up* packet);

	void ProcessAttack(int sessionId, cs_packet_attack* packet);

	void ProcessMove(int sessionId, cs_packet_move* packet);

	void ProcessHit(int sessionId, cs_packet_hit* packet);

	void ProcessGameStart(int sessionId, cs_packet_game_start* packet);

	void ProcessGetItem(int sessionId, cs_packet_get_item* packet);

	void ProcessUseItem(int sessionId, cs_packet_use_item* packet);

	void ChangePhase(int sessionId);

	void ProcessLoadProgressing(int sessionId, cs_packet_load_progressing* packet);

	void ProcessLoadEnd(int sessionId, cs_packet_load_end* packet);

	void ProcessEnterRoom(int sessionId, cs_packet_enter_room* packet);

	void ProcessLeaveRoom(int sessionId, cs_packet_leave_room* packet);

	void ProcessPlayerReady(int sessionId, cs_packet_player_ready* packet);

	void ProcessPlayerCancleReady(int sessionId, cs_packet_player_cancle_ready* packet);

	void ProcessLogOut(int sessionId, cs_packet_player_log_out* packet);

	void ProcessRoomsDataInLobby(int sessionId, cs_packet_request_rooms_data_for_lobby* packet);

	void ProcessRoomsDataInRoom(int sessionId, cs_packet_request_rooms_data_for_room* packet);

	void ProcessDamageCheat(int sessionId);

	void ProcessBullet(int sessionId, cs_packet_bullet* packet);



	void StartGame(int room_id);
};

