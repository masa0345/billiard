#include "player.h"
#include "ball.h"
#include "table.h"
#include "model.h"
#include "camera.h"
#include "scene.h"
#include "range.h"
#include "image_ball_guide.h"
#include "graphics.h"

#include <DxLib.h>

Player::Player(Ball* ball0, Table* table) : GameObject(PLAYER, vec3f()), ball_(ball0), 
	mouse_(MouseInput::GetInstance()), cue_length_(14.7f), table_height_(table->GetPos().y+table->GetHeight()), 
	cue_visible_(true), camera_target_(), camera_pos_(25.f, PI/4.f, 0.f), 
	state_(3), state_count_(0), shot_power_(0.1f), shot_power_max_(1.5f), display_guide_(true)
{
	cue_ = std::make_shared<Model>("cue/cue_stick.pmd");
	cue_->SetVisible(cue_visible_);
	camera_ = std::make_unique<Camera3D>();
	camera_->SetViewTransform(camera_pos_.ToVec3f()+camera_target_, camera_target_);
}

void Player::Update()
{
	switch (state_) {
	case 0: // �L���[�𓮂���
		Graphics2D::DrawString(0,  0, "���N���b�N�F��������", 0xffffff);
		Graphics2D::DrawString(0, 16, "�E�N���b�N�F�J�����ړ�", 0xffffff);
		Graphics2D::DrawString(0, 32, "�z�C�[���@�F�Y�[��", 0xffffff);
		CameraUpdate();
		CueUpdate();
		if (display_guide_) {
			vec3f shot_dir = cue_dir_;
			shot_dir.y = 0.f;
			new ImageBallGuide(ball_->GetPos(), shot_dir.Normalize());
		}
		// ���N���b�N�ŃL���[�̕�������
		if (mouse_.LeftDown()) {
			state_ = 1;
		}
		break;
	case 1: // �V���b�g�̋��������߂�
		Graphics2D::DrawString(0, 0, "���N���b�N�F�V���b�g", 0xffffff);
		Graphics2D::DrawString(0, 16, "�E�N���b�N�F�L�����Z��", 0xffffff);
		// ������0.1�`shot_power_max_�̊Ԃŕϓ�
		shot_power_ = 0.1f + (1.f - std::cos(PI / 60.f * state_count_)) * 0.5f * shot_power_max_;
		// ���N���b�N�ŃV���b�g
		if (mouse_.LeftDown()) {
			vec3f shot_dir = cue_dir_;
			shot_dir.y = 0.f;
			ball_->SetVel(shot_dir.Normalize()*shot_power_);
			mouse_.UpdateClickRight();
			state_ = 2;
			state_count_ = 0;
			shot_power_ = 0.1f;
			break;
		}
		// �E�N���b�N�ŃL�����Z��
		if (mouse_.RightDown()) {
			mouse_.UpdateClickRight();
			state_ = 0;
			state_count_ = 0;
			shot_power_ = 0.1f;
			break;
		}
		++state_count_;
		break;
	case 2: // ���ړ���
		Graphics2D::DrawString(0, 0, "�E�N���b�N�F�J�����ړ�", 0xffffff);
		Graphics2D::DrawString(0, 16, "�z�C�[���@�F�Y�[��", 0xffffff);
		CameraUpdate();
		if (ball_->IsFalling()) state_ = 4;
		// �S�Ă̋����~�܂��scene���L���[�\����ύX����state=3�ɂȂ�
		break;
	case 3: // �J�������苅�̈ʒu�ɓ�����
		CueUpdate();
		if (state_count_ < 30) {
			CameraMove(std::sin(PI * 0.5f * state_count_ / 30.f));
		} else {
			CameraMove(1.f);
			camera_pos_ += move_pos_;
			move_pos_ = SphericalCoord();
			mouse_.UpdateClickRight();
			state_ = 0;
			state_count_ = 0;
			break;
		}
		++state_count_;
		break;
	case 4: // �苅������
		if (!ball_->IsFalling()) {
			state_ = 2;
			state_count_ = 0;
		}
		break;
	case 5: // �Q�[���N���A
		break;

		/*
	case 6: // �J�������苅�̈ʒu�ɓ�����
		CueUpdate();
		if (state_count_ < 30) {
			CameraMove(std::sin(PI * 0.5f * state_count_ / 30.f));
		} else {
			CameraMove(1.f);
			camera_pos_ += move_pos_;
			move_pos_ = SphericalCoord();
			mouse_.UpdateClickRight();
			state_ = 7;
			state_count_ = 0;
			break;
		}
		++state_count_;
		break;
	case 7: // �L���[�𓮂���
		Graphics2D::DrawString(0, 0, "���N���b�N�F��������", 0xffffff);
		Graphics2D::DrawString(0, 16, "�E�N���b�N�F�J�����ړ�", 0xffffff);
		Graphics2D::DrawString(0, 32, "�z�C�[���@�F�Y�[��", 0xffffff);
		CameraUpdate();
		CueUpdate();
		if (display_guide_) {
			vec3f shot_dir = cue_dir_;
			shot_dir.y = 0.f;
			new ImageBallGuide(ball_->GetPos(), shot_dir.Normalize());
		}
		// ���N���b�N�ŃL���[�̕�������
		if (mouse_.LeftDown()) {
			state_ = 8;
		}
		break;
	case 8: // �V���b�g�̋��������߂�
		Graphics2D::DrawString(0, 0, "���N���b�N�F�V���b�g", 0xffffff);
		Graphics2D::DrawString(0, 16, "�E�N���b�N�F�L�����Z��", 0xffffff);
		// ������0.1�`shot_power_max_�̊Ԃŕϓ�
		shot_power_ = 0.1f + (1.f - std::cos(PI / 60.f * state_count_)) * 0.5f * shot_power_max_ * 3.f;
		// ���N���b�N�ŃV���b�g
		if (mouse_.LeftDown()) {
			vec3f shot_dir = cue_dir_;
			shot_dir.y = 0.f;
			ball_->SetVel(shot_dir.Normalize()*shot_power_);
			mouse_.UpdateClickRight();
			state_ = 2;
			state_count_ = 0;
			shot_power_ = 0.1f;
			break;
		}
		// �E�N���b�N�ŃL�����Z��
		if (mouse_.RightDown()) {
			mouse_.UpdateClickRight();
			state_ = 7;
			state_count_ = 0;
			shot_power_ = 0.1f;
			break;
		}
		++state_count_;
		break;*/
	}

	

	/*
	if (mouse_.LeftDown()) {
		//ball_->SetVel(-vec3f(4.f, 0.f, -15.f).Normalize()*10.4f);
		//ball_->SetVel(vec3f(1.f, 0.f, 0.f).Normalize()*2.0f);
	}
	*/
}

