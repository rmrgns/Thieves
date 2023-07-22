#pragma once

constexpr short SERVER_PORT = 8471;

// 맵 크기

constexpr int MAX_NAME_SIZE = 20;		// 아이디 사이즈
constexpr int MAX_PASSWORD_SIZE = 20;	// 비밀 번호 사이즈
constexpr int MAX_CHAT_SIZE = 100;		// 채팅 사이즈

constexpr int MAX_ROOM_SIZE = 8;		// 방 최대 개수

constexpr int MAX_USER = MAX_ROOM_SIZE * 8;		// 동접 가능 인원

// 룸 하나의 최대 인원
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

//--------- 여기는 서버에서 클라이언트에게 에러 코드를 전달해 줄때 어떤 에러인지 알려주는 용도
constexpr int ERROR_GAME_IN_PROGRESS = 1;		// 게임이 진행중인 방에 들어가려고 했을 때
constexpr int ERROR_ROOM_IS_FULL = 2;			// 룸 안에 사람이 가득 차있는 경우
constexpr int ERROR_ROOM_NOT_EXIST = 3;			// 룸이 없음
constexpr int ERROR_PLAYER_NOT_READY = 4;

//---------

constexpr int ITEM_NUM_DIAMOND = 0;
constexpr int ITEM_NUM_GUN = 1;
constexpr int ITEM_NUM_TRAP = 2;
constexpr int ITEM_NUM_MAP = 3;

//---------
// CS
constexpr char CS_PACKET_SIGN_IN = 1;			// 로그인
constexpr char CS_PACKET_SIGN_UP = 2;			// 가입
constexpr char CS_PACKET_MOVE = 3;				// 이동
constexpr char CS_PACKET_ATTACK = 4;			// 공격
constexpr char CS_PACKET_CHAT = 5;				// 채팅
constexpr char CS_PACKET_HIT = 6;				// 피격
constexpr char CS_PACKET_GAME_START = 7;		// 게임 시작
constexpr char CS_PACKET_MATCHING = 8;			// 매칭 -> 현재는 사용 안하기로
constexpr char CS_PACKET_TEST = 9;				// 패킷 확인용
constexpr char CS_PACKET_START_JUMP = 10;				// 점프 시작 패킷
constexpr char CS_PACKET_COMPLETE_JUMP = 11;				// 점프 완료 패킷
constexpr char CS_PACKET_LOAD_PROGRESSING = 12;	// 로딩 중 진행 상황 패킷
constexpr char CS_PACKET_LOAD_END = 13;			// 로딩 완료 패킷
constexpr char CS_PACKET_ENTER_ROOM = 14;
constexpr char CS_PACKET_LEAVE_ROOM = 15;
constexpr char CS_PACKET_PLAYER_READY = 16;
constexpr char CS_PACKET_PLAYER_CANCLE_READY = 17;
constexpr char CS_PACKET_PLAYER_LOG_OUT = 18;	// 로그아웃
constexpr char CS_PACKET_REQUEST_ROOMS_DATA_FOR_LOBBY = 19; // 로비에서 보여줄 데이터를 받기
constexpr char CS_PACKET_REQUEST_ROOMS_DATA_FOR_ROOM = 20; // 룸에서 보여줄 데이터를 받기
constexpr char CS_PACKET_STEEL_DIAMOND = 21; // 다이아를 훔쳤음
constexpr char CS_PACKET_GET_ITEM = 22; // 아이템 획득
constexpr char CS_PACKET_USE_ITEM = 23; // 아이템 사용


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
constexpr char SC_PACKET_LOAD_PROGRESS_PERCENT = 20;	// 게임 로딩중 진행도 패킷
constexpr char SC_PACKET_LOAD_END = 21; // 게임 로딩 완료 패킷. 플레이어 하나에 대한 패킷이므로, 모두 끝난 경우는 ALL_PLAYER_LOAD_END로 해주어야 한다
constexpr char SC_PACKET_ENTER_ROOM = 22; // 클라이언트가 룸으로 들어갈 때 줄 패킷
constexpr char SC_PACKET_ENTER_ROOM_OK = 23;
constexpr char SC_PACKET_LEAVE_ROOM = 24; // 클라이언트가 룸에서 나갈 때 줄 패킷
constexpr char SC_PACKET_PLAYER_READY = 25; // 클라이언트가 룸 안에서 레디를 했을 때 줄 패킷
constexpr char SC_PACKET_PLAYER_CANCLE_READY = 26; // 클라이언트가 룸 안에서 레디를 취소 했을 때 패킷
constexpr char SC_PACKET_LOG_OUT_OK = 27; // 클라이언트가 로그아웃 했을 때 줄 패킷
constexpr char SC_PACKET_ALL_PLAYER_LOAD_END = 28; // 모든 플레이어가 로딩이 끝났을 때 줄 패킷
constexpr char SC_PACKET_ROOMS_DATA_FOR_LOBBY = 29; // 플레이어가 로비에 들어 갔을 때 룸 들에 대한 정보를 줄 패킷
constexpr char SC_PACKET_ROOMS_DATA_FOR_LOBBY_END = 30; // 위의 패킷에서 룸당 하나씩 보내줄 것이므로 끝났음을 표시해 주어야 함. (WSASend가 많이 불리므로 나중에 고쳐야 할 수도 잇다.)
constexpr char SC_PACKET_ROOMS_DATA_FOR_ROOM = 31; // 플레이어가 룸에 들어 갔을 때 룸 내부의 정보를 줄 패킷
constexpr char SC_PACKET_ROOMS_DATA_FOR_ROOM_END = 32; // LOBBY_END와 비슷하게 끝났음을 알리는 패킷. (이것도 마찬가지로 WSASend가 많이 불리게 되므로 고쳐야 할 수도 잇음)
constexpr char SC_PACKET_ERROR = 33; // 어떤 식으로든 에러를 보내야 할때
constexpr char SC_PACKET_GAME_START = 34; // 아니 게임 시작 패킷이 없는게 말이 됨?
constexpr char SC_PACKET_OBJ_INFO_END = 35; // OBJ_INFO도 시작하기 전에 보내야 하는게 맞는듯.
//constexpr char SC_PACKET_PHASE_CHANGE = 36; // 페이즈가 바뀌었다는 걸 모두에게도 알려야 함 --> 페이즈 패킷으로 어떻게 하면 될듯
constexpr char SC_PACKET_ITEM_USE = 36; // 아이템이 사용 되었다는 걸 모두에게도 알려야 함. 물론, 이건 아이템에 맞았다 같은 것은 HIT로 같이 보내주면 될듯
constexpr char SC_PACKET_STUN_END = 37; // 스턴이 끝나는 것도 알려 줘야지 이것아
constexpr char SC_PACKET_ATTACK = 38; // 아니 공격했다는걸 알려줘야 하니까 당연히 있어야지
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

