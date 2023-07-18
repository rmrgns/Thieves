#pragma once

constexpr short SERVER_PORT = 8471;

// �� ũ��

constexpr int MAX_NAME_SIZE = 20;		// ���̵� ������
constexpr int MAX_PASSWORD_SIZE = 20;	// ��� ��ȣ ������
constexpr int MAX_CHAT_SIZE = 100;		// ä�� ������

constexpr int MAX_ROOM_SIZE = 8;		// �� �ִ� ũ��

constexpr int MAX_USER = MAX_ROOM_SIZE * 8;		// ���� ���� �ο�

constexpr int USER_NUM = 8;
// ����
constexpr float FOV_RANGE = 0.f;

constexpr int MAX_NPC = 7;

// ���� NPC�� ����
// ���� AI�� ����
constexpr int PLAYER_HP = 5;		// �÷��̾� HP
constexpr int PLAYER_DAMAGE = 1;	// �÷��̾� DMG
// �÷��̾� �̵��Ÿ�
// �÷��̾� ���� ��Ÿ�
// ID

// CS
constexpr char CS_PACKET_SIGN_IN = 1;			// �α���
constexpr char CS_PACKET_SIGN_UP = 2;			// ����
constexpr char CS_PACKET_MOVE = 3;				// �̵�
constexpr char CS_PACKET_ATTACK = 4;			// ����
constexpr char CS_PACKET_CHAT = 5;				// ä��
constexpr char CS_PACKET_HIT = 6;				// �ǰ�
constexpr char CS_PACKET_GAME_START = 7;		// ���� ����
constexpr char CS_PACKET_MATCHING = 8;
constexpr char CS_PACKET_TEST = 9;
constexpr char CS_PACKET_START_JUMP = 10;				// ���� ���� ��Ŷ
constexpr char CS_PACKET_COMPLETE_JUMP = 11;				// ���� �Ϸ� ��Ŷ
constexpr char CS_PACKET_LOAD_PROGRESSING = 12;
constexpr char CS_PACKET_LOAD_END = 13;



// SC
constexpr char SC_PACKET_SIGN_IN_OK = 1;		// �α��� OK
constexpr char SC_PACKET_SIGN_UP_OK = 2;		// ���� OK
constexpr char SC_PACKET_MOVE = 3;				// �̵�
constexpr char SC_PACKET_PUT_OBJECT = 4;		// �÷��̾� ����
constexpr char SC_PACKET_REMOVE_OBJECT = 5;		// �÷��̾� ����
constexpr char SC_PACKET_CHAT = 6;				// ä��
constexpr char SC_PACKET_LOGIN_FAIL = 7;		// �α��� ����
constexpr char SC_PACKET_TIME = 8;
constexpr char SC_PACKET_POLICE_ATTACK = 9;		// ���� ����
constexpr char SC_PACKET_PLAYER_ATTACK = 10;	// �÷��̾� ����
constexpr char SC_PACKET_WIN = 11;				// win
constexpr char SC_PACKET_DEFEAT = 12;			// defeat
constexpr char SC_PACKET_STUN = 13;				// stun
constexpr char SC_PAKCET_INTERACTION = 14;		// ��ȣ�ۿ�
constexpr char SC_PACKET_PHASE = 15;			// ������ ����  
constexpr char SC_PACKET_TEST = 16;
constexpr char SC_PACKET_OBJ_INFO = 17;		// OBJ ����
constexpr char SC_PACKET_START_JUMP = 18;			// ���� ���� ��Ŷ
constexpr char SC_PACKET_COMPLETE_JUMP = 19;			// ���� �Ϸ� ��Ŷ
constexpr char SC_PACKET_LOAD_PROGRESS_PERCENT = 20;
constexpr char SC_PACKET_LOAD_END = 21;
//#pragma pack (push, 1)

// Ŭ���̾�Ʈ -> ������ ������ ��Ŷ�� � Ű�� �󸶳� ���������� ���ؼ��� �����ָ� �ȴ�.
// ������ �������� ��ġ�� ����Ͽ� Ŭ���̾�Ʈ�� ������ ���̱� ������ ������ ����.
struct cs_packet_move {
	unsigned char size;
	char	type;
	char	direction;			// 1 : ��,  2: ��, 3:��, 4:��
	int		move_time; //����� �� -> �����ð� -�����ð� = ����ϴ� �ð�
	float	vecX, vecZ;	// look vec
	float	deltaTime;	//delta time
	char	action_type; // action type
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

struct cs_packet_load_progressing {
	unsigned char size;
	char type;
	char progressed;
};

struct cs_packet_load_end {
	unsigned char size;
	char type;
};

struct cs_packet_bullet {
	unsigned char size;
	char type;
	float x, y, z;
	float d_x, d_y, x_z;
};


//------------------------------------------------------------------



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

// ���� -> Ŭ���� ��� �����ǰ� �� ���� ��� ������� �ϱ�� �Ѵ�.
// �ڱ� �ڽ��� ���� �׳� �� ���Ϳ� ���ؼ��� �����Ű�� �ʵ��� �ϱ⸸ �ϸ� �ȴ�.
// �̹� id �����͸� ������ �ֱ� ������ �� �κ��� ������ ���� ���̴�.

// -> ������. �ܼ��ϰ� �� ��Ŷ�� �ڽŰ��� ���Ḹ �ϴ� ������ ���ٸ� �׷��� �ؾ� �ϰ�����,
// ��� �÷��̾ ���� ��Ŷ�̶�� �Ѵٸ� �� ���͵� �ο��� �־�� �Ѵ�.

struct sc_packet_move {
	unsigned char size;
	char	type;
	int		id;
	bool	recv_bool;
	float	posX, posY, posZ;
	float   rotX, rotZ;
	char	action_type; // action type
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
struct sc_packet_interaction {
};

struct sc_packet_test {
	unsigned char size;
	char	type;
	int		id;
	float	x, y, z;
};

struct sc_packet_obj_info {
	unsigned char size;
	char	type;
	int		id;
	float	x, y, z;
	bool	start;
	char	object_type;
};

struct sc_packet_load_progress_percent {
	unsigned char size;
	char type;
	int id;
	char percent;
};

struct sc_packet_load_end {
	unsigned char size;
	char type;
	int id;
};

struct sc_packet_bullet {
	unsigned char size;
	char type;
	
	float x, y, z;
	int id;
};