void Player::UpdatePosition()
{
}

void Player::Draw() const
{
	cue_->SetRotation(cue_rot_);
	cue_->SetPosition(ball_->GetPos() - cue_dir_ * (cue_length_ + 0.6f + shot_power_));
	cue_->Draw();
	if (state_ != 3) {
		camera_->SetViewTransform((camera_pos_ + move_pos_).ToVec3f() + camera_target_, camera_target_);
	}
}

void Player::SetCueVisible(bool visible)
{
	if (state_ == 4 || state_ == 5) {
		cue_visible_ = false;
	} else if (!cue_visible_ && visible) {
		cue_visible_ = visible;
		CueUpdate();
		if(state_ == 2) state_ = 3;
	} else {
		cue_visible_ = visible;
	}
	cue_->SetVisible(cue_visible_);
}

bool Player::GetDisplayGuide() const
{
	return display_guide_;
}

void Player::SetDisplayGuide(bool dg)
{
	display_guide_ = dg;
}

void Player::SetGameClear()
{
	state_ = 5;
}

void Player::CueUpdate()
{
	// �}�E�X��3D��ԏ�ł̍��W���v�Z
	Point mouse_pos = mouse_.GetPoint();
	vec3f near_pos = camera_->ConvWorldPos(mouse_pos, 0.f);
	vec3f far_pos = camera_->ConvWorldPos(mouse_pos, 1.f);
	vec3f d = far_pos - near_pos;
	float t = (table_height_ + ball_->GetRadius() - near_pos.y) / d.y;
	vec3f mouse_wpos = near_pos + d * t;

	// �}�E�X���W�Ǝ苅�ʒu����L���[�̈ʒu������
	vec3f ball_pos = ball_->GetPos();
	ball_pos.y = mouse_wpos.y;
	cue_dir_ = (ball_pos - mouse_wpos).Normalize();

	// �L���[�̊p�x���v�Z
	float y_angle = 0.1f * PI;
	cue_dir_.y -= std::sin(y_angle);
	float xz = std::atan2f(cue_dir_.x, cue_dir_.z);
	cue_rot_ = vec3f(PI*0.5f + y_angle, xz, 0.f);
}

void Player::CameraUpdate()
{
	if (mouse_.LeftDown()) {
		camera_pos_ += move_pos_;
		move_pos_ = SphericalCoord();
		mouse_.UpdateClickRight();
	}
	// �E�h���b�N�ŃJ�����ړ�
	if (mouse_.RightDrag()) {
		move_pos_.phi = mouse_.RightDragDistance().x / 60.f;
	} else if (move_pos_.phi != 0.f) {
		camera_pos_ += move_pos_;
		move_pos_ = SphericalCoord();
	}
	// �}�E�X�z�C�[���ŃY�[��
	if (mouse_.GetWheel()) {
		float zoom = 0.75f * mouse_.GetWheel();
		if (Range<float>(3.f, 30.f).Within(camera_pos_.r + zoom)) {
			camera_pos_.r += zoom;
		}
	}
}

void Player::CameraMove(float time)
{
	vec3f t;
	if (time == 1.f) {
		camera_target_ = t = ball_->GetPos();
	} else {
		t = (1.f - time) * camera_target_ + time * ball_->GetPos();
	}
	camera_->SetViewTransform((camera_pos_ + move_pos_).ToVec3f() + t, t);
}

SphericalCoord::SphericalCoord() : r(0.f), theta(0.f), phi(0.f)
{
}

SphericalCoord::SphericalCoord(float r, float theta, float phi) : 
	r(r), theta(theta), phi(phi)
{
}

vec3f SphericalCoord::ToVec3f() const
{
	return vec3f(r * std::sin(-theta) * std::sin(phi), 
				 r * std::cos(-theta),
				 r * std::sin(-theta) * std::cos(phi));
}

const SphericalCoord SphericalCoord::operator+(const SphericalCoord rhs) const
{
	return SphericalCoord(r + rhs.r, theta + rhs.theta, phi + rhs.phi);
}

SphericalCoord& SphericalCoord::operator+=(const SphericalCoord rhs)
{
	r += rhs.r; theta += rhs.theta;  phi += rhs.phi;
	return *this;
}
