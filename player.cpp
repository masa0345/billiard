#include "player.h"
#include "ball.h"
#include "model.h"

#include <DxLib.h>

Player::Player(Ball* ball0) : GameObject(PLAYER, vec3f()), ball_(ball0), cue_length_(14.7f)
{
	cue_ = std::make_shared<Model>("cue/cue_stick.pmd");
	//cue_->SetVisible(false);

	// ƒJƒƒ‰Œn‚Ìˆ—‚ð‰¼Ý’u
	SetCameraNearFar(1.f, 100.f);
	SetCameraPositionAndTarget_UpVecY({ 0.f, 20.f, -20.f }, { 0.f, 0.f, 0.f });
}

void Player::Update()
{
	mouse_.ButtonUpdate();

	Point mouse_pos = mouse_.GetPoint();
	cue_pos_ = vec3f(-320.f+mouse_pos.x, 0.f, 240.f-mouse_pos.y).Normalize() * (cue_length_+1.f);
	cue_pos_.y += 5.f;
	vec3f ball_pos = vec3f(0.f, 0.f, 0.f);

	//float yz = std::atan2f(-ball_pos.y + cue_pos_.y, ball_pos.z - cue_pos_.z);
	float xz = std::atan2f(ball_pos.x - cue_pos_.x, ball_pos.z - cue_pos_.z);
	vec3f cue_rot = vec3f(PI*0.5f, xz, 0.f);
	cue_->SetRotation(cue_rot);
	cue_->SetPosition(cue_pos_);
	//DrawLine3D({ 0.f, 5.f, 0.f }, { cue_pos_.x, cue_pos_.y, cue_pos_.z }, 0xffff00);

	if (mouse_.LeftDown()) {
		//ball_->SetVel(-vec3f(4.f, 0.f, -15.f).Normalize()*10.4f);
		//ball_->SetVel(vec3f(1.f, 0.f, 0.f).Normalize()*2.0f);
		vec3f shot_dir = -cue_pos_.Normalize();
		shot_dir.y = 0.f;
		ball_->SetVel(shot_dir*1.5f);
	}
}

void Player::UpdatePosition()
{
}

void Player::Draw() const
{
	cue_->Draw();
}
