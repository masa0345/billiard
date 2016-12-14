#include "model.h"
#include "matrix4x3.h"
#include <DxLib.h>

namespace 
{
// DxLib‚ÌMATRIX‚É•ÏŠ·
MATRIX ToMATRIX(const Matrix4x3& m) {
	MATRIX r;
	auto a = m.ToArray();
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			r.m[i][j] = a[i*4+j];
		}
	}
	return r;
}

int shadow_handle[3] = { -1, -1, -1 };

}

Model::Model(std::string file_name) : visible_(true)
{
	handle_ = MV1LoadModel(("data/model/" + file_name).c_str());
}

Model::~Model()
{
	MV1DeleteModel(handle_);
}

void Model::SetVisible(bool visible)
{
	visible_ = visible;
}

void Model::SetPosition(const vec3f& pos)
{
	pos_ = pos;
}

void Model::SetRotation(const vec3f& rot)
{
	rot_ = Quaternion(rot);
}

void Model::SetRotation(const vec3f& axis, float angle)
{
	rot_ = Quaternion(axis, angle);
}

void Model::AddRotation(const vec3f& axis, float angle)
{
	rot_ = Quaternion(axis, angle) * rot_;
}

void Model::Draw() const
{
	if (!visible_) return;
	Matrix4x3 mat = Matrix4x3::Rotate(rot_) * Matrix4x3::Translation(pos_);
	MV1SetMatrix(handle_, ToMATRIX(mat));
	MV1DrawModel(handle_);
}

void Model::InitShadowMap()
{
	SetLightDirection({ 1.f, -5.f, 1.f });
	for (int i = 0; i < 2; ++i) {
		shadow_handle[i] = MakeShadowMap(1024, 1024);
		SetShadowMapLightDirection(shadow_handle[i], GetLightDirection());
		SetShadowMapDrawArea(shadow_handle[i], VGet(-10.f, 5.f, -10.f), VGet(10.f, -5.f, 10.f));
	}
}

void Model::DestShadowMap()
{
	for (int i = 0; i < 3; ++i) {
		if (shadow_handle[i] != -1) DeleteShadowMap(shadow_handle[i]);
	}
}

void Model::ShadowMapSetup(int num)
{
	ShadowMap_DrawSetup(shadow_handle[num]);
}

void Model::ShadowMapEnd()
{
	ShadowMap_DrawEnd();
}

void Model::UseShadowMapBegin()
{
	for (int i = 0; i < 3; ++i) {
		if (shadow_handle[i] != -1) SetUseShadowMap(i, shadow_handle[i]);
	}
}

void Model::UseShadowMapEnd()
{
	for (int i = 0; i < 3; ++i) {
		if (shadow_handle[i] != -1) SetUseShadowMap(i, -1);
	}
}
