#pragma once
#include "object/object.h"
#include "collisioner.h"
#include <d3d12.h>
#include <DirectXMath.h>
#include <DirectXcollision.h> 

class MapObj : public Object
{
public:
	//MapObj(int id, const Vector3& pos, const Vector3& extent, bool is_blocking, OBJ_TYPE type)
	//	:m_extent(extent), m_is_blocking(is_blocking)
	//{
	//	m_id = id;
	//	m_pos = pos;
	//	m_type = type;
	//	m_ground_pos = Vector3(pos.x, 0 , pos.z);
	//	SetMinPos();
	//	SetMaxPos();
	//	//std::cout << m_min_pos << std::endl;
	//	//std::cout << m_max_pos << std::endl;

	//};
	MapObj(Vector3 center, Vector3 extent, Vector3 up, Vector3 right, Vector3 look)
	{
		// 센터와 익스텐트는 그대로 넣는다
		m_obb.Center = DirectX::XMFLOAT3(center.x, center.y, center.z);
		m_obb.Extents = DirectX::XMFLOAT3(extent.x, extent.y, extent.z);

		// 기저 벡터는 쿼터니언을 기준으로 하므로 계산을 해 줘야 한다.
		DirectX::XMFLOAT3 upVector = DirectX::XMFLOAT3(up.x, up.y, up.z);
		DirectX::XMFLOAT3 rightVector = DirectX::XMFLOAT3(right.x, right.y, right.z);
		DirectX::XMFLOAT3 lookVector = DirectX::XMFLOAT3(look.x, look.y, look.z);

		DirectX::XMVECTOR Vup = DirectX::XMLoadFloat3(&upVector);
		DirectX::XMVECTOR Vright = DirectX::XMLoadFloat3(&rightVector);
		DirectX::XMVECTOR Vlook = DirectX::XMLoadFloat3(&lookVector);

		DirectX::XMVECTOR origin = DirectX::XMVectorSet(0.f, 0.f, 0.f, 0.f);
		DirectX::XMMATRIX view = DirectX::XMMatrixLookToRH(origin, Vlook, Vup);

		DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationMatrix(view);

		DirectX::XMFLOAT4 orientation;
		DirectX::XMStoreFloat4(&orientation, quaternion);
		m_obb.Orientation = orientation;

	};

	MapObj() = default;
	~MapObj() = default;
//	bool GetIsBlocked() { return m_is_blocking; }
//	void SetIsBlocked(bool val) { m_is_blocking = val; }
	const Vector3& GetMaxPos()const { return m_max_pos; }
	const Vector3& GetMinPos()const { return m_min_pos; }
//	const Vector3& GetGroundPos()const { return m_ground_pos; }
	const Vector3& GetExtent()const { return m_extent; }
private:

	void SetMinPos();
	void SetMaxPos();
//	bool m_is_blocking = false;
	Vector3 m_min_pos;
	Vector3 m_max_pos;
	Vector3 m_extent;
//	Vector3 m_ground_pos;

	DirectX::BoundingOrientedBox m_obb;
};
