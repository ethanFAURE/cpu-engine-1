#include "stdafx.h"

TRANSFORM::TRANSFORM()
{
	Identity();
}

void TRANSFORM::Identity()
{
	pos = { 0.0f, 0.0f, 0.0f };
	sca = { 1.0f, 1.0f, 1.0f };
	ResetRotation();
}

void TRANSFORM::UpdateWorld()
{
	DirectX::XMVECTOR s = XMLoadFloat3(&sca);
	DirectX::XMVECTOR p = XMLoadFloat3(&pos);

	DirectX::XMVECTOR sx = DirectX::XMVectorSplatX(s);
	DirectX::XMVECTOR sy = DirectX::XMVectorSplatY(s);
	DirectX::XMVECTOR sz = DirectX::XMVectorSplatZ(s);

	DirectX::XMMATRIX w = XMLoadFloat4x4(&rot);
	w.r[0] = DirectX::XMVectorMultiply(w.r[0], sx);
	w.r[1] = DirectX::XMVectorMultiply(w.r[1], sy);
	w.r[2] = DirectX::XMVectorMultiply(w.r[2], sz);
	w.r[3] = DirectX::XMVectorSetW(p, 1.0f);

	XMStoreFloat4x4(&world, w);
}

void TRANSFORM::SetScaling(float scale)
{
	sca.x = scale;
	sca.y = scale;
	sca.z = scale;
}

void TRANSFORM::SetPosition(float x, float y, float z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;
}

void TRANSFORM::Move(float dist)
{
	pos.x += dir.x * dist;
	pos.y += dir.y * dist;
	pos.z += dir.z * dist;
}

void TRANSFORM::ResetRotation()
{
	dir = { 0.0f, 0.0f, 1.0f };
	right = { 1.0f, 0.0f, 0.0f };
	up = { 0.0f, 1.0f, 0.0f };
	quat = { 0.0f, 0.0f, 0.0f, 1.0f };
	XMStoreFloat4x4(&rot, DirectX::XMMatrixIdentity());
}

void TRANSFORM::SetRotation(TRANSFORM& transform)
{
	dir = transform.dir;
	right = transform.right;
	up = transform.up;
	quat = transform.quat;
	rot = transform.rot;
}

void TRANSFORM::SetYPR(float yaw, float pitch, float roll)
{
	ResetRotation();
	AddYPR(yaw, pitch, roll);
}

void TRANSFORM::AddYPR(float yaw, float pitch, float roll)
{
	DirectX::XMVECTOR axisDir = XMLoadFloat3(&dir);
	DirectX::XMVECTOR axisRight = XMLoadFloat3(&right);
	DirectX::XMVECTOR axisUp = XMLoadFloat3(&up);

	DirectX::XMVECTOR qRot = XMLoadFloat4(&quat);
	if ( roll )
		qRot = DirectX::XMQuaternionMultiply(qRot, DirectX::XMQuaternionRotationAxis(axisDir, roll));
	if ( pitch )
		qRot = DirectX::XMQuaternionMultiply(qRot, DirectX::XMQuaternionRotationAxis(axisRight, pitch));
	if ( yaw )
		qRot = DirectX::XMQuaternionMultiply(qRot, DirectX::XMQuaternionRotationAxis(axisUp, yaw));

	XMStoreFloat4(&quat, qRot);

	DirectX::XMMATRIX mRot = DirectX::XMMatrixRotationQuaternion(qRot);
	XMStoreFloat4x4(&rot, mRot);

	right.x = rot._11;
	right.y = rot._12;
	right.z = rot._13;
	up.x = rot._21;
	up.y = rot._22;
	up.z = rot._23;
	dir.x = rot._31;
	dir.y = rot._32;
	dir.z = rot._33;
}

void TRANSFORM::LookAt(float x, float y, float z)
{
	DirectX::XMVECTOR vA = XMLoadFloat3(&pos);
	DirectX::XMVECTOR vB = DirectX::XMVectorSet(x, y, z, 0.0f);
	DirectX::XMVECTOR vDir = DirectX::XMVectorSubtract(vB, vA);
	const DirectX::XMMATRIX cam = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(DirectX::XMVectorZero(), vDir, XMUP));
	XMStoreFloat4x4(&rot, cam);

	XMStoreFloat4(&quat, DirectX::XMQuaternionRotationMatrix(cam));

	right.x = rot._11;
	right.y = rot._12;
	right.z = rot._13;
	up.x = rot._21;
	up.y = rot._22;
	up.z = rot._23;
	dir.x = rot._31;
	dir.y = rot._32;
	dir.z = rot._33;
}

void TRANSFORM::LookTo(float ndx, float ndy, float ndz)
{
	DirectX::XMVECTOR vDir = DirectX::XMVectorSet(ndx, ndy, ndz, 0.0f);
	DirectX::XMMATRIX cam = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookToLH(DirectX::XMVectorZero(), vDir, XMUP));
	XMStoreFloat4x4(&rot, cam);

	XMStoreFloat4(&quat, DirectX::XMQuaternionRotationMatrix(cam));

	right.x = rot._11;
	right.y = rot._12;
	right.z = rot._13;
	up.x = rot._21;
	up.y = rot._22;
	up.z = rot._23;
	dir.x = rot._31;
	dir.y = rot._32;
	dir.z = rot._33;
}

void TRANSFORM::LookTo(DirectX::XMFLOAT3& ndir)
{
	LookTo(ndir.x, ndir.y, ndir.z);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ENTITY::ENTITY()
{
	index = -1;
	sortedIndex = -1;
	dead = false;
	pMesh = nullptr;
	material = Engine::ToColor(255, 255, 255);
	lifetime = 0.0f;
	tile = 0;
}
