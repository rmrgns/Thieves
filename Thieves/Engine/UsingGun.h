#pragma once
#include "MonoBehaviour.h"

class UsingGun : public MonoBehaviour
{
public:
	UsingGun();
	virtual ~UsingGun();

	void DrawBullet();
	void RecvBullet(Vec3, Vec3, Vec3);
	virtual void LateUpdate() override;
	

	Vec3 bullet_pos{ 0.0f, 0.0f, 0.0f };
	Vec3 bullet_rotation{ 0.0f, 0.0f, 0.0f };
	bool _recv_bullet_pos = false;
	Vec3 GetBulletPos() {return bullet_pos;}
	void SetBulletPos(Vec3 val) { bullet_pos = val; }
	Vec3 GetBulletRot() { return bullet_rotation; }
	void SetBulletRot(Vec3 val) { bullet_rotation = val; }

	void SetBulletbool(bool val) { _recv_bullet_pos = val; }
	bool GetBulletbool() { return _recv_bullet_pos; }
private:
	float _count = 0.f;


	bool _bulletcheck = false;


};