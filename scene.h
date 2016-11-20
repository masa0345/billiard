#pragma once

#include <vector>

class GameObject;
class Collidable;

// �S�ẴV�[���̊��N���X
class Scene
{
public:
	explicit Scene(int level);
	virtual ~Scene();
	virtual Scene* Update() = 0;
	unsigned int GetLevel() const;
protected:
	std::vector<GameObject*> objects_;
	std::vector<Collidable*> collider_;
private:
	unsigned int level_; // �V�[���K�w�̐[��

public:
	static GameObject* RegisterObject(GameObject* o);
	static void RegisterCollider(Collidable* c);
};

// ���[�g�V�[��
class Root : public Scene
{
public:
	Root();
	virtual ~Root();
	virtual Scene* Update() override;
private:
	Scene* child_;
};

// �I�� ���̃V�[����Ԃ��ƃE�B���h�E������
class Exit : public Scene
{
public:
	Exit() : Scene(0) {}
	virtual ~Exit() = default;
	virtual Scene* Update() override { return nullptr; }
};

class Table;
class Player;

// ���C���V�[��
class GameMain : public Scene
{
public:
	GameMain();
	virtual ~GameMain();
	virtual Scene* Update() override;
private:
	Scene* child_;
	Table* table_;
	Player* player_;
};