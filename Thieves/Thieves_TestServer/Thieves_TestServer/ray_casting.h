#pragma once
#include "pch.h"

class RayCasting
{
	RayCasting();
	~RayCasting();

private:
	// �Ѿ� �߻� ��ġ
	Vector3 bulletPos;

	// �� ���� �÷��̾� ���⺤��
	Vector3 beulletDir;

	//������ ���⺤��
	float ray;

	//�������� ������ ���ڷ� �޾Ƽ� ���� ǥ���Ѵ�.
	//�浹 ������ ���� ���ͱ״�� �� ����
public:
		bool Shoot();

};