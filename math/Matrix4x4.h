#pragma once
#include "Vector3.h"
/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];

	// スケール行列の作成
	static Matrix4x4 Scale(const Vector3& scale);

	// 回転行列の作成
	static Matrix4x4 RotationX(float angle);
	static Matrix4x4 RotationY(float angle);
	static Matrix4x4 RotationZ(float angle);
	static Matrix4x4 Rotation(const Vector3& rotation);

	// 平行移動行列の作成
	static Matrix4x4 Translation(const Vector3& translation);

	// アフィン変換行列の作成
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotation, const Vector3& translation);

	// 行列のかけ算
	static Matrix4x4 Multiply(const Matrix4x4& a, const Matrix4x4& b);
};