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
