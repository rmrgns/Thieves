#pragma once
#include <chrono>

constexpr short SERVER_PORT = 8471;

constexpr int MAX_NAME_SIZE = 20;
constexpr int MAX_PASSWORD_SIZE = 20;
constexpr int MAX_CHAT_SIZE = 100;

constexpr int MAX_ROOM_SIZE = 8;

constexpr int MAX_USER = MAX_ROOM_SIZE * 8;
constexpr int MAX_ITEM = 20;
constexpr int USER_NUM = 8;
constexpr float FOV_RANGE = 0.f;

constexpr int MAX_NPC = MAX_ROOM_SIZE * 8;

constexpr int NPC_ID_END = MAX_USER + MAX_NPC - 1;
constexpr int BASE_ID = NPC_ID_END + 2;
constexpr int PLAYER_HP = 5;
constexpr int PLAYER_DAMAGE = 1;

// ERROR CODE

constexpr int ERROR_GAME_IN_PROGRESS = 1;
constexpr int ERROR_ROOM_IS_FULL = 2;
constexpr int ERROR_ROOM_NOT_EXIST = 3;
constexpr int ERROR_PLAYER_NOT_READY = 4;

// ITEM CODE

constexpr int ITEM_NUM_DIAMOND = 0;
constexpr int ITEM_NUM_GUN = 1;
constexpr int ITEM_NUM_TRAP = 2;
constexpr int ITEM_NUM_MAP = 3;

//---------
// CS
constexpr char CS_PACKET_SIGN_IN = 1;			//
constexpr char CS_PACKET_SIGN_UP = 2;			//
constexpr char CS_PACKET_MOVE = 3;				//
constexpr char CS_PACKET_ATTACK = 4;			//
constexpr char CS_PACKET_CHAT = 5;				//
constexpr char CS_PACKET_HIT = 6;				//
constexpr char CS_PACKET_GAME_START = 7;		//
constexpr char CS_PACKET_MATCHING = 8;			//
constexpr char CS_PACKET_TEST = 9;				//
constexpr char CS_PACKET_START_JUMP = 10;				//
constexpr char CS_PACKET_COMPLETE_JUMP = 11;				//
constexpr char CS_PACKET_LOAD_PROGRESSING = 12;	//
constexpr char CS_PACKET_LOAD_END = 13;			//
constexpr char CS_PACKET_ENTER_ROOM = 14;
constexpr char CS_PACKET_LEAVE_ROOM = 15;
constexpr char CS_PACKET_PLAYER_READY = 16;
constexpr char CS_PACKET_PLAYER_CANCLE_READY = 17;
constexpr char CS_PACKET_PLAYER_LOG_OUT = 18;	//
constexpr char CS_PACKET_REQUEST_ROOMS_DATA_FOR_LOBBY = 19; //
constexpr char CS_PACKET_REQUEST_ROOMS_DATA_FOR_ROOM = 20; //

constexpr char CS_PACKET_STEEL_DIAMOND = 21; //
constexpr char CS_PACKET_GET_ITEM = 22; //
constexpr char CS_PACKET_USE_ITEM = 23; //
constexpr char CS_PACKET_BULLET = 24;	//

constexpr char CS_PACKET_ATTACKMODE = 99;		//





