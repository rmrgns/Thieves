#pragma once
#include "iocp_server.h"

class PacketManager;
class InGameServer :
	public IOCPServer {

public:
	InGameServer();
	virtual ~InGameServer();

	virtual bool OnAccept(EXP_OVER* exp_over) override;
	virtual bool OnRecv(int c_id, EXP_OVER* exp_over, DWORD num_byte) override;
	virtual void OnEvent(int c_id, EXP_OVER* exp_over)override;
	virtual void Disconnect(int c_id) override;

	// Timer �κ� �Լ� �ѹ� �ٽ� ������ ����
	void CreateTimer();
	void Dotimer(HANDLE hiocp);
	void Run();
	void End();

private:
	std::unique_ptr<PacketManager>m_Packet;

};





