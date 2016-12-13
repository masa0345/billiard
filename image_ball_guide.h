#pragma once

#include "game_object.h"
#include "collidable.h"
#include "vector3.h"
#include <list>

// ƒVƒ‡ƒbƒg‚Ì—\‘ªü
class ImageBallGuide : public GameObject, public Collidable
{
public:
	ImageBallGuide(const vec3f& pos, const vec3f& dir);

	virtual void Update() override;
	virtual void Draw() const override;

	virtual bool CollideWith(Collidable* c) override;
	virtual bool Response(Ball* b) override;
	virtual bool Response(Table* t) override;

private:
	struct BounceData;
	bool IsCollision(const vec3f& bp, const vec3f& p, const vec3f& d, float l);
	
	const float radius_;
	vec3f dir_, bounce_dir_;
	vec3f ball_dir_, ball_pos_;
	float length_;
	std::list<BounceData> wall_bounce_;
	bool fall_;
};