struct cs_packet_enter_room {
	unsigned char size;
	char type;
	int room_id;
};

struct cs_packet_leave_room {
	unsigned char size;
	char type;
};

struct cs_packet_player_ready {
	unsigned char size;
	char type;
};

struct cs_packet_player_cancle_ready {
	unsigned char size;
	char type;
};

struct cs_packet_player_log_out {
	unsigned char size;
	char type;
};

struct cs_packet_request_rooms_data_for_lobby {
	unsigned char size;
	char type;
};

struct cs_packet_request_rooms_data_for_room {
	unsigned char size;
	char type;
};

struct cs_packet_steel_diamond {
	unsigned char size;
	char type;
};

struct cs_packet_get_item {
	unsigned char size;
	char type;
	char itemNum;
};

struct cs_packet_use_item {
	unsigned char size;
	char type;
	char itemNum;
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

struct sc_packet_phase_change {
	unsigned char size;
	char type;
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

struct sc_packet_enter_room {
	unsigned char size;
	char type;
	int id;
	char userName[MAX_NAME_SIZE];
};

struct sc_packet_enter_room_ok {
	unsigned char size;
	char type;
	int room_id;
};

struct sc_packet_leave_room {
	unsigned char size;
	char type;
	int id;
	int master_id; // 룸 마스터가 변경될 때에 사용
};

struct sc_packet_player_ready {
	unsigned char size;
	char type;
	int id;
};

struct sc_packet_player_cancle_ready {
	unsigned char size;
	char type;
	int id;
};

struct sc_packet_log_out {
	unsigned char size;
	char type;
	int id;
};

struct sc_packet_all_player_load_end {
	unsigned char size;
	char type;
};

struct sc_packet_rooms_data_for_lobby {
	unsigned char size;
	char type;
	int roomID;
	char playerNum;
	bool isInGame;
};

struct sc_packet_rooms_data_for_lobby_end {
	unsigned char size;
	char type;
};

struct sc_packet_rooms_data_for_room {
	unsigned char size;
	char type;
	int userId;
	char userName[MAX_NAME_SIZE];
	bool isReady;

};

struct sc_packet_rooms_data_for_room_end {
	unsigned char size;
	char type;
	int master_id; // 룸 마스터의 번호 전송
};

struct sc_packet_error {
	unsigned char size;
	char type;
	char err_type;
	char err_val; // 에러에 특정 값이 필요한 경우
};

struct sc_packet_game_start {
	unsigned char size;
	char type;
};

struct sc_packet_obj_info_end {
	unsigned char size;
	char type;
};

struct sc_packet_item_use {
	unsigned char size;
	char type;
};

struct sc_packet_stun_end {
	unsigned char size;
	char type;
};

struct sc_packet_attack {
	unsigned char size;
	char type;
	int player;
};

struct sc_packet_get_item {
	unsigned char size;
	char type;
	int itemNum;
};