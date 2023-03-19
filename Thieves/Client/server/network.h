#pragma once
#include "pch.h"
#include"server/define.h"
#include <thread>
#include<iostream>
#include<string>

#include<client_core.h>
//#include"client/math/quaternion.h"
//#include"client/math/Vec3.h"
#include"packet_manager.h"
#include"send_manager.h"
class PacketManager;
class SendManager;
class Network
{
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
public:
	Network() {
		//���߿��� object �����̳ʿ� �ִ� ������Ʈ ��Ŷ���� �ʱ�ȭ



	};
	~Network() {
		closesocket(m_s_socket);
		WSACleanup();
		if (worker.joinable())
			worker.join();
	};

	bool Init(client_fw::UPtr<PacketManager>&& packet_manager, client_fw::UPtr<SendManager>&& send_manager);
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

	//void SendPacket(int num_byte ,void* packet);

	void SendMessageToServer(const client_fw::SPtr<client_fw::MessageEventInfo>& message);
	void SendMovePacket(const client_fw::Vec3& position, const client_fw::Quaternion& rotation);
	SOCKET& GetSock() { return m_s_socket; }
	static bool matching_end;
private:
	void Worker();

	void OnRecv(int client_id, EXP_OVER* exp_over, DWORD num_byte, SOCKET& socket);

	int GetID() const
	{
		return m_id;
	}

	SOCKET m_s_socket;
	HANDLE m_hiocp;

	std::thread worker;
	client_fw::UPtr<PacketManager>m_packet_manager;
	client_fw::UPtr<SendManager>m_send_manager;
	std::chrono::system_clock::time_point m_move_time;
	int m_id;
	int m_prev_size = 0;
	bool m_iswork = true;

};