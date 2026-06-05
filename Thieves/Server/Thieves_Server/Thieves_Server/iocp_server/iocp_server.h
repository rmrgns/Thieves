#pragma once
#pragma comment (lib, "WS2_32.LIB")
#pragma comment (lib, "MSWSock.LIB")

#include "define.h"

#include <thread>
#include <vector>
#include <array>

class IOCPServer
{
public:
	IOCPServer();
	virtual ~IOCPServer();

	// Server initaiallize
	bool Init(const int);
	
	// Bind Listen
	bool BindListen(const int);

	// accept
	virtual bool OnAccept(EXP_OVER* exp_over) { return false; };
	
	
	virtual bool OnRecv(int c_id, EXP_OVER* exp_over, DWORD num_bytes) { return true; };
	virtual void OnEvent(int c_id, EXP_OVER* exp_over) { return; };
	virtual void Disconnect(int c_id) {}
	static  void error_display(int err_no);
	virtual bool StartServer();
public:
	void JoinThread();	
	void CreateWorker();
	void Worker();

protected:
	EXP_OVER	accept_ex;
	SOCKET		m_s_socket;
	HANDLE		m_hiocp;
	int			m_worker_num;
	std::vector <std::thread> m_worker_threads;


};
