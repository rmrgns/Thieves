#pragma once

const short SERVER_PORT = 7777;

// �� ũ��

const int MAX_NAME_SIZE = 20;		// ���̵� ������
const int MAX_PASSWORD_SIZE = 20;	// ��� ��ȣ ������
const int MAX_CHAT_SIZE = 100;		// ä�� ������

const int MAX_ROOM_SIZE = 1000;		// �� �ִ� ũ��

const int MAX_USER = MAX_ROOM_SIZE * 8;		// ���� ���� �ο�

// ����
const float FOV_RANGE = 0.f;

constexpr int MAX_NPC = 7;

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
const char CS_PACKET_MATCHING = 8;
const char CS_PACKET_TEST = 9;
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
const char SC_PACKET_TEST = 16;
//#pragma pack (push, 1)

struct cs_packet_move {
	unsigned char size;
	char	type;
	char	direction;			// 1 : ��,  2: ��, 3:��, 4:��
	int		move_time; //����� �� -> �����ð� -�����ð� = ����ϴ� �ð�
//	float	s_x, s_y, s_z;
//	float	f_x, f_y, f_z;
//	float	r_x, r_y, r_z;
//	float	l_x, l_y, l_z;
	float	posX, posY, posZ;	// ��ġ vec
	float	vecX, vecY, vecZ;	// look vec
	float	deltaTime;
};

struct cs_packet_test {
	unsigned char size;
	char	type;
	int		id;
	int		num;
};
struct cs_packet_sign_in {
	unsigned char size;
	char	type;
	char	name[MAX_NAME_SIZE];
	char	password[MAX_PASSWORD_SIZE];
};
struct cs_packet_sign_up {
	unsigned char size;
	char	type;
	char	name[MAX_NAME_SIZE];
	char	password[MAX_PASSWORD_SIZE];
};

struct cs_packet_attack {
	unsigned char size;
	char	type;
	//float f_x, f_y, f_z; 
};
struct cs_packet_chat {
	unsigned char size;
	char	type;
	char	message[MAX_CHAT_SIZE];
};
struct cs_packet_hit {
	unsigned char size;
	char	type;
	int victim_id;
	int attacker_id;
};
struct cs_packet_game_start {
	unsigned char size;
	char	type;
};

struct sc_packet_sign_in_ok {
	unsigned char size;
	char type;
	int		id;
	float	x, y, z;
};
struct sc_packet_sign_up_ok {
	unsigned char size;
	char type;
};
struct sc_packet_move {
	unsigned char size;
	char	type;
	int		id;
//	char	direction;			// 0 : ��,  1: ��, 2:��, 3:��
//	int		move_time; //����� �� -> �����ð� -�����ð� = ����ϴ� �ð�
	//float	s_x, s_y, s_z;
	//float	f_x, f_y, f_z;
	//float	r_x, r_y, r_z;
	float	posX, posY, posZ;
};
struct sc_packet_put_object {
	unsigned char size;
	char type;
	int id;
	float x, y, z;
	char object_type;
	char name[MAX_NAME_SIZE];
};
struct sc_packet_remove_object {
	unsigned char size;
	char type;
	int id;
};
struct sc_packet_chat {
	unsigned char size;
	char type;
	int id;
	char message[MAX_CHAT_SIZE];
};
struct sc_packet_login_fail {
	unsigned char size;
	char type;
	char reason;		// 1: DBerror,  2:����� Full, 3:�̹� ������, 4:���Ʋ��, 5:���̵����, 6:�ش���̵� �̹�����
};
struct sc_packet_time {
	unsigned char size;
	char	type;
	float time;
	int send_time;//���Ŀ� ���־ߵɼ��� ����
};
struct sc_packet_player_attack {
	unsigned char size;
	char type;
	int	obj_id;
	//float f_x, f_y, f_z;
};
struct sc_packet_win {
	unsigned char size;
	char type;
};
struct sc_packet_defeat {
	unsigned char size;
	char type;
};
struct sc_packet_stun {
	unsigned char size;
	char type;
	int	obj_id;
};
struct sc_pakcet_interaction {
};

struct sc_packet_test {
	unsigned char size;
	char	type;
	int		id;
	float	x, y, z;
};