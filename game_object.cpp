#include "game_object.h"
#include "scene.h"

GameObject::GameObject(ObjectType type, const vec3f& pos) 
	: type_(type), pos_(pos), exist_(true)
{
	Scene::RegisterObject(this);
}

void GameObject::UpdatePosition()
{
}

void GameObject::Draw() const
{
}

const ObjectType GameObject::GetType() const
{
	return type_;
}

bool GameObject::GetExist() const
{
	return exist_;
}

const vec3f& GameObject::GetPos() const
{
	return pos_;
}

void GameObject::SetPos(const vec3f& pos)
{
	pos_ = pos;
}
