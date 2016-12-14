#pragma once

#include "vector3.h"
#include "quaternion.h"

#include <string>

// 3Dƒ‚ƒfƒ‹
class Model
{
public:
	explicit Model(std::string file_name);
	virtual ~Model();

	void SetVisible(bool visible);
	void SetPosition(const vec3f& pos);
	void SetRotation(const vec3f& rot);
	void SetRotation(const vec3f& axis, float angle);
	void AddRotation(const vec3f& axis, float angle);
	void Draw() const;

	static void InitShadowMap();
	static void DestShadowMap();
	static void ShadowMapSetup(int num);
	static void ShadowMapEnd();
	static void UseShadowMapBegin();
	static void UseShadowMapEnd();

private:
	int handle_;
	bool visible_;
	vec3f pos_;
	Quaternion rot_;
};