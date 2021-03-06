#include "camera.h"
#include "sound.h"
#include <DxLib.h>

const Vector2<int> Camera3D::screen_size_ = Vector2<int>(640, 480);

Camera3D::Camera3D() : near_(1.f), far_(100.f), fov_(PI/3.f)
{
	SetupCamera_Perspective(fov_);
	SetCameraNearFar(near_, far_);
}

void Camera3D::UpdatePosition() const
{
	SetCameraPositionAndTarget_UpVecY(VGet(pos_.x, pos_.y, pos_.z), VGet(target_.x, target_.y, target_.z));
	Sound::GetInstance().Set3DListener(pos_, target_);
}

void Camera3D::SetViewTransform(const vec3f& p, const vec3f& t)
{
	pos_ = p;
	target_ = t;
}

vec3f Camera3D::ConvWorldPos(const Vector2<int>& screen_pos, float z) const
{
	VECTOR v = ConvScreenPosToWorldPos(VGet(static_cast<float>(screen_pos.x), static_cast<float>(screen_pos.y), z));
	return vec3f(v.x, v.y, v.z);
}

Vector2<int> Camera3D::GetScreenSize()
{
	return screen_size_;
}
