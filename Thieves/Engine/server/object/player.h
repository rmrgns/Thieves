#pragma once


class Player
{
public:
	Player();
	~Player();
private:
	Vec3 PlayerPos;


public:
	Player();
	~Player();

	void Init() {};

	void GetPos();
	Vec3 SetPos();

};

