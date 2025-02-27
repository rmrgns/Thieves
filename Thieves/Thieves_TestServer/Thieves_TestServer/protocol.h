#pragma once

constexpr short SERVER_PORT = 8471;

// 맵 크기

constexpr int MAX_NAME_SIZE = 20;		// 아이디 사이즈
constexpr int MAX_PASSWORD_SIZE = 20;	// 비밀 번호 사이즈
constexpr int MAX_CHAT_SIZE = 100;		// 채팅 사이즈

constexpr int MAX_ROOM_SIZE = 8;		// 방 최대 크기

constexpr int MAX_USER = MAX_ROOM_SIZE * 8;		// 동접 가능 인원

constexpr int USER_NUM = 8;
// 수정
constexpr float FOV_RANGE = 0.f;

constexpr int MAX_NPC = 7;

// 경찰 NPC의 갯수
// 도둑 AI의 갯수
constexpr int PLAYER_HP = 5;		// 플레이어 HP
constexpr int PLAYER_DAMAGE = 1;	// 플레이어 DMG
// 플레이어 이동거리
// 플레이어 공격 사거리
// ID

// CS
constexpr char CS_PACKET_SIGN_IN = 1;			// 로그인
constexpr char CS_PACKET_SIGN_UP = 2;			// 가입
constexpr char CS_PACKET_MOVE = 3;				// 이동
constexpr char CS_PACKET_ATTACK = 4;			// 공격
constexpr char CS_PACKET_CHAT = 5;				// 채팅
constexpr char CS_PACKET_HIT = 6;				// 피격
constexpr char CS_PACKET_GAME_START = 7;		// 게임 시작
constexpr char CS_PACKET_MATCHING = 8;
constexpr char CS_PACKET_TEST = 9;
constexpr char CS_PACKET_START_JUMP = 10;				// 점프 시작 패킷
constexpr char CS_PACKET_COMPLETE_JUMP = 11;				// 점프 완료 패킷
constexpr char CS_PACKET_LOAD_PROGRESSING = 12;
constexpr char CS_PACKET_LOAD_END = 13;



// SC
constexpr char SC_PACKET_SIGN_IN_OK = 1;		// 로그인 OK
constexpr char SC_PACKET_SIGN_UP_OK = 2;		// 가입 OK
constexpr char SC_PACKET_MOVE = 3;				// 이동
constexpr char SC_PACKET_PUT_OBJECT = 4;		// 플레이어 생성
constexpr char SC_PACKET_REMOVE_OBJECT = 5;		// 플레이어 제거
constexpr char SC_PACKET_CHAT = 6;				// 채팅
constexpr char SC_PACKET_LOGIN_FAIL = 7;		// 로그인 실패
constexpr char SC_PACKET_TIME = 8;
constexpr char SC_PACKET_POLICE_ATTACK = 9;		// 경찰 어택
constexpr char SC_PACKET_PLAYER_ATTACK = 10;	// 플레이어 어택
constexpr char SC_PACKET_WIN = 11;				// win
constexpr char SC_PACKET_DEFEAT = 12;			// defeat
constexpr char SC_PACKET_STUN = 13;				// stun
constexpr char SC_PAKCET_INTERACTION = 14;		// 상호작용
constexpr char SC_PACKET_PHASE = 15;			// 페이즈 변경  
constexpr char SC_PACKET_TEST = 16;
constexpr char SC_PACKET_OBJ_INFO = 17;		// OBJ 정보
constexpr char SC_PACKET_START_JUMP = 18;			// 점프 시작 패킷
constexpr char SC_PACKET_COMPLETE_JUMP = 19;			// 점프 완료 패킷
constexpr char SC_PACKET_LOAD_PROGRESS_PERCENT = 20;
constexpr char SC_PACKET_LOAD_END = 21;
//#pragma pack (push, 1)

// 클라이언트 -> 서버로 보내는 패킷은 어떤 키를 얼마나 눌렀는지에 대해서만 보내주면 된다.
// 어차피 서버에서 위치를 계산하여 클라이언트로 보내줄 것이기 때문에 문제가 없다.
struct cs_packet_move {
	unsigned char size;
	char	type;
	char	direction;			// 1 : 앞,  2: 뒤, 3:왼, 4:오
	int		move_time; //디버그 용 -> 보낸시간 -받은시간 = 통신하는 시간
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

// 서버 -> 클라의 경우 포지션과 룩 벡터 모두 보내줘야 하기는 한다.
// 자기 자신의 경우는 그냥 룩 벡터에 대해서는 적용시키지 않도록 하기만 하면 된다.
// 이미 id 데이터를 가지고 있기 때문에 이 부분은 문제가 없을 것이다.

// -> 정정함. 단순하게 이 패킷이 자신과의 연결만 하는 데이터 였다면 그렇게 해야 하겠지만,
// 모든 플레이어에 대한 패킷이라고 한다면 룩 벡터도 부여해 주어야 한다.

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
	char reason;		// 1: DBerror,  2:사용자 Full, 3:이미 접속중, 4:비번틀림, 5:아이디없음, 6:해당아이디 이미존재
};
struct sc_packet_time {
	unsigned char size;
	char	type;
	float time;
	int send_time;//추후에 없애야될수도 있음
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