#pragma once

#include "server/main/send_manager.h"

class ThievesSendManager :
    public SendManager
{
public:
    ThievesSendManager() = default;
    virtual ~ThievesSendManager() = default;
    virtual void SendStartPacket(const SOCKET& s_socket)override;
    virtual void SendSignInPacket(const SOCKET& s_socket);
    virtual void SendMovePacket(const SOCKET& s_socket, char direction,
        Vec3 pos, Vec3 vec, float deltatime, char );
    void SendLoadProgressPacket(const SOCKET& s_socket, char progressed);
    void SendLoadEnd(const SOCKET& s_socket);
    void SendAttackPacket(const SOCKET& s_socket, const Vec3& forward_vec, char action_type);
    void SendEnterRoom(const SOCKET& s_socket, int room_id);
    void SendLeaveRoom(const SOCKET& s_socket);
    void SendReady(const SOCKET& s_socket);
    void SendCancleReady(const SOCKET& s_socket);
    void SendLogOut(const SOCKET& s_socket);
    void SendRequestRoomsData(const SOCKET& s_socket);
    void SendRequestInRoomData(const SOCKET& s_socket);
    void SendBullet(const SOCKET& s_socket, Vec3 , Vec3, Vec3);

    void SendAattackMode(const SOCKET& s_socket, char mode );

    //void SendSignInPacket(const SOCKET& s_socket, char*, char*);
    //void SendSignUPPacket(const SOCKET& s_socket, char*, char*);
    //void SendMatchingPacket(const SOCKET& s_socket, int user_num);
    //void SendMovePacket(const SOCKET& s_socket, client_fw::Vec3& pos, client_fw::Quaternion& rot);
    //void SendHitPacket(const SOCKET& s_socket, int obj_id, int attacker_id);
    //virtual void ProcessSend(const SOCKET& s_socket, const client_fw::SPtr<MessageEventInfo>& message)override;
    //void SendDamageCheatPacket(const SOCKET& s_socket);

    

};
