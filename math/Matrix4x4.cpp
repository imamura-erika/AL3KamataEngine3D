#include "Matrix4x4.h"
#include <cmath>

// スケール行列の作成
Matrix4x4 Matrix4x4::Scale(const Vector3& scale) { 
	Matrix4x4 result = {}; 
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;
	result.m[3][3] = 1.0f;
	return result;
}

// x軸回転行列の作成
Matrix4x4 Matrix4x4::RotationX(float angle) { 
	Matrix4x4 result = {};
	float cosAngle = cosf(angle);
	float sinAngle = sinf(angle);
	result.m[0][0] = 1.0f;
	result.m[1][1] = cosAngle;
	result.m[1][2] = -sinAngle;
	result.m[2][1] = sinAngle;
	result.m[2][2] = cosAngle;
	result.m[3][3] = 1.0f;
	return result;
}

// y軸回転行列の作成
Matrix4x4 Matrix4x4::RotationY(float angle) { 
	Matrix4x4 result = {}; 
	float cosAngle = cosf(angle);
	float sinAngle = sinf(angle);
	result.m[0][0] = cosAngle;
	result.m[0][2] = sinAngle;
	result.m[1][1] = 1.0f;
	result.m[2][0] = -sinAngle;
	result.m[2][2] = cosAngle;
	result.m[3][3] = 1.0f;
	return result;
}

// z軸回転行列の作成
Matrix4x4 Matrix4x4::RotationZ(float angle) { 
	Matrix4x4 result = {};
	float cosAngle = cosf(angle);
	float sinAngle = sinf(angle);
	result.m[0][0] = cosAngle;
	result.m[0][1] = -sinAngle;
	result.m[1][0] = sinAngle;
	result.m[1][1] = cosAngle;
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	return result;
}

// 3軸回転行列の作成
Matrix4x4 Matrix4x4::Rotation(const Vector3& rotation) { 
	Matrix4x4 rotX = RotationX(rotation.x);
	Matrix4x4 rotY = RotationY(rotation.y);
	Matrix4x4 rotZ = RotationZ(rotation.z);
	return Multiply(Multiply(rotZ, rotY), rotX);
	}

// 平行移動行列の作成
Matrix4x4 Matrix4x4::Translation(const Vector3& translation) { 
	Matrix4x4 result = {};
	result.m[0][0] = 1.0f;
	result.m[1][1] = 1.0f;
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;
	result.m[0][3] = translation.x;
	result.m[1][3] = translation.y;
	result.m[2][3] = translation.z;
	return result;
}

// アフィン変換行列の作成
Matrix4x4 Matrix4x4::MakeAffineMatrix(const Vector3& scale, const Vector3& rotation, const Vector3& translation) {
	Matrix4x4 scaleMatrix = Scale(scale);
	Matrix4x4 rotationMatrix = Rotation(rotation);
	Matrix4x4 translationMatrix = Translation(translation);
	return Multiply(Multiply(scaleMatrix, rotationMatrix), translationMatrix);
}

// 行列の積
Matrix4x4 Matrix4x4::Multiply(const Matrix4x4& a, const Matrix4x4& b) {
	Matrix4x4 result = {};

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0;
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += a.m[i][k] * b.m[k][j];
			}
		}
	}
	return result;
}