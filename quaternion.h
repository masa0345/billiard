#pragma once

#include "vector3.h"

// �l����
class Quaternion 
{
	friend class Matrix4x3;
public:
	Quaternion();
	Quaternion(float x, float y, float z, float w);
	Quaternion(const vec3f& axis, float angle);
	explicit Quaternion(const vec3f& euler);

	// X->Y->Z��]�̃I�C���[�p�ɕϊ�
	//vec3f ToEuler() const;
	// ����
	float Dot(const Quaternion& q) const;
	// �t��
	const Quaternion Inverse() const;

	// �O��
	const Quaternion operator*(const Quaternion& rhs) const;
	Quaternion& operator*=(const Quaternion& rhs);

	// ���ʐ��`���
	static const Quaternion Slerp(const Quaternion& from, const Quaternion& to, float time);

private:
	vec3f v;
	float w;
};