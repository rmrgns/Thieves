#ifndef BIND_PACKET
#define BIND_PACKET(PKT_ID, PKT_TYPE, HANDLER_FUNC)
#endif

BIND_PACKET(CS_PACKET_SIGN_IN, cs_packet_sign_in, ProcessSignIn);
BIND_PACKET(CS_PACKET_SIGN_UP, cs_packet_sign_up, ProcessSignUp);
BIND_PACKET(CS_PACKET_MOVE, cs_packet_move, ProcessMove);
BIND_PACKET(CS_PACKET_ATTACK, cs_packet_attack, ProcessAttack);
BIND_PACKET(CS_PACKET_HIT, cs_packet_hit, ProcessHit);
BIND_PACKET(CS_PACKET_GAME_START, cs_packet_game_start, ProcessGameStart);
BIND_PACKET(CS_PACKET_LOAD_PROGRESSING, cs_packet_load_progressing, ProcessLoadProgressing);
BIND_PACKET(CS_PACKET_LOAD_END, cs_packet_load_end, ProcessLoadEnd);
BIND_PACKET(CS_PACKET_ENTER_ROOM, cs_packet_enter_room, ProcessEnterRoom);
BIND_PACKET(CS_PACKET_LEAVE_ROOM, cs_packet_leave_room, ProcessLeaveRoom);
BIND_PACKET(CS_PACKET_PLAYER_READY, cs_packet_player_ready, ProcessPlayerReady);
BIND_PACKET(CS_PACKET_PLAYER_CANCLE_READY, cs_packet_player_cancle_ready, ProcessPlayerCancleReady);
BIND_PACKET(CS_PACKET_PLAYER_LOG_OUT, cs_packet_player_log_out, ProcessLogOut);
BIND_PACKET(CS_PACKET_REQUEST_ROOMS_DATA_FOR_LOBBY, cs_packet_request_rooms_data_for_lobby, ProcessRoomsDataInLobby);
BIND_PACKET(CS_PACKET_REQUEST_ROOMS_DATA_FOR_ROOM, cs_packet_request_rooms_data_for_room, ProcessRoomsDataInRoom);
BIND_PACKET(CS_PACKET_BULLET, cs_packet_bullet, ProcessBullet);
BIND_PACKET(CS_PACKET_GET_ITEM, cs_packet_get_item, ProcessGetItem);
BIND_PACKET(CS_PACKET_USE_ITEM, cs_packet_use_item, ProcessUseItem);
