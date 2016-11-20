#include "scene.h"
#include "game_object.h"
#include "collidable.h"
#include "table.h"
#include "player.h"
#include <cassert>
#include <stack>
#include <algorithm>

#include <DxLib.h> //test用

namespace 
{
	// 現在のシーン階層
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


GameObject * Scene::RegisterObject(GameObject * o)
{
	current_scene.top()->objects_.push_back(o);
	return o;
}

void Scene::RegisterCollider(Collidable * c)
{
	current_scene.top()->collider_.push_back(c);
}


Root::Root() : Scene(0), child_(nullptr)
{
	child_ = new GameMain();
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
	}
	return this;
}


GameMain::GameMain() : Scene(1), child_(nullptr)
{
	table_ = new Table({ 0.f, -9.4f, 0.f });
	Ball* b0 = table_->SetupNineBall();
	player_ = new Player(b0);

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

Scene* GameMain::Update()
{
	if (child_) {
		// 描画のみ行う
		for (auto o : objects_) {
			o->Draw();
		}

		Scene* next_scene = child_->Update();
		if (next_scene != child_) {
			delete child_;
			child_ = next_scene;
			if (child_ && child_->GetLevel() <= GetLevel()) {
				return child_;
			}
		}
		return this;
	}

	// 更新
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
	// 当たり判定
	bool all_col_finish = false;
	while (!all_col_finish) {
		all_col_finish = true;
		for (auto i = collider_.begin(); i != collider_.end() - 1; ++i) {
			for (auto j = i + 1; j != collider_.end(); ++j) {
				if (!(*i)->CollideWith(*j)) all_col_finish = false;
				if (!(*j)->CollideWith(*i)) all_col_finish = false;
			}
		}
	}
	collider_.clear();
	// 座標更新
	for (auto o : objects_) {
		o->UpdatePosition();
	}

	//描画
	for (auto o : objects_) {
		o->Draw();
	}

	return this;
}