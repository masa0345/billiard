#pragma once

#include <vector>

class GameObject;
class Collidable;
enum ObjectType;

// �S�ẴV�[���̊��N���X
class Scene
{
public:
	explicit Scene(int level);
	virtual ~Scene();
	virtual void Init();
	virtual Scene* Update() = 0;
	unsigned int GetLevel() const;
protected:
	virtual void UpdateGameObjects();
	virtual void DrawGameObjects() const;

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


// �^�C�g��
class Title : public Scene
{
public:
	Title();
	virtual ~Title();
	virtual Scene* Update() override;
private:
	Scene* child_;
};


class Table;
class Player;
class Button;

// ���C���V�[��
class GameMain : public Scene
{
public:
	GameMain();
	virtual ~GameMain();
	virtual void Init() override;
	virtual Scene* Update() override;
private:
	virtual void UpdateGameObjects() override;

	Scene* child_;
	Table* table_;
	Player* player_;
	Button* menu_btn_;
};

// �t�F�[�h�C��
class FadeIn : public Scene
{
public:
	FadeIn(int time, Scene* next = nullptr);
	virtual ~FadeIn();
	virtual Scene* Update() override;
private:
	int time_;
	int bright, fade_rate_;
	Scene* next_;
};

// �t�F�[�h�A�E�g
class FadeOut : public Scene
{
public:
	FadeOut(int time, Scene* next = nullptr);
	virtual ~FadeOut();
	virtual Scene* Update() override;
private:
	int time_;
	int bright, fade_rate_;
	Scene* next_;
};

class PauseMenu;

// �|�[�Y
class Pause : public Scene
{
public:
	Pause(Player* p);
	virtual ~Pause() = default;
	virtual Scene* Update() override;
private:
	PauseMenu* menu_;
};