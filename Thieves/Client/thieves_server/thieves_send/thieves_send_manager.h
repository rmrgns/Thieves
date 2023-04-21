#pragma once

#include "server/send_manager/send_manager.h"

class ThievesSendManager :
    public SendManager
{
public:
    ThievesSendManager() = default;
    virtual ~ThievesSendManager() = default;
   // virtual void ProcessSend(const SOCKET& s_socket, const client_fw::SPtr<MessageEventInfo>& message)override;
//    void SendTestPacket(const SOCKET& s_socket);
    //virtual void SendMovePacket(const SOCKET& s_socket, char direction,
    //        float f_x, float f_y, float f_z,
    //        float r_x, float r_y, float r_z,
    //        float deltatime)override;
    virtual void SendMovePacket(const SOCKET& s_socket, char direction,
        Vec3 pos, Vec3 vec, float deltatime)override;

    //void SendSignInPacket(const SOCKET& s_socket, char*, char*);
    //void SendSignUPPacket(const SOCKET& s_socket, char*, char*);
    //void SendMatchingPacket(const SOCKET& s_socket, int user_num);
    //void SendMovePacket(const SOCKET& s_socket, client_fw::Vec3& pos, client_fw::Quaternion& rot);
    //void SendAttackPacket(const SOCKET& s_socket, const client_fw::Vec3& forward_vec);
    //void SendHitPacket(const SOCKET& s_socket, int obj_id, int attacker_id);
    virtual void SendStartPacket(const SOCKET& s_socket)override;

    //void SendDamageCheatPacket(const SOCKET& s_socket);

    

};
