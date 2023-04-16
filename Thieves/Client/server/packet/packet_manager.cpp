#include "pch.h"

#include "packet_manager.h"
#include <iostream>


PacketManager::PacketManager() :m_id(0), m_prev_size(0)
{
	m_stop_recv = false;
}

PacketManager::~PacketManager()
{
}

// 소켓으로 부터 받은 데이터를 처리
// 데이터의 크기와 reamin_data를 이용 패킷 단위로 데이터를 처리
void PacketManager::ProcessRecv(int client_id, EXP_OVER* exp_over, DWORD num_byte, SOCKET& socket)
{
	if (num_byte == 0)
	{
		std::cout << "받은게 없음" << std::endl;
	}

	int remain_data = num_byte + m_prev_size;
	unsigned char* packet_start = exp_over->_net_buf;
	int packet_size = packet_start[0];
	while (packet_size <= remain_data) {
		ProcessPacket(client_id, packet_start);
		remain_data -= packet_size;
		packet_start += packet_size;
		if (remain_data > 0) packet_size = packet_start[0];
		else break;
	}
	std::cout << "패킷 처리" << std::endl;

	if (0 < remain_data) {
		m_prev_size = remain_data;
		memcpy(&exp_over->_net_buf, packet_start, remain_data);
	}
	if (remain_data == 0)
		m_prev_size = 0;

	DoRecv(socket);
}

void PacketManager::RegisterRecvFunction(char key, const std::function<void(int, unsigned char*)>& recv_func)
{
	m_recv_func_map[key] = recv_func;
}

// 패킷을 처리하는 함수 
void PacketManager::ProcessPacket(int c_id, unsigned char* p)
{
	unsigned char packet_type = p[1];

	auto iter = m_recv_func_map.find(packet_type);
	if (iter != m_recv_func_map.end() && false == m_stop_recv)
	{
		//(this->*(iter->second))(c_id, p);
		(iter->second)(c_id, p);
	}



}

// 소켓으로 부터 데이터를 받는 함수
void PacketManager::DoRecv(SOCKET& socket)
{
	DWORD recv_flag = 0;

	ZeroMemory(&recv_over._wsa_over, sizeof(recv_over._wsa_over));
	recv_over._wsa_buf.buf = reinterpret_cast<char*>(recv_over._net_buf + m_prev_size);
	recv_over._wsa_buf.len = sizeof(recv_over._net_buf) - m_prev_size;
	int ret = WSARecv(socket, &recv_over._wsa_buf, 1, 0, &recv_flag, &recv_over._wsa_over, NULL);
	if (SOCKET_ERROR == ret) {
		int error_num = WSAGetLastError();
		if (ERROR_IO_PENDING != error_num)
			std::cout << "error" << std::endl;

	}
}

