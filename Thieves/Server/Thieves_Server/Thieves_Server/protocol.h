#pragma once

const short SERVER_PORT = 7777;

// 맵 크기

const int MAX_NAME_SIZE = 20;		// 아이디 사이즈
const int MAX_PASSWORD_SIZE = 20;	// 비밀 번호 사이즈
const int MAX_CHAT_SIZE = 100;		// 채팅 사이즈

const int MAX_ROOM_SIZE = 1000;		// 방 최대 크기

const int MAX_USER = MAX_ROOM_SIZE * 8;		// 동접 가능 인원


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