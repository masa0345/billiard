#pragma once

#include "game_object.h"
#include "mouse_input.h"
#include <memory>

class Ball;
class Camera3D;
class Model;

class Player : public GameObject
{
public:
	Player(Ball* ball0);
	virtual void Update() override;
	virtual void UpdatePosition() override;
	virtual void Draw() const override;

private:
	const float cue_length_;
	MouseInput mouse_;
	std::shared_ptr<Model> cue_;
	Ball* ball_;
	vec3f cue_pos_;
};