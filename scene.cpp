#include "scene.h"
#include "game_object.h"
#include "collidable.h"
#include "table.h"
#include "player.h"
#include "ball.h"
#include "camera.h"
#include "graphics.h"
#include "ui.h"
#include "sound.h"
#include <cassert>
#include <stack>
#include <string>
#include <algorithm>


namespace 
{
	// 現在のシーン階層
	std::stack<Scene*> current_scene;
}

Scene::Scene(int level) : level_(level), child_(nullptr)
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
	if (child_) delete child_;
	for (auto o : objects_) delete o;
	if (current_scene.top() == this) current_scene.pop();
}

unsigned int Scene::GetLevel() const
{
	return level_;
}

void Scene::Init()
{
	// シーンの実行直前に1度だけ実行される 
	// (Fade系の第2引数に渡したシーンなどは生成から実行までに時間差がある)
	// カメラの位置, BGMなどの初期設定に使う
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

Scene* Scene::UpdateChild()
{
	assert(child_);
	Scene* next_scene = child_->Update();
	if (next_scene != child_) {
		delete child_;
		if (next_scene && next_scene->GetLevel() <= GetLevel()) {
			child_ = nullptr;
			return next_scene;
		}
		child_ = next_scene;
		if (child_) child_->Init();
	}
	return this;
}

Scene* Scene::GetCurrentScene()
{
	return current_scene.top();
}

void Scene::RegisterObject(GameObject* o)
{
	assert(objects_.size() < objects_.capacity());
	objects_.push_back(o);
}

void Scene::RegisterCollider(Collidable* c)
{
	collider_.push_back(c);
}


// ルート
Root::Root() : Scene(0)
{
	child_ = new Title();
	child_->Init();
}

Scene* Root::Update()
{
	return UpdateChild();
}


// タイトル
Title::Title(int fade) : Scene(1)
{
	if (fade > 0) {
		child_ = new FadeIn(fade);
	}
}

void Title::Init()
{
	if (child_) child_->Init();
	Table* t = new Table(vec3f());
	t->SetupEightBall();
	Camera3D camera;
	vec3f camera_pos = t->GetPos() + vec3f(t->GetWidth()-2.7f, t->GetHeight()+2.1f, -1.4f);
	camera.SetViewTransform(camera_pos, vec3f(t->GetWidth()*2.f, t->GetHeight(), 0.f));
	auto screen_size = Camera3D::GetScreenSize();
	nine_  = new StringButton(screen_size.x - 220, screen_size.y - 220, 3, "9ボール配置");
	eight_ = new StringButton(screen_size.x - 220, screen_size.y - 170, 3, "8ボール配置");
	exit_  = new StringButton(screen_size.x - 220, screen_size.y - 100, 3, "終了");
}

Scene * Title::Update()
{
	// 子シーンがあればそちらを実行
	if (child_) {
		// 描画のみ行う
		auto screen_size = Camera3D::GetScreenSize();
		Graphics2D::DrawFontStringCenter(2, screen_size.x / 2, 100, 0xffffff, "1人ビリヤード");
		DrawGameObjects();
		return UpdateChild();
	}

	UpdateGameObjects();

	auto screen_size = Camera3D::GetScreenSize();
	Graphics2D::DrawFontStringCenter(2, screen_size.x / 2, 100, 0xffffff, "1人ビリヤード");
	DrawGameObjects();

	if (nine_->IsClicked()) {
		child_ = new FadeOut(30, new GameMain(NINE_BALL));
		return this;
	}
	if (eight_->IsClicked()) {
		child_ = new FadeOut(30, new GameMain(EIGHT_BALL));
		return this;
	}
	if (exit_->IsClicked()) {
		return new Exit();
	}

	return this;
}


// メイン
GameMain::GameMain(GameMode mode) : 
	Scene(1), mode_(mode), reset_count_(0), update_while_child_scene_(false)
{
}

void GameMain::Init()
{
	table_ = new Table({ 0.f, -9.4f, 0.f });
	Ball* b0 = nullptr;
	if (mode_ == NINE_BALL) {
		b0 = table_->SetupNineBall();
	} else if (mode_ == EIGHT_BALL) {
		b0 = table_->SetupEightBall();
	}
	assert(b0);
	player_ = new Player(b0, table_);
	player_->SetCueVisible(false);
	menu_btn_ = new Button(580, 10, 50, 30, "MENU");

	child_ = new FadeIn(30, new GameStart());
	child_->Init();

	Sound::GetInstance().Play("stage_bgm");
}

Scene* GameMain::Update()
{
	// 子シーンがあればそちらを実行
	if (child_) {
		if (update_while_child_scene_) UpdateGameObjects();
		DrawGameObjects();
		return UpdateChild();
	}

	// ポーズ
	if (menu_btn_->IsMouseOver()) {
		if (menu_btn_->IsClicked()) {
			DrawGameObjects();
			child_ = new Pause(mode_, player_);
			return this;
		}
		MouseInput::GetInstance().SetValid(false);
	}

	// 更新
	UpdateGameObjects();
	//描画
	DrawGameObjects();

	// ゲームオーバー
	if (table_->GetBallState(0) == 0) {
		update_while_child_scene_ = true;
		child_ = new GameOver(this);
		return this;
	}
	// ゲームクリア
	if (IsFallingAllTargetBall()) {
		player_->SetGameClear();
		update_while_child_scene_ = true;
		child_ = new GameClear(reset_count_, player_->GetTurn());
		return this;
	}

	return this;
}

void GameMain::ResetCueBall()
{
	Ball* ball;
	Ball* b0 = nullptr;
	vec3f reset_pos = table_->GetPos() + vec3f(-table_->GetWidth(), table_->GetHeight(), 0.f);
	bool is_hit = false;
	do {
		for (auto o : objects_) {
			if (o->GetType() == BALL) {
				assert(dynamic_cast<Ball*>(o));
				ball = static_cast<Ball*>(o); // Ball型確定なのでstatic_castでいい
				if (ball->GetNumber() == 0) {
					b0 = ball;
					continue;
				}
				if (ball->IsFalling()) continue;
				// 設置場所が他の球と重なってたらずらす
				if ((ball->GetPos() - reset_pos).LengthSq() < ball->GetRadius()*ball->GetRadius()) {
					reset_pos.x += ball->GetRadius();
					is_hit = true;
					break;
				}
			}
		}
	} while (is_hit);
	assert(b0);
	b0->Reset(reset_pos);
	++reset_count_;
	update_while_child_scene_ = false;
}

void GameMain::UpdateGameObjects()
{
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
	// 座標更新
	for (auto o : objects_) {
		o->UpdatePosition();
	}
}

bool GameMain::IsFallingAllTargetBall() const
{
	for (int i = 1; i < 16; ++i) {
		if (table_->GetBallState(i) == -1) break;
		if (table_->GetBallState(i) == 1) return false;
	}
	return true;
}


// フェードイン
FadeIn::FadeIn(int time, Scene* next) : 
	Scene(2), time_(time), next_(next), bright_(255), fade_rate_(255 / time)
{
	assert(time_ >= 0);
}

FadeIn::~FadeIn()
{
	if (time_ > 0 && next_) delete next_;
}

Scene* FadeIn::Update()
{
	bright_ -= fade_rate_;
	auto screen_size = Camera3D::GetScreenSize();
	Graphics2D::DrawRect(0, 0, screen_size.x, screen_size.y, 0, true, bright_);
	Graphics2D::Update();
	if (time_-- == 0) return next_;
	return this;
}

// フェードアウト
FadeOut::FadeOut(int time, Scene* next) :
	Scene(2), time_(time), next_(next), fade_rate_(255 / time), bright_(fade_rate_)
{
	assert(time_ >= 0);
}

FadeOut::~FadeOut()
{
	if (time_ > 0 && next_) delete next_;
}

Scene* FadeOut::Update()
{
	bright_ += fade_rate_;
	if (time_ == 0) bright_ = 255;
	auto screen_size = Camera3D::GetScreenSize();
	Graphics2D::DrawRect(0, 0, screen_size.x, screen_size.y, 0, true, bright_);
	Graphics2D::Update();
	if (time_-- == 0) return next_;
	return this;
}

// ポーズ
Pause::Pause(GameMode mode, Player* p) : Scene(2), menu_(new PauseMenu(p)), mode_(mode)
{
}

Scene* Pause::Update()
{
	if (menu_->PushClose()) return nullptr;
	if (menu_->PushRestart()) {
		Sound::GetInstance().SetFadeOut(60);
		return new FadeOut(60, new GameMain(mode_));
	}
	if (menu_->PushTitle()) {
		Sound::GetInstance().SetFadeOut(60);
		return new FadeOut(60, new Title(30));
	}

	UpdateGameObjects();

	auto screen_size = Camera3D::GetScreenSize();
	Graphics2D::DrawRect(0, 0, screen_size.x, screen_size.y, 0, true, 128);
	Graphics2D::Update();
	DrawGameObjects();

	return this;
}


// ゲームスタート
GameStart::GameStart() : Scene(2), state_(0), state_count_(0)
{
}

Scene * GameStart::Update()
{
	char* message = "クリア条件: 全ての的球をポケットに落とす(順番不問)";
	int width = 20;
	auto screen_size = Camera3D::GetScreenSize();
	int ofs_y;
	switch (state_) {
	case 0:
		ofs_y = static_cast<int>(std::sin(PI / 60.f * state_count_) * (screen_size.y / 2 + width));
		Graphics2D::DrawRect(0, -width * 2 + ofs_y, screen_size.x, 0 + ofs_y, 0x000000, true, 128);
		Graphics2D::DrawFontStringCenter(0, screen_size.x / 2, -width + ofs_y, 0xffffff, message);
		if (state_count_ == 30) {
			++state_;
			state_count_ = 0;
		}
		break;
	case 1:
		Graphics2D::DrawRect(0, screen_size.y / 2 - width, screen_size.x, screen_size.y / 2 + width, 0x000000, true, 128);
		Graphics2D::DrawFontStringCenter(0, screen_size.x / 2, screen_size.y / 2, 0xffffff, message);
		if (state_count_ == 180 || MouseInput::GetInstance().LeftDown() || MouseInput::GetInstance().RightDown()) {
			++state_;
			state_count_ = 0;
		}
		break;
	case 2:
		ofs_y = static_cast<int>(std::cos(PI/2.f - PI / 60.f * state_count_) * (screen_size.y / 2 + width));
		Graphics2D::DrawRect(0, screen_size.y / 2 - width + ofs_y, screen_size.x, screen_size.y / 2 + width + ofs_y, 0x000000, true, 128);
		Graphics2D::DrawFontStringCenter(0, screen_size.x / 2, screen_size.y / 2 + ofs_y, 0xffffff, message);
		if (state_count_ == 15) {
			return nullptr;
		}
		break;
	}
	++state_count_;

	Graphics2D::Update();
	UpdateGameObjects();
	DrawGameObjects();

	return this;
}

// ゲームオーバー
GameOver::GameOver(GameMain* parent) : 
	Scene(2), parent_(parent), yes_(nullptr), no_(nullptr), state_(0), state_count_(0)
{
}

Scene* GameOver::Update()
{
	int width = 40;
	auto screen_size = Camera3D::GetScreenSize();
	int ofs_y;
	switch (state_) {
	case 0:
		ofs_y = static_cast<int>(std::sin(PI / 120.f * state_count_) * (screen_size.y / 2 + width));
		Graphics2D::DrawRect(0, -width*2 + ofs_y,
			screen_size.x, 0 + ofs_y, 0x000000, true, 128);
		Graphics2D::DrawStringCenter(screen_size.x / 2, -width * 2 + 10 + ofs_y,
			"GAME OVER", 0xffffff);
		Graphics2D::DrawStringCenter(screen_size.x / 2, -width * 2 + 35 + ofs_y,
			"手球が落下しました。このまま続行しますか？", 0xffffff);
		if (state_count_ == 60) {
			++state_;
			state_count_ = 0;
		}
		break;
	case 1:
		Graphics2D::DrawRect(0, screen_size.y/2 - width, 
			screen_size.x, screen_size.y/2 + width, 0x000000, true, 128);
		Graphics2D::DrawStringCenter(screen_size.x / 2, screen_size.y / 2 - width + 10,
			"GAME OVER", 0xffffff);
		Graphics2D::DrawStringCenter(screen_size.x / 2, screen_size.y / 2 - width + 35, 
			"手球が落下しました。このまま続行しますか？", 0xffffff);
		if (state_count_ == 20) {
			++state_;
			state_count_ = 0;
			yes_ = new StringButton(screen_size.x / 2 - 80, screen_size.y / 2 - width + 50, 0, "はい");
			no_  = new StringButton(screen_size.x / 2 + 80, screen_size.y / 2 - width + 50, 0, "いいえ");
		}
		break;
	case 2:
		Graphics2D::DrawRect(0, screen_size.y / 2 - width,
			screen_size.x, screen_size.y / 2 + width, 0x000000, true, 128);
		Graphics2D::DrawStringCenter(screen_size.x / 2, screen_size.y / 2 - width + 10,
			"GAME OVER", 0xffffff);
		Graphics2D::DrawStringCenter(screen_size.x / 2, screen_size.y / 2 - width + 35,
			"手球が落下しました。このまま続行しますか？", 0xffffff);
		if (yes_->IsClicked()) {
			parent_->ResetCueBall();
			return nullptr;
		} else if(no_->IsClicked()) {
			Sound::GetInstance().SetFadeOut(60);
			return new FadeOut(60, new Title(60));
		}
		break;
	}
	++state_count_;

	Graphics2D::Update();
	UpdateGameObjects();
	DrawGameObjects();

	return this;
}

// ゲームクリア
GameClear::GameClear(int reset, int turn) : 
	Scene(2), state_(0), state_count_(0), reset_(reset), turn_(turn)
{
}

Scene* GameClear::Update()
{
	int width = 60;
	auto screen_size = Camera3D::GetScreenSize();
	int ofs_y;
	std::string result_turn  = "　　使用ターン数: " + std::to_string(turn_);
	std::string result_reset = "手球が落ちた回数: " + std::to_string(reset_);
	switch (state_) {
	case 0:
		ofs_y = static_cast<int>(std::sin(PI / 120.f * state_count_) * (screen_size.y / 2 + width));
		Graphics2D::DrawRect(0, -width * 2 + ofs_y, screen_size.x, 0 + ofs_y, 0x000000, true, 128);
		Graphics2D::DrawFontStringCenter(1, screen_size.x / 2, -width - 30 + ofs_y, 0xffffff, "GAME CLEAR");
		if (state_count_ == 60) {
			++state_;
			state_count_ = 0;
		}
		break;
	case 1:
		Graphics2D::DrawRect(0, screen_size.y / 2 - width, screen_size.x, screen_size.y / 2 + width, 0x000000, true, 128);
		Graphics2D::DrawFontStringCenter(1, screen_size.x / 2, screen_size.y / 2 - 30, 0xffffff, "GAME CLEAR");
		if (state_count_ == 20) {
			++state_;
			state_count_ = 0;
		}
		break;
	case 2:
		Graphics2D::DrawRect(0, screen_size.y / 2 - width, screen_size.x, screen_size.y / 2 + width, 0x000000, true, 128);
		Graphics2D::DrawFontStringCenter(1, screen_size.x / 2, screen_size.y / 2 - 30, 0xffffff, "GAME CLEAR");
		Graphics2D::DrawFontStringCenter(0, screen_size.x / 2, screen_size.y / 2 + 10, 0xffffff, result_turn);
		Graphics2D::DrawFontStringCenter(0, screen_size.x / 2, screen_size.y / 2 + 30, 0xffffff, result_reset);
		if (MouseInput::GetInstance().LeftDown()) {
			Sound::GetInstance().SetFadeOut(90);
			return new FadeOut(90, new Title(60));
		}
		break;
	}
	++state_count_;

	Graphics2D::Update();
	UpdateGameObjects();
	DrawGameObjects();

	return this;
}
