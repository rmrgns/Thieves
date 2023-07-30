#pragma once
#include "move_object.h"

class Player :
    public MoveObj
{
public:
    Player() :
        m_prev_size(0), m_socket(INVALID_SOCKET)
    {
        m_last_move_time = 0;
        m_type = OBJ_TYPE::OT_PLAYER;
        m_state = STATE::ST_FREE;
        m_room_id = 0;

    }
    virtual ~Player() = default;
    std::mutex state_lock;
    int		m_prev_size;
    std::atomic_bool is_matching = false;


private:

    EXP_OVER m_recv_over;
    SOCKET  m_socket;
    STATE m_state;
    std::atomic_bool m_is_ready = false;
    
    char m_password[MAX_PASSWORD_SIZE + 1];
    short m_mach_user_size = 0;
    
    // 인게임 로딩 중 활용할 데이터
    int m_loadProgressed = 0;

    // 인게임에서 한대 맞았을 때
    std::atomic_bool m_is_attacked = false;
    std::atomic_int m_Item = -1;
    std::atomic_bool m_has_diamond = false;
    std::atomic_bool m_invincible = false;
    std::atomic_bool m_isAbleToOpenSafe = false;

public:


    STATE GetState()const { return m_state; }
    void SetState(STATE val) { m_state = val; }
    void DoRecv();
    void DoSend(int num_bytes, void* mess);
    SOCKET& GetSock() { return m_socket; }
    void Init(SOCKET&);
    void ResetPlayer();
    virtual void Reset()override;
    void SetIsReady(bool val) { m_is_ready = val; }
    bool GetIsReady() { return m_is_ready; }

    void SetLoadProgressed(int val) { m_loadProgressed = val; }

    int GetLoadProgressed() { return m_loadProgressed; }
 
    void SetAttacked(bool val) { m_is_attacked = val; }

    bool GetAttacked() { return m_is_attacked; }
     
    void SetInvincible(bool val) { m_invincible = val; }

    bool GetInvincible() { return m_invincible; };

    char* GetPassword() { return m_password; }
    short GetMatchUserSize() { return m_mach_user_size; }
    void SetMatchUserSize(short val) { m_mach_user_size = val; }

    void SetItem(int val) { m_Item = val; }
    int GetItem() { return m_Item; };
    void DropItem() { m_Item = -1; }

    void SetHasDiamond(bool val) { m_has_diamond = val; };
    bool GetHasDiamond() { return m_has_diamond; };

};