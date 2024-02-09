#pragma once

#include <raylib.h>
#include <raymath.h>

// wrapper class because I want operators
struct Vec3 : public Vector3
{
	Vec3() = default;
	~Vec3() = default;

	Vec3(const Vector3& other)
		:Vector3(other)
	{};

	Vec3 operator+(const Vec3& other)
	{ return Vector3Add(*this, other); }

	Vec3 operator-(const Vec3& other)
	{ return Vector3Subtract(*this, other); }

	Vec3 operator*(float fl)
	{ return Vector3Scale(*this, fl); }

	Vec3 operator/(float fl)
	{ return Vector3Scale(*this, 1.0f/fl); }

	float dot(const Vec3& other)
	{ return Vector3DotProduct(*this, other); }

	Vec3 cross(const Vec3& other)
	{ return Vector3CrossProduct(*this, other); }

	Vec3 normalize()
	{ return Vector3Normalize(*this); }

	float length()
	{ return Vector3Length(*this); }
};
