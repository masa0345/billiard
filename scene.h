#pragma once

#include <vector>

class GameObject;
class Collidable;
enum ObjectType;

// 全てのシーンの基底クラス
class Scene
{
public:
	explicit Scene(int level);
	virtual ~Scene();
	virtual void Init();
	virtual Scene* Update() = 0;
	unsigned int GetLevel() const;
	void RegisterObject(GameObject* o);
	void RegisterCollider(Collidable* c);

protected:
	virtual void UpdateGameObjects();
	virtual void DrawGameObjects() const;
	Scene* UpdateChild();

	std::vector<GameObject*> objects_;
	std::vector<Collidable*> collider_;
	Scene* child_;
private:
	unsigned int level_; // シーン階層の深さ

public:
	static Scene* GetCurrentScene();
	//static GameObject* RegisterObject(GameObject* o);
	//static void RegisterCollider(Collidable* c);
};

// ルートシーン
class Root : public Scene
{
public:
	Root();
	virtual ~Root() = default;
	virtual Scene* Update() override;
};

// 終了 このシーンを返すとウィンドウが閉じる
class Exit : public Scene
{
public:
	Exit() : Scene(0) {}
	virtual ~Exit() = default;
	virtual Scene* Update() override { return nullptr; }
};


// タイトル
class StringButton;
class Title : public Scene
{
public:
	Title(int fade = 0);
	virtual ~Title() = default;
	virtual void Init() override;
	virtual Scene* Update() override;
private:
	StringButton* nine_;
	StringButton* eight_;
	StringButton* exit_;
};

enum GameMode
{
	NINE_BALL,
	EIGHT_BALL,
};

class Table;
class Player;
class Button;
// メインシーン
class GameMain : public Scene
{
public:
	GameMain(GameMode mode);
	virtual ~GameMain() = default;
	virtual void Init() override;
	virtual Scene* Update() override;
	
	void ResetCueBall();

private:
	virtual void UpdateGameObjects() override;
	bool IsFallingAllTargetBall() const;

	GameMode mode_;
	Table* table_;
	Player* player_;
	Button* menu_btn_;
	bool update_while_child_scene_;
};

// フェードイン
class FadeIn : public Scene
{
public:
	FadeIn(int time, Scene* next = nullptr);
	virtual ~FadeIn();
	virtual Scene* Update() override;
private:
	int time_;
	int bright_, fade_rate_;
	Scene* next_;
};

// フェードアウト
class FadeOut : public Scene
{
public:
	FadeOut(int time, Scene* next = nullptr);
	virtual ~FadeOut();
	virtual Scene* Update() override;
private:
	int time_;
	int bright_, fade_rate_;
	Scene* next_;
};

// ポーズ
class PauseMenu;
class Pause : public Scene
{
public:
	Pause(GameMode mode, Player* p);
	virtual ~Pause() = default;
	virtual Scene* Update() override;
private:
	PauseMenu* menu_;
	GameMode mode_;
};

// ゲームスタート
class GameStart : public Scene
{
public:
	GameStart();
	virtual ~GameStart() = default;
	virtual Scene* Update() override;
private:
	int state_, state_count_;
};

// ゲームオーバー
class GameOver : public Scene
{
public:
	GameOver(GameMain* parent);
	virtual ~GameOver() = default;
	virtual Scene* Update() override;
private:
	GameMain* parent_;
	StringButton* yes_;
	StringButton* no_;
	int state_, state_count_;
};

// ゲームクリア
class GameClear : public Scene
{
public:
	GameClear();
	virtual ~GameClear() = default;
	virtual Scene* Update() override;
private:
	int state_, state_count_;
};