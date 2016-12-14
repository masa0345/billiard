#pragma once

#include "vector3.h"

enum ObjectType
{
	PLAYER,
	BALL,
	TABLE,
	UI,
};

class Scene;

class GameObject
{
public:
	GameObject(ObjectType type, const vec3f& pos);
	virtual ~GameObject() = default;

	virtual void Update() = 0;
	virtual void UpdatePosition();
	virtual void Draw() const;
	virtual void DrawShadowMap() const;

	const ObjectType GetType() const;
	bool GetExist() const;
	const vec3f& GetPos() const;
	void SetPos(const vec3f&);

protected:
	ObjectType type_;
	bool exist_;
	vec3f pos_;
	Scene* const my_scene_;
};