// SC
constexpr char SC_PACKET_SIGN_IN_OK = 1;		//
constexpr char SC_PACKET_SIGN_UP_OK = 2;		//
constexpr char SC_PACKET_MOVE = 3;				//
constexpr char SC_PACKET_PUT_OBJECT = 4;		//
constexpr char SC_PACKET_REMOVE_OBJECT = 5;		//
constexpr char SC_PACKET_CHAT = 6;				//
constexpr char SC_PACKET_LOGIN_FAIL = 7;		//
constexpr char SC_PACKET_TIME = 8;
constexpr char SC_PACKET_POLICE_ATTACK = 9;		//
constexpr char SC_PACKET_PLAYER_ATTACK = 10;	//
constexpr char SC_PACKET_WIN = 11;				// win
constexpr char SC_PACKET_DEFEAT = 12;			// defeat
constexpr char SC_PACKET_STUN = 13;				// stun
constexpr char SC_PACKET_INTERACTION = 14;		//
constexpr char SC_PACKET_PHASE = 15;			//
constexpr char SC_PACKET_TEST = 16;
constexpr char SC_PACKET_OBJ_INFO = 17;		//
constexpr char SC_PACKET_START_JUMP = 18;			//
constexpr char SC_PACKET_COMPLETE_JUMP = 19;			// 
constexpr char SC_PACKET_LOAD_PROGRESS_PERCENT = 20;	// 
constexpr char SC_PACKET_LOAD_END = 21; //
constexpr char SC_PACKET_ENTER_ROOM = 22; //
constexpr char SC_PACKET_ENTER_ROOM_OK = 23;
constexpr char SC_PACKET_LEAVE_ROOM = 24; //
constexpr char SC_PACKET_PLAYER_READY = 25; //
constexpr char SC_PACKET_PLAYER_CANCLE_READY = 26; //
constexpr char SC_PACKET_LOG_OUT_OK = 27; //
constexpr char SC_PACKET_ALL_PLAYER_LOAD_END = 28; //
constexpr char SC_PACKET_ROOMS_DATA_FOR_LOBBY = 29; //
constexpr char SC_PACKET_ROOMS_DATA_FOR_LOBBY_END = 30; //
constexpr char SC_PACKET_ROOMS_DATA_FOR_ROOM = 31; // 
constexpr char SC_PACKET_ROOMS_DATA_FOR_ROOM_END = 32; //
constexpr char SC_PACKET_ERROR = 33; //
constexpr char SC_PACKET_GAME_START = 34; //
constexpr char SC_PACKET_OBJ_INFO_END = 35; //
//constexpr char SC_PACKET_PHASE_CHANGE = 36; //
constexpr char SC_PACKET_ITEM_USE = 36; //
constexpr char SC_PACKET_STUN_END = 37; //
constexpr char SC_PACKET_ATTACK = 38; //
constexpr char SC_PACKET_TIMER_START = 39;
constexpr char SC_PACKET_BULLET = 40;		//
constexpr char SC_PACKET_ITEM_INFO = 41;
constexpr char SC_PACKET_ACTIVE_ESCAPE = 42;
constexpr char SC_PACKET_ACTIVE_SPECIAL_ESCAPE = 43;
constexpr char SC_PACKET_GAME_TIMER_START = 44;
constexpr char SC_PACKET_OPEN_SAFE = 45;
constexpr char SC_PACKET_INVINCIBLE_END = 46;
constexpr char SC_PACKET_INVINCIBLE = 47;
constexpr char SC_PACKET_GET_ITEM = 48;
constexpr char SC_PACKET_DIAMOND_OWNER_CHANGE = 49;
constexpr char SC_PACKET_GAME_END = 50;


constexpr char SC_PACKET_ATTACKMODE = 99;		//
constexpr char SC_PACKET_NPC_ATTACK = 100;




//#pragma pack (push, 1)

//
//
struct cs_packet_move {
	unsigned char size;
	char	type;
	char	direction;			//
	int		move_time; //
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

struct cs_packet_bullet {
	unsigned char size;
	char type;
	float p_x, p_y, p_z;	//
	float d_x, d_y, d_z;	//
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
	char reason;
};
struct sc_packet_time {
	unsigned char size;
	char	type;
	float time;
	int send_time;
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
	int attack_player;
	int	stun_player;
	bool stun_by_item;
};
struct sc_packet_interaction {
	unsigned char size;
	char type;
	bool interaction_on;
};

struct sc_packet_phase_change {
	unsigned char size;
	char type;
	int diamond_player;
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
	int master_id;
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
	int master_id;
};

struct sc_packet_error {
	unsigned char size;
	char type;
	char err_type;
	char err_val;
};

struct sc_packet_game_start {
	unsigned char size;
	char type;
};

struct sc_packet_obj_info_end {
	unsigned char size;
	char type;
};

struct sc_packet_bullet {
	unsigned char size;
	char type;
	float p_x, p_y, p_z;
	//	float d_x, d_y, d_z;	
};

struct sc_packet_attackmode {
	unsigned char size;
	char type;
	char mode;
};

struct sc_packet_npc_attack {
	unsigned char size;
	char type;
	int obj_id;
	int target_id;
};

struct sc_packet_item_use {
	unsigned char size;
	char type;
	int obj_id;
	int player;
	float x, y, z;
	char item_type;
};

struct sc_packet_stun_end {
	unsigned char size;
	char type;
	int obj_id;
};

struct sc_packet_attack {
	unsigned char size;
	char type;
	int player;
};

struct sc_packet_get_item {
	unsigned char size;
	char type;
	int obj_id;
	int player;
};

struct sc_packet_item_info {
	unsigned char size;
	char	type;
	int		id;
	float	x, y, z;
	char item_type;
};

struct sc_packet_active_escape {
	unsigned char size;
	char type;
	float x[3];
	float y[3];
	float z[3];
};

struct sc_packet_active_special_escape {
	unsigned char size;
	char type;
	float x, y, z;
};

struct sc_packet_timer_start {
	unsigned char size;
	char type;
	std::chrono::system_clock::time_point start_time;
};

struct sc_packet_open_safe {
	unsigned char size;
	char type;
};

struct sc_packet_invincible {
	unsigned char size;
	char type;
	int player;
};

struct sc_packet_invincible_end {
	unsigned char size;
	char type;
	int player;
};

struct sc_packet_game_timer_start {
	unsigned char size;
	char type;
};

struct sc_packet_diamond_owner_change {
	unsigned char size;
	char type;
	int new_owner;
};

struct sc_packet_game_end {
	unsigned char size;
	char type;
};