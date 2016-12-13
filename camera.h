#pragma once

#include "vector2.h"
#include "vector3.h"

class Camera3D 
{
public:
	Camera3D();
	void SetViewTransform(const vec3f& p, const vec3f& t);
	vec3f ConvWorldPos(const Vector2<int>& screen_pos, float z) const;
	static Vector2<int> GetScreenSize();

private:
	float near_, far_, fov_;

	static const Vector2<int> screen_size_;
};