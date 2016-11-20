#include "model.h"
#include "vector3.h"
#include <DxLib.h>

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

void Model::SetPosition(const vec3f & pos)
{
	MV1SetPosition(handle_, { pos.x, pos.y, pos.z });
}

void Model::SetRotation(const vec3f & rot)
{
	MV1SetRotationXYZ(handle_, { rot.x, rot.y, rot.z });
}

void Model::Draw() const
{
	if (!visible_) return;
	MV1DrawModel(handle_);
}
