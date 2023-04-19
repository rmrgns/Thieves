#pragma once

const short SERVER_PORT = 7777;

// 맵 크기

const int MAX_NAME_SIZE = 20;		// 아이디 사이즈
const int MAX_PASSWORD_SIZE = 20;	// 비밀 번호 사이즈
const int MAX_CHAT_SIZE = 100;		// 채팅 사이즈

const int MAX_ROOM_SIZE = 1000;		// 방 최대 크기

const int MAX_USER = MAX_ROOM_SIZE * 8;		// 동접 가능 인원

// 수정
const float FOV_RANGE = 0.f;

constexpr int MAX_NPC = 7;

// 경찰 NPC의 갯수
// 도둑 AI의 갯수
const int PLAYER_HP = 5;		// 플레이어 HP
const int PLAYER_DAMAGE = 1;	// 플레이어 DMG
// 플레이어 이동거리
// 플레이어 공격 사거리
// ID

// CS
const char CS_PACKET_SIGN_IN = 1;			// 로그인
const char CS_PACKET_SIGN_UP = 2;			// 가입
const char CS_PACKET_MOVE = 3;				// 이동
const char CS_PACKET_ATTACK = 4;			// 공격
const char CS_PACKET_CHAT = 5;				// 채팅
const char CS_PACKET_HIT = 6;				// 피격
const char CS_PACKET_GAME_START = 7;		// 게임 시작
const char CS_PACKET_MATCHING = 8;
const char CS_PACKET_TEST = 9;
// SC
const char SC_PACKET_SIGN_IN_OK = 1;		// 로그인 OK
const char SC_PACKET_SIGN_UP_OK = 2;		// 가입 OK
const char SC_PACKET_MOVE = 3;				// 이동
const char SC_PACKET_PUT_OBJECT = 4;		// 플레이어 생성
const char SC_PACKET_REMOVE_OBJECT = 5;		// 플레이어 제거
const char SC_PACKET_CHAT = 6;				// 채팅
const char SC_PACKET_LOGIN_FAIL = 7;		// 로그인 실패
const char SC_PACKET_TIME = 8;
const char SC_PACKET_POLICE_ATTACK = 9;		// 경찰 어택
const char SC_PACKET_PLAYER_ATTACK = 10;	// 플레이어 어택
const char SC_PACKET_WIN = 11;				// win
const char SC_PACKET_DEFEAT = 12;			// defeat
const char SC_PACKET_STUN = 13;				// stun
const char SC_PAKCET_INTERACTION = 14;		// 상호작용
const char SC_PACKET_PHASE = 15;			// 페이즈 변경  
const char SC_PACKET_TEST = 16;
//#pragma pack (push, 1)

struct cs_packet_move {
	unsigned char size;
	char	type;
	char	direction;			// 1 : 앞,  2: 뒤, 3:왼, 4:오
	int		move_time; //디버그 용 -> 보낸시간 -받은시간 = 통신하는 시간
//	float	s_x, s_y, s_z;
//	float	f_x, f_y, f_z;
//	float	r_x, r_y, r_z;
//	float	l_x, l_y, l_z;
	float	posX, posY, posZ;	// 위치 vec
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
//	char	direction;			// 0 : 앞,  1: 뒤, 2:왼, 3:오
//	int		move_time; //디버그 용 -> 보낸시간 -받은시간 = 통신하는 시간
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
struct sc_pakcet_interaction {
};

struct sc_packet_test {
	unsigned char size;
	char	type;
	int		id;
	float	x, y, z;
};