#include "sound.h"
#include <cassert>
#include <unordered_map>
#include <string>
#include <DxLib.h>

Sound* Sound::instance_ = nullptr;

enum class SoundType
{
	SE,
	BGM
};

struct SoundData
{
	SoundData() : handle_(-1), volume_(255), loop_(false), type_(SoundType::SE) {}
	SoundData(char* fname) : volume_(255), loop_(false), type_(SoundType::SE) {
		handle_ = LoadSoundMem(fname);
	}
	SoundData(char* fname, int vol, bool loop, SoundType type) :
		volume_(vol), loop_(loop), type_(type) {
		handle_ = LoadSoundMem(fname);
	}
	int handle_;
	int volume_;
	bool loop_;
	SoundType type_;
};

namespace
{
	std::unordered_map<std::string, SoundData> sound_table;
}

void Sound::Init()
{
	assert(instance_ == nullptr);
	instance_ = new Sound();
	instance_->se_volume_ = 200;
	instance_->bgm_volume_ = 120;
	instance_->fade_rate_ = instance_->fade_time_ = 0;
	instance_->fade_vol_ = 255;

	SetCreate3DSoundFlag(TRUE);
	sound_table.insert({ "col", SoundData("data/sound/se/ball_col.mp3") });
	sound_table["col"].volume_ = 120;
	sound_table.insert({ "pocket", SoundData("data/sound/se/pocket.mp3") });
	sound_table.insert({ "shot", SoundData("data/sound/se/shot.mp3") });
	SetCreate3DSoundFlag(FALSE);
	sound_table.insert({ "stage_bgm", SoundData("data/sound/bgm/dimanche.mp3", 255, true, SoundType::BGM) });
}

void Sound::Dest()
{
	assert(instance_ != nullptr);
	delete instance_;
}

Sound & Sound::GetInstance()
{
	assert(instance_);
	return *instance_;
}

void Sound::Update()
{
	if (fade_time_ > 0) {
		if (fade_time_ == 1) {
			fade_vol_ = 0;
			StopAll();
		} else {
			fade_vol_ -= fade_rate_;
			for (auto e : sound_table) {
				if (CheckSoundMem(e.second.handle_) == 1) {
					int vol = e.second.type_ == SoundType::SE ? se_volume_ : bgm_volume_;
					vol = vol * fade_vol_ * e.second.volume_ / 65025;
					ChangeVolumeSoundMem(vol, e.second.handle_);
				}
			}
		}
		--fade_time_;
	}
}

void Sound::Play(char* tag) const
{
	SoundData& sd = sound_table[tag];
	assert(sd.handle_ != -1);
	int vol = sd.type_ == SoundType::SE ? se_volume_ : bgm_volume_;
	vol = vol * sd.volume_ / 255;
	ChangeVolumeSoundMem(vol, sd.handle_);
	PlaySoundMem(sd.handle_, sd.loop_ ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK);
}

void Sound::Play3D(char* tag, const vec3f& pos)
{
	SoundData& sd = sound_table[tag];
	assert(sd.handle_ != -1);
	Set3DPositionSoundMem(VGet(pos.x, pos.y, pos.z), sd.handle_);
	Set3DRadiusSoundMem(80.f, sd.handle_);
	Set3DPresetReverbParamSoundMem(DX_REVERB_PRESET_STONEROOM, sd.handle_);
	int vol = sd.type_ == SoundType::SE ? se_volume_ : bgm_volume_;
	vol = vol * sd.volume_ / 255;
	ChangeVolumeSoundMem(vol, sd.handle_);
	PlaySoundMem(sd.handle_, sd.loop_ ? DX_PLAYTYPE_LOOP : DX_PLAYTYPE_BACK);
}

void Sound::Stop(char* tag) const
{
	StopSoundMem(sound_table[tag].handle_);
}

void Sound::StopAll() const
{
	for (auto e : sound_table) {
		if (CheckSoundMem(e.second.handle_) == 1) {
			StopSoundMem(e.second.handle_);
		}
	}
}

void Sound::SetFadeOut(int time)
{
	fade_time_ = time;
	fade_rate_ = 255 / time;
	fade_vol_ = 255;
}

void Sound::Set3DListener(const vec3f& p, const vec3f& t)
{
	Set3DSoundListenerPosAndFrontPos_UpVecY(VGet(p.x, p.y, p.z), VGet(t.x, t.y, t.z));
}

int Sound::GetSEVol() const
{
	return se_volume_;
}

void Sound::SetSEVol(int vol)
{
	se_volume_ = vol;
	for (auto e : sound_table) {
		if (e.second.type_ == SoundType::BGM) continue;
		if (CheckSoundMem(e.second.handle_) == 1) {
			ChangeVolumeSoundMem(e.second.volume_ * se_volume_ / 255, e.second.handle_);
		}
	}
}

int Sound::GetBGMVol() const
{
	return bgm_volume_;
}

void Sound::SetBGMVol(int vol)
{
	bgm_volume_ = vol;
	for (auto e : sound_table) {
		if (e.second.type_ == SoundType::SE) continue;
		if (CheckSoundMem(e.second.handle_) == 1) {
			ChangeVolumeSoundMem(e.second.volume_ * bgm_volume_ / 255, e.second.handle_);
		}
	}
}
