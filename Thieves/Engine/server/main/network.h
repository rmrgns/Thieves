#pragma once

#include "server/define.h"

#include <thread>
#include <iostream>
#include <string>
#include "packet_manager.h"
#include "send_manager.h"
#include "server/thieves_server/thieves_packet_manager.h"
#include "server/thieves_server/thieves_send_manager.h"
#include "server/ptr.h"
class Network {
private:
	static Network* m_pInst;
public:
	static Network* GetInst()
	{
		if (!m_pInst)
			m_pInst = new Network;
		return m_pInst;
	}
	static void DestroyInst()
	{
		if (m_pInst)
		{
			delete m_pInst;
			m_pInst = NULL;
		}
	}

private:
	SOCKET m_s_socket;
	HANDLE m_hiocp;

	std::thread worker;

	int m_id;
	int m_prev_size = 0;
	bool m_iswork = true;
public:
	Network() {};
	~Network() {
		closesocket(m_s_socket);
		WSACleanup();
		if (worker.joinable())
			worker.join();
	};

	bool Init(client_fw::UPtr<ThievesPacketManager>&& packet_manager, client_fw::UPtr<ThievesSendManager>&& send_manager);
	bool Connect();

	static void error_display(int err_no)
	{
		WCHAR* lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, err_no,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf, 0, 0);
		std::wcout << lpMsgBuf << std::endl;

		//LOG_INFO(lpMsgBuf);
		//while (true);

		LocalFree(lpMsgBuf);
	}

	void DestroyWorker()
	{
		worker.detach();
		m_iswork = false;
		//worker.join();
	}
	void CreateWorker()
	{

		worker = std::thread([this]() {Worker(); });
		//worker.join();
	}

	client_fw::UPtr<ThievesPacketManager>& GetPacketManager() { return m_packet_manager; };
	client_fw::UPtr<ThievesSendManager>& GetSendManager() { return m_send_manager; };

	std::unordered_map<int, shared_ptr<NetworkMoveObj>>& GetNetworkObjMap() { return m_packet_manager->GetObjMap(); }


	void SendMessageToServer(const shared_ptr<MessageEventInfo>& message);
	void SendMovePacket(char direction, Vec3 pos, Vec3 vec, float deltatime, char);
	void SendLoadProgressPacket(char progressed);
	void SendLoadEndPacket();
	void SendStartPacket();
	void SendSignInPacket();
	void SendEnterRoom(int room_id);
	void SendLeaveRoom();
	void SendReady();
	void SendCancleReady();
	void SendLogOut();
	void SendRequestRoomsData();
	void SendRequestInRoomData();
	void SendBullet(Vec3, Vec3);

	void SendPacket();
	
	SOCKET& GetSock() { return m_s_socket; }
	static bool matching_end;
private:
	void Worker();

	void OnRecv(int client_id, EXP_OVER* exp_over, DWORD num_byte, SOCKET& socket);

	int GetID() const
	{
		return m_id;
	}

	client_fw::UPtr<ThievesPacketManager>m_packet_manager;
	client_fw::UPtr<ThievesSendManager>m_send_manager;
	// 이전 이동 패킷이 전송된 시점을 저장하는 변수
	std::chrono::system_clock::time_point m_move_time;

};