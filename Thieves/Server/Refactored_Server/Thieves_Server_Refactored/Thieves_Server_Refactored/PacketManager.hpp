#pragma once
#include <span>

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
		// Instance를 요청했을 때 생성한다!
		static PacketManager s;
		return s;
	}

	void Init();
	void ProcessPacket(int sessionId, std::span<char> packet);
};

