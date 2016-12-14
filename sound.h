#pragma once

#include "vector3.h"

// ƒTƒEƒ“ƒh singleton
class Sound
{
public:
	Sound(const Sound&) = delete;
	Sound& operator=(const Sound&) = delete;
	Sound(Sound&&) = delete;
	Sound& operator=(Sound&&) = delete;
	static void Init();
	static void Dest();
	static Sound& GetInstance();

	void Update();
	void Play(char* tag) const;
	void Play3D(char* tag, const vec3f& pos);
	void Stop(char* tag) const;
	void StopAll() const;

	void SetFadeOut(int time);
	void Set3DListener(const vec3f& p, const vec3f& t);

	int GetSEVol() const;
	void SetSEVol(int vol);
	int GetBGMVol() const;
	void SetBGMVol(int vol);

private:
	Sound() = default;
	~Sound() = default;
	static Sound* instance_;

	int se_volume_, bgm_volume_;
	int fade_time_, fade_rate_, fade_vol_;
};