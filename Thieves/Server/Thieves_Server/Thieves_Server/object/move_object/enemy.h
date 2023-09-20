#pragma once
#include "move_object.h"
#include "define.h"
//#include"collisioner.h"
#include<mutex>
#include<atomic>
#include<vector>
const int SYNC_TIME = 30;

class Enemy :
    public MoveObj
{
public:
    Enemy(int i) :in_use(false){
        npc_dir_num = 0;
        m_id = i;
        m_attack_time = std::chrono::system_clock::now();
        m_check_time = std::chrono::system_clock::now();
        m_move_time = std::chrono::system_clock::now();
    };
    ~Enemy()
    {};



    void InitEnemy(OBJ_TYPE type, int room_id, Vector3& pos);
    void SetSpawnPoint(float x, float z);
    Vector3& GetLookVec() { return m_look; }
    Vector3& GetPrevPos() { return m_prev_pos; }
    Vector3& GetPrevTestPos() { return m_prev_test_pos; }
    void SetPrevTestPos(const Vector3& val) { m_prev_test_pos = val; }
    virtual void Reset()override;

    const int GetTargetId()const { return target_id; }
    void SetTargetId(const int val) { target_id = val; }
    void SetMoveTime(int val);
    std::chrono::system_clock::time_point& GetAttackTime() { return m_attack_time; }
    std::chrono::system_clock::time_point& GetCheckTime() { return m_check_time; }
    void DoMove(const Vector3& target_pos);
    void DoPrevMove(const Vector3& target_pos);
    void SetToPrevPos() {
         m_pos = m_prev_pos;
    }

    void SetNpcDirNum(int val) {
        npc_dir_num = val;
    }
    int GetNpcDirNum() {
        return npc_dir_num;
    }
    int GetTargetID() {
        return target_id;
    }
    void SetTargetID(int val) {
        target_id = val;
    }
    std::atomic_bool in_use;
    std::atomic_bool in_game = false;

    Vector3 m_prev_test_pos{ 0.0f,0.0f,0.0f };
    std::chrono::system_clock::time_point	m_move_time;
    bool look_temp = TRUE;
    float look_x = 0;
    float look_z = 0;
    // base pos 저장됨.
    float e_dir_pos_x = 0;
    float e_dir_pos_z = 0;
    // astar 가야되는 경로 좌표들 저장됨.
    std::vector<Vector3> enemy_astar{};
    // 가야되는 경로좌표 저장 위치
    Vector3 astar_base_pos{};

    // 주변에 플레이어 있어?
    bool near_player = FALSE;
    
  
private:
    std::chrono::system_clock::time_point	m_attack_time;
    std::chrono::system_clock::time_point	m_check_time;

    Vector3 m_prev_pos{ 0.0f,0.0f,0.0f };
    Vector3 m_look;
    std::atomic_int target_id;
    
    int npc_dir_num;
    // target_id
    int astar_target_id = 9999;
    
};
