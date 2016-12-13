#include "scene.h"
#include "game_object.h"
#include "collidable.h"
#include "table.h"
#include "player.h"
#include "camera.h"
#include "graphics.h"
#include "ui.h"
#include <cassert>
#include <stack>
#include <algorithm>

#include <DxLib.h>

namespace 
{
	// ���݂̃V�[���K�w
	std::stack<Scene*> current_scene;
}

Scene::Scene(int level) : level_(level)
{
	assert(level_ <= current_scene.size());
	while (level_ < current_scene.size()) {
		current_scene.pop();
	}
	if (level_ == current_scene.size()) {
		current_scene.push(this);
	}
	objects_.reserve(64);
	collider_.reserve(64);
}

Scene::~Scene()
{
	for (auto o : objects_) delete o;
	if (current_scene.top() == this) current_scene.pop();
}

unsigned int Scene::GetLevel() const
{
	return level_;
}

void Scene::Init()
{
	// �V�[���̎��s���O��1�x�������s����� 
	// (Fade�n�̑�2�����ɓn�����V�[���Ȃǂ͐���������s�܂łɎ��ԍ�������)
	// �J�����̈ʒu, BGM�Ȃǂ̏����ݒ�Ɏg��
}

void Scene::UpdateGameObjects()
{
	for (auto it = objects_.begin(); it != objects_.end();) {
		if ((*it)->GetExist()) {
			(*it)->Update();
			(*it)->UpdatePosition();
			++it;
		} else {
			delete *it;
			*it = nullptr;
			it = objects_.erase(it);
		}
	}
	collider_.clear();
}

void Scene::DrawGameObjects() const
{
	for (auto o : objects_) {
		o->Draw();
	}
	Graphics2D::Update();
}


GameObject * Scene::RegisterObject(GameObject * o)
{
	assert(current_scene.top()->objects_.size() < current_scene.top()->objects_.capacity());
	current_scene.top()->objects_.push_back(o);
	return o;
}

void Scene::RegisterCollider(Collidable * c)
{
	current_scene.top()->collider_.push_back(c);
}


Root::Root() : Scene(0), child_(nullptr)
{
	child_ = new Title();
	child_->Init();
}

Root::~Root()
{
	if (child_) delete child_;
}

Scene* Root::Update()
{
	Scene* next_scene = child_->Update();
	if (next_scene != child_) {
		delete child_;
		child_ = next_scene;
		assert(child_);
		if (child_->GetLevel() <= GetLevel()) {
			return child_;
		}
		child_->Init();
	}
	return this;
}


Title::Title() : Scene(1), child_(nullptr)
{
}

Title::~Title()
{
	if (child_) delete child_;
}

Scene * Title::Update()
{
	// �q�V�[��������΂���������s
	if (child_) {

		Graphics2D::DrawString(100, 100, "�����Ƃ�", 0xffffff);
		// �`��̂ݍs��
		DrawGameObjects();

		Scene* next_scene = child_->Update();
		if (next_scene != child_) {
			delete child_;
			child_ = next_scene;
			if (child_ && child_->GetLevel() <= GetLevel()) {
				child_ = nullptr;
				return next_scene;
			}
			if (child_) child_->Init();
		}
		return this;
	}

	child_ = new FadeOut(30, new GameMain());

	Graphics2D::DrawString(100, 100, "�����Ƃ�", 0xffffff);

	UpdateGameObjects();
	DrawGameObjects();

	return this;
}


GameMain::GameMain() : Scene(1), child_(nullptr)
{
	//Ball* ball = new Ball({ 0.f, 0.f, 0.f }, 0);
	//Ball* ball = new Ball({ -8.f, 0.f, 3.f }, 0);
	//for (int i = 1; i < 16; ++i) {
		//new Ball({ -5.76f + 0.72f*i, 0.f, 3.f }, i);
		//new Ball({ 4.38f+0.72f*i, 0.f, 3.f }, i);
		//new Ball({ -8.f + 1.f*i, 0.f, 7.95f-0.36f }, i);
	//}
}

GameMain::~GameMain()
{
	if(child_) delete child_;
}

