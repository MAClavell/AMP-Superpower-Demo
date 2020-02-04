#pragma once
#include <DirectXMath.h>
#include <algorithm>

// --------------------------------------------------------
//Quickly add two XMFLOAT3s together
// --------------------------------------------------------
static DirectX::XMFLOAT3 XMFloat3QuickAdd(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b)
{
	DirectX::XMFLOAT3 res;
	DirectX::XMStoreFloat3(&res, DirectX::XMVectorAdd(
		DirectX::XMLoadFloat3(&a), DirectX::XMLoadFloat3(&b)
	));
	return res;
}

// --------------------------------------------------------
//Quickly subtract two XMFLOAT3s from eachother
// --------------------------------------------------------
static DirectX::XMFLOAT3 XMFloat3QuickSubtract(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b)
{
	DirectX::XMFLOAT3 res;
	DirectX::XMStoreFloat3(&res, DirectX::XMVectorSubtract(
		DirectX::XMLoadFloat3(&a), DirectX::XMLoadFloat3(&b)
	));
	return res;
}

// --------------------------------------------------------
//Clamp a value between a range
// --------------------------------------------------------
static float Clamp(float value, float min, float max)
{
	return (std::max)(min, (std::min)(value, max));
}

// --------------------------------------------------------
//Returns whether the given value is in a min-max range.
// --------------------------------------------------------
static bool InRange(float value, float min, float max)
{
	return value > min && value < max;
}

// --------------------------------------------------------
//Returns whether the given value is in a positive-negative single-value range.
// --------------------------------------------------------
static bool InRange(float value, float range)
{
	return value > -range && value < range;
}

// --------------------------------------------------------
//Lerps until the result is negligably close to the target value.
// --------------------------------------------------------
static float LerpThreshhold(float v1, float v2, float t, float thresh = 0.001)
{
	if (abs(v1 - v2) > thresh)
		return v1 + t * (v2 - v1);
	else
		return v2;
	return 0;
}

// --------------------------------------------------------
//Calculate the distance between two Float3s
// --------------------------------------------------------
static float DistanceFloat3(DirectX::XMFLOAT3 v1, DirectX::XMFLOAT3 v2)
{
	DirectX::XMVECTOR dist = DirectX::XMVector3Length(
		DirectX::XMVectorSubtract(
			DirectX::XMLoadFloat3(&v1), DirectX::XMLoadFloat3(&v2))
	);
	return DirectX::XMVectorGetX(dist);
}

// --------------------------------------------------------
//Extract a quaternion from a rotation matrix
//https://forum.unity.com/threads/how-to-assign-matrix4x4-to-transform.121966/
// --------------------------------------------------------
static DirectX::XMFLOAT4 MatrixToQuaternion(DirectX::XMFLOAT4X4 m)
{
	float tr = m._11 + m._22 + m._33;
	float w, x, y, z;
	if (tr > 0.0f) {
		float s = sqrt(1.0f + tr) * 2.0f;
		w = 0.25f * s;
		x = (m._32 - m._23) / s;
		y = (m._13 - m._31) / s;
		z = (m._21 - m._12) / s;
	}
	else if ((m._11 > m._22) && (m._11 > m._33)) {
		float s = sqrt(1.0f + m._11 - m._22 - m._33) * 2.0f;
		w = (m._32 - m._23) / s;
		x = 0.25f * s;
		y = (m._12 + m._21) / s;
		z = (m._13 + m._31) / s;
	}
	else if (m._22 > m._33) {
		float s = sqrt(1.0f + m._22 - m._11 - m._33) * 2.0f;
		w = (m._13 - m._31) / s;
		x = (m._12 + m._21) / s;
		y = 0.25f * s;
		z = (m._23 + m._32) / s;
	}
	else {
		float s = sqrt(1.0f + m._33 - m._11 - m._22) * 2.0f;
		w = (m._21 - m._12) / s;
		x = (m._13 + m._31) / s;
		y = (m._23 + m._32) / s;
	z = 0.25f * s;
	}

		DirectX::XMFLOAT4 quat = DirectX::XMFLOAT4(x, y, z, w);
	return quat;
}

// --------------------------------------------------------
// Create a world matrix from a position and scale
// --------------------------------------------------------
static DirectX::XMFLOAT4X4 CreateWorldMatrix(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 scale)
{
	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslationFromVector(XMLoadFloat3(&position));
	DirectX::XMMATRIX scaling = DirectX::XMMatrixScalingFromVector(XMLoadFloat3(&scale));
	DirectX::XMMATRIX world = DirectX::XMMatrixTranspose(scaling * translation);
	DirectX::XMFLOAT4X4 worldMat;
	DirectX::XMStoreFloat4x4(&worldMat, world);
	return worldMat;
}
// --------------------------------------------------------
// Create a world matrix from a position, rotation, and scale
// --------------------------------------------------------
static DirectX::XMFLOAT4X4 CreateWorldMatrix(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 rotation, DirectX::XMFLOAT3 scale)
{
	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslationFromVector(XMLoadFloat3(&position));
	DirectX::XMMATRIX rot = DirectX::XMMatrixRotationQuaternion(XMLoadFloat4(&rotation));
	DirectX::XMMATRIX scaling = DirectX::XMMatrixScalingFromVector(XMLoadFloat3(&scale));
	DirectX::XMMATRIX world = DirectX::XMMatrixTranspose(scaling * rot * translation);
	DirectX::XMFLOAT4X4 worldMat;
	DirectX::XMStoreFloat4x4(&worldMat, world);
	return worldMat;
}