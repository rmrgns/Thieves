#pragma once

//enum class 선언
//// 각 상태에 따른 상태

// 캐릭터 상태
enum class COMP_OP {
	OP_RECV,
	OP_SEND,
	OP_ACCEPT,
//	OP_POLICE_SPAWN,
//	OP_POLICE_MOVE,
//	OP_POLICE_ATTACK,
//	OP_PLAYER_MOVE,
//	OP_PLAYER_ATTACK
	
};

// game state
enum class STATE {
	ST_FREE,
	ST_ACCEPT,
	ST_LOGIN,
	ST_INGAME
};

enum class ROOM_STATE
{
	RT_FREE,
	RT_INGAME,
	RT_RESET
};

// Room state
enum class ROOM_TYPE {
	RT_FREE,
	RE_INGAME,
	RT_RESET
};

// Object type
enum class OBJ_TYPE
{
	OT_MAPOBJ,
	OT_SPAWN_AREA,
	OT_ACTIViTY_AREA,
	OT_BASE,
	OT_PLAYER,
	OT_POLICE
};

enum class ENEMY_STATE {

};

//enum class OBJ_TYPE {
//	OT_MAPOBJ,
//	OT_SPAWN_AREA,
//
//};

// Login type
enum class LOGINFAIL_TYPE {
	OK,
	DB_ERROR,
	FULL,
	AREADY_SIGHN_IN,
	WRONG_PASSWORD,
	NO_ID
};

// DB
enum class DB_TASK_TYPE {
	SIGN_IN,
	SIGN_UP
};

// Player action 
enum class PL_ACTION_TYPE {
	NONE,
	MOVE,
	JUMP,
	ATTACK
};