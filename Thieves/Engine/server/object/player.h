#pragma once


class Player
{
public:
	Player();
	~Player();
private:
	Vec3 PlayerPos;


	void Init() {};

	void GetPos();
	Vec3 SetPos();

};

