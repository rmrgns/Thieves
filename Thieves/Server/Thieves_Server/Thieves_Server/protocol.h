#pragma once

const short SERVER_PORT = 7777;

// �� ũ��

const int MAX_NAME_SIZE = 20;		// ���̵� ������
const int MAX_PASSWORD_SIZE = 20;	// ��� ��ȣ ������
const int MAX_CHAT_SIZE = 100;		// ä�� ������

const int MAX_ROOM_SIZE = 1000;		// �� �ִ� ũ��

const int MAX_USER = MAX_ROOM_SIZE * 8;		// ���� ���� �ο�


// ���� NPC�� ����
// ���� AI�� ����
const int PLAYER_HP = 5;		// �÷��̾� HP
const int PLAYER_DAMAGE = 1;	// �÷��̾� DMG
// �÷��̾� �̵��Ÿ�
// �÷��̾� ���� ��Ÿ�
// ID

// CS
const char CS_PACKET_SIGN_IN = 1;			// �α���
const char CS_PACKET_SIGN_UP = 2;			// ����
const char CS_PACKET_MOVE = 3;				// �̵�
const char CS_PACKET_ATTACK = 4;			// ����
const char CS_PACKET_CHAT = 5;				// ä��
const char CS_PACKET_HIT = 6;				// �ǰ�
const char CS_PACKET_GAME_START = 7;		// ���� ����

// SC
const char SC_PACKET_SIGN_IN_OK = 1;		// �α��� OK
const char SC_PACKET_SIGN_UP_OK = 2;		// ���� OK
const char SC_PACKET_MOVE = 3;				// �̵�
const char SC_PACKET_PUT_OBJECT = 4;		// �÷��̾� ����
const char SC_PACKET_REMOVE_OBJECT = 5;		// �÷��̾� ����
const char SC_PACKET_CHAT = 6;				// ä��
const char SC_PACKET_LOGIN_FAIL = 7;		// �α��� ����
const char SC_PACKET_TIME = 8;
const char SC_PACKET_POLICE_ATTACK = 9;		// ���� ����
const char SC_PACKET_PLAYER_ATTACK = 10;	// �÷��̾� ����
const char SC_PACKET_WIN = 11;				// win
const char SC_PACKET_DEFEAT = 12;			// defeat
const char SC_PACKET_STUN = 13;				// stun
const char SC_PAKCET_INTERACTION = 14;		// ��ȣ�ۿ�
const char SC_PACKET_PHASE = 15;			// ������ ����  

struct cs_packet_sign_in {};
struct cs_packet_sign_up {};
struct cs_packet_move {};
struct cs_packet_attack {};
struct cs_packet_chat {};
struct cs_packet_hit {};
struct cs_packet_game_start {};

struct sc_packet_sign_in_ok {};
struct sc_packet_sign_up_ok {};
struct sc_packet_move {};
struct sc_packet_put_object {};
struct sc_packet_remove_object {};
struct sc_packet_chat {};
struct sc_packet_login_fail {};
struct sc_packet_time {};
struct sc_packet_police_attack {};
struct sc_packet_player_attack {};
struct sc_packet_win {};
struct sc_packet_defeat {};
struct sc_packet_stun {};
struct sc_pakcet_interaction {};