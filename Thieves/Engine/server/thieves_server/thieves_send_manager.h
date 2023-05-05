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
        Vec3 pos, Vec3 vec, float deltatime, char )override;
    void SendAttackPacket(const SOCKET& s_socket, const Vec3& forward_vec, char);


    //void SendSignInPacket(const SOCKET& s_socket, char*, char*);
    //void SendSignUPPacket(const SOCKET& s_socket, char*, char*);
    //void SendMatchingPacket(const SOCKET& s_socket, int user_num);
    //void SendMovePacket(const SOCKET& s_socket, client_fw::Vec3& pos, client_fw::Quaternion& rot);
    //void SendHitPacket(const SOCKET& s_socket, int obj_id, int attacker_id);
    //virtual void ProcessSend(const SOCKET& s_socket, const client_fw::SPtr<MessageEventInfo>& message)override;
    //void SendDamageCheatPacket(const SOCKET& s_socket);

    

};
