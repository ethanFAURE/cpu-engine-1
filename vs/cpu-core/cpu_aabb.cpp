#include "pch.h"

cpu_aabb::cpu_aabb()
{
	Zero();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cpu_aabb& cpu_aabb::operator=(const cpu_obb& obb)
{
	min = {  FLT_MAX,  FLT_MAX,  FLT_MAX };
	max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

	for ( int i=0 ; i<8 ; ++i )
	{
		const auto& p = obb.pts[i];

		if ( p.x<min.x )
			min.x = p.x;
		if ( p.y<min.y )
			min.y = p.y;
		if ( p.z<min.z )
			min.z = p.z;

		if ( p.x>max.x )
			max.x = p.x;
		if ( p.y>max.y )
			max.y = p.y;
		if ( p.z>max.z )
			max.z = p.z;
	}
	return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void cpu_aabb::Zero()
{
	min = { 0.0f, 0.0f, 0.0f };
	max = { 0.0f, 0.0f, 0.0f };
}

bool XM_CALLCONV cpu_aabb::ToScreen(cpu_rectangle& out, FXMMATRIX wvp, float renderWidth, float renderHeight)
{
	const float renderX = 0.0f;
	const float renderY = 0.0f;
	float minX =  FLT_MAX, minY =  FLT_MAX;
	float maxX = -FLT_MAX, maxY = -FLT_MAX;

	const float xmin = min.x;
	const float ymin = min.y;
	const float zmin = min.z;
	const float xmax = max.x;
	const float ymax = max.y;
	const float zmax = max.z;

	const XMVECTOR pts[8] =
	{
		XMVectorSet(xmin, ymin, zmin, 1.0f),
		XMVectorSet(xmax, ymin, zmin, 1.0f),
		XMVectorSet(xmax, ymax, zmin, 1.0f),
		XMVectorSet(xmin, ymax, zmin, 1.0f),
		XMVectorSet(xmin, ymin, zmax, 1.0f),
		XMVectorSet(xmax, ymin, zmax, 1.0f),
		XMVectorSet(xmax, ymax, zmax, 1.0f),
		XMVectorSet(xmin, ymax, zmax, 1.0f)
	};

	for ( int i=0 ; i<8 ; ++i )
	{
		XMVECTOR clip = XMVector4Transform(pts[i], wvp);

		float w = XMVectorGetW(clip);
		if ( w<=0.00001f )
			continue; // derrière caméra

		float invW = 1.0f / w;
		float ndcX = XMVectorGetX(clip) * invW;
		float ndcY = XMVectorGetY(clip) * invW;

		float sx = renderX + (ndcX * 0.5f + 0.5f) * renderWidth;
		float sy = renderY + (-ndcY * 0.5f + 0.5f) * renderHeight;

		minX = std::min(minX, sx);
		minY = std::min(minY, sy);
		maxX = std::max(maxX, sx);
		maxY = std::max(maxY, sy);
	}

	// Outside
	if ( minX>maxX || minY>maxY )
		return false;

	minX = cpu::Clamp(minX, renderX, renderX+renderWidth);
	maxX = cpu::Clamp(maxX, renderX, renderX+renderWidth);
	minY = cpu::Clamp(minY, renderY, renderY+renderHeight);
	maxY = cpu::Clamp(maxY, renderY, renderY+renderHeight);
	out.min = { minX, minY };
	out.max = { maxX, maxY };
	return true;
}

bool cpu_aabb::Contains(const XMFLOAT3& p)
{
	return p.x>=min.x && p.x<=max.x && p.y>=min.y && p.y<=max.y && p.z>=min.z && p.z<=max.z;
}
