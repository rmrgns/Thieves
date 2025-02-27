#pragma once

#include <WS2tcpip.h>
#include <MSWSock.h>
#include "state.h"
#pragma comment (lib, "WS2_32.LIB")
#pragma comment (lib, "MSWSock.LIB")
//
////
#include"protocol.h"
const int BUFSIZE = 2048;

enum class COMP_OP {
	OP_RECV,
	OP_SEND,
	OP_ACCEPT,
	OP_STUN_END,
	OP_INVINCIBLE_END,
	OP_NPC_MOVE,
	OP_NPC_SPAWN,
	OP_COUNT_TIME,
	OP_NPC_ATTACK

};

class EXP_OVER
{
public:
	WSAOVERLAPPED _wsa_over;
	COMP_OP		  _comp_op;
	WSABUF		  _wsa_buf;
	unsigned char _net_buf[BUFSIZE];
public:

	EXP_OVER(COMP_OP comp_op, char num_bytes, void* mess) : _comp_op(comp_op)
	{
		ZeroMemory(&_wsa_over, sizeof(_wsa_over));
		_wsa_buf.buf = reinterpret_cast<char*>(_net_buf);
		_wsa_buf.len = num_bytes;
		memcpy(_net_buf, mess, num_bytes);
	};
	EXP_OVER(COMP_OP comp_op) :_comp_op(comp_op) {}
	EXP_OVER() { _comp_op = COMP_OP::OP_RECV; }
	~EXP_OVER() {}


};