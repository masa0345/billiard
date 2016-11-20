#pragma once

template<typename T> struct Vector3;
typedef Vector3<float> vec3f;

#include <string>

// 3Dƒ‚ƒfƒ‹
class Model
{
public:
	explicit Model(std::string file_name);
	virtual ~Model();

	void SetVisible(bool visible);
	void SetPosition(const vec3f& pos);
	void SetRotation(const vec3f& rot);
	void Draw() const;

private:
	int handle_;
	bool visible_;
};