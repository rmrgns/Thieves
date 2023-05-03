#pragma once
class GameInfo {
public:
	GameInfo() {};
	~GameInfo() = default;

	int GetRoomID() { return m_room_id; }
	void SetRoomID(int val) { m_room_id = val; }
	int GetNetworkID() { return m_network_id; }
	void SetNetworkID(int val) { m_network_id = val; }

private:
	int m_room_id = 0;
	int m_network_id = -1;

};