void GameMain::Init()
{
	table_ = new Table({ 0.f, -9.4f, 0.f });
	Ball* b0 = table_->SetupNineBall();
	player_ = new Player(b0, table_);
	player_->SetCueVisible(false);
	menu_btn_ = new Button(580, 10, 50, 30, "MENU");

	child_ = new FadeIn(30);
	child_->Init();
}

Scene* GameMain::Update()
{
	// �q�V�[��������΂���������s
	if (child_) {
		// �`��̂ݍs��
		DrawGameObjects();

		Scene* next_scene = child_->Update();
		if (next_scene != child_) {
			delete child_;
			child_ = next_scene;
			if (child_ && child_->GetLevel() <= GetLevel()) {
				child_ = nullptr;
				return next_scene;
			}
			if(child_) child_->Init();
		}
		return this;
	}

	if (menu_btn_->IsMouseOver()) {
		if (menu_btn_->IsClicked()) {
			DrawGameObjects();
			child_ = new Pause(player_);
			return this;
		}
		MouseInput::GetInstance().SetValid(false);
	}

	// �X�V
	UpdateGameObjects();
	//�`��
	DrawGameObjects();

	for (int i = 0; i <= 9; ++i) {
		printfDx("%d", table_->GetBallState(i));
	}
	printfDx("\n");

	return this;
}

void GameMain::UpdateGameObjects()
{
	// �X�V
	for (auto it = objects_.begin(); it != objects_.end();) {
		if ((*it)->GetExist()) {
			(*it)->Update();
			++it;
		} else {
			delete *it;
			*it = nullptr;
			it = objects_.erase(it);
		}
	}
	// �����蔻��
	bool all_col_finish = false;
	while (!all_col_finish) {
		all_col_finish = true;
		if (collider_.size() < 2) break;
		for (auto i = collider_.begin(); i != collider_.end() - 1; ++i) {
			for (auto j = i + 1; j != collider_.end(); ++j) {
				if (!(*i)->CollideWith(*j)) all_col_finish = false;
				if (!(*j)->CollideWith(*i)) all_col_finish = false;
			}
		}
	}
	bool all_stationary = true;
	for (auto c : collider_) {
		if (!c->IsStatic()) {
			all_stationary = false;
			break;
		}
	}
	collider_.clear();
	player_->SetCueVisible(all_stationary);
	// ���W�X�V
	for (auto o : objects_) {
		o->UpdatePosition();
	}
}


FadeIn::FadeIn(int time, Scene* next) : 
	Scene(2), time_(time), next_(next), bright(255), fade_rate_(255 / time)
{
	assert(time_ >= 0);
}

FadeIn::~FadeIn()
{
	if (time_ > 0 && next_) delete next_;
}

Scene* FadeIn::Update()
{
	bright -= fade_rate_;
	auto screen_size = Camera3D::GetScreenSize();
	Graphics2D::DrawRect(0, 0, screen_size.x, screen_size.y, 0, true, bright);
	Graphics2D::Update();
	if (--time_ == 0) return next_;
	return this;
}

FadeOut::FadeOut(int time, Scene* next) :
	Scene(2), time_(time), next_(next), bright(0), fade_rate_(255 / time)
{
	assert(time_ >= 0);
}

FadeOut::~FadeOut()
{
	if (time_ > 0 && next_) delete next_;
}

Scene* FadeOut::Update()
{
	bright += fade_rate_;
	auto screen_size = Camera3D::GetScreenSize();
	Graphics2D::DrawRect(0, 0, screen_size.x, screen_size.y, 0, true, bright);
	Graphics2D::Update();
	if (--time_ == 0) return next_;
	return this;
}

Pause::Pause(Player* p) : Scene(2), menu_(new PauseMenu(p))
{
}

Scene* Pause::Update()
{
	if (menu_->PushClose()) return nullptr;
	if (menu_->PushRestart()) return new FadeOut(60, new GameMain());
	if (menu_->PushTitle()) return new FadeOut(60, new Title());

	UpdateGameObjects();

	auto screen_size = Camera3D::GetScreenSize();
	Graphics2D::DrawRect(0, 0, screen_size.x, screen_size.y, 0, true, 128);
	Graphics2D::Update();
	DrawGameObjects();

	return this;
}
