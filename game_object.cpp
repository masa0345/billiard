#include "game_object.h"
#include "scene.h"

GameObject::GameObject(ObjectType type, const vec3f& pos) 
	: my_scene_(Scene::GetCurrentScene()), type_(type), pos_(pos), exist_(true)
{
	my_scene_->RegisterObject(this);
}

void GameObject::UpdatePosition()
{
}

void GameObject::Draw() const
{
}

void GameObject::DrawShadowMap() const
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
