#pragma once
/*
패킷을 관리하는 PacketManager
*/
#include "../define.h"

#include<functional>
#include<unordered_map>

class PacketManager
{
public:
	PacketManager();
	virtual ~PacketManager();

	virtual void Init() {};
	void ProcessRecv(int client_id, EXP_OVER* exp_over, DWORD num_byte, SOCKET& socket);
	void ProcessPacket(int client_id, unsigned char* p);
	// 수정필요
	void RegisterRecvFunction(char key, const std::function<void(int, unsigned char*)>& recv_func);
	void DoRecv(SOCKET& socket);

	int GetID() { return m_id; }
	void SetID(int val) { m_id = val; }
	int GetPrevSize() { return m_prev_size; }
	void SetPrevSize(int val) { m_prev_size = val; }

protected:
	bool m_stop_recv;
	int m_id;
	int m_prev_size = 0;
	EXP_OVER recv_over;
	std::unordered_map<char, std::function<void(int, unsigned char*)> >m_recv_func_map;
};


