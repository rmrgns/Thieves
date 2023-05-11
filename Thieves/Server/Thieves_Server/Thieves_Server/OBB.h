#pragma once
#include <d3d12.h>
#include <DirectXMath.h>
#include <DirectXcollision.h> 
#include <vec/vec3.h>



class OBB
{
public:

	OBB() = default;

	OBB(Vector3 Center, Vector3 Extents, Vector3 up, Vector3 right, Vector3 look) {

		
		m_obb.Center = DirectX::XMFLOAT3(Center.x, Center.y, Center.z);
		m_obb.Extents = DirectX::XMFLOAT3(Extents.x, Extents.y, Extents.z);

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

	~OBB() {};

	DirectX::BoundingOrientedBox& GetObb() { return m_obb; };

	bool ObbIntersectWithBox(const OBB& other_obb) {
		return m_obb.Intersects(other_obb.m_obb);
	}
private:
	DirectX::BoundingOrientedBox m_obb;
};

