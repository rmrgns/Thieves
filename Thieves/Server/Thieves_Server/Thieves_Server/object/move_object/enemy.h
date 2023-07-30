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

    std::atomic_bool in_use;
    std::atomic_bool in_game = false;

    Vector3 m_prev_test_pos{ 0.0f,0.0f,0.0f };
    std::chrono::system_clock::time_point	m_move_time;
private:
    std::chrono::system_clock::time_point	m_attack_time;
    std::chrono::system_clock::time_point	m_check_time;

    Vector3 m_prev_pos{ 0.0f,0.0f,0.0f };
    Vector3 m_look;
    std::atomic_int target_id;
};
