#pragma once

#include "game_object.h"
#include "mouse_input.h"
#include <memory>

class Ball;
class Camera3D;
class Model;
class Table;

// 3次元の極座標
struct SphericalCoord {
	SphericalCoord();
	SphericalCoord(float r, float theta, float phi);
	vec3f ToVec3f() const;
	float r, theta, phi;
	const SphericalCoord operator+(const SphericalCoord rhs) const;
	SphericalCoord& operator+=(const SphericalCoord rhs);
};

// プレイヤー
class Player : public GameObject
{
public:
	Player(Ball* ball0, Table* table);
	virtual void Update() override;
	virtual void UpdatePosition() override;
	virtual void Draw() const override;

	void SetCueVisible(bool visible);
	bool GetDisplayGuide() const;
	void SetDisplayGuide(bool dg);
	void SetGameClear();
	int GetTurn() const;

private:
	void CueUpdate();
	void CameraUpdate();
	void CameraMove(float time);

	const float cue_length_;
	const float table_height_;
	MouseInput& mouse_;
	std::unique_ptr<Camera3D> camera_;
	std::shared_ptr<Model> cue_;
	Ball* ball_;
	bool cue_visible_;
	vec3f cue_dir_, cue_rot_;
	float shot_power_, shot_power_max_;
	vec3f camera_target_;
	SphericalCoord camera_pos_, move_pos_;
	bool display_guide_;
	int turn_;
	int state_, state_count_;
};