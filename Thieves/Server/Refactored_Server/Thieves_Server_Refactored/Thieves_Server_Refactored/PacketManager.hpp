#pragma once


class PacketManager
{
// 싱글톤 구현.
private:
	PacketManager() {};
	PacketManager(const PacketManager& other) {}
	PacketManager& operator=(const PacketManager& other) {}
	~PacketManager() = default;
public:
	static PacketManager& GetInst()
	{
		static PacketManager s;
		return s;
	}

	void Init();
	void ProcessPacket(int sessionId, unsigned char* packet);
};

