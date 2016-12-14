#include "ball.h"
#include "model.h"
#include "scene.h"
#include "table.h"
#include "range.h"
#include "quaternion.h"
#include "random.h"
#include "sound.h"
#include <cassert>


namespace
{
// 許容する誤差
const float epsilon = 1e-4f;

// ボール同士の衝突時刻を返す　衝突しない場合は-1を返す
float CalcCollisionTimeOfBalls(
	const vec3f& cp1, const vec3f& cp2, const vec3f& cv1, const vec3f& cv2,
	float r, float t) {
	vec3f dp = cp2 - cp1;
	vec3f dv = (cv2 - cv1) * t;
	float rr = 4.f * r * r; // (r + r)^2

	// 同速平行移動
	if (dv.IsZero()) {
		return -1.f;
	}

	// 進行方向的に衝突しない
	float b = dp.Dot(dv);
	if (b > -epsilon) return -1.f;

	// 最初から接触
	if (dp.LengthSq() < rr-epsilon) return 0.f;

	// 2次方程式の解の公式で計算
	float a = dv.LengthSq();
	float c = dp.LengthSq() - rr;
	float judge = b*b - a*c;
	if (judge < 0.f) return -1.f;
	judge = std::sqrtf(judge);
	float col_time = (-b - judge) / a;
	// 0から1の間なら衝突
	if (Range<float>(0.f, 1.f).Within(col_time, epsilon)) {
		return col_time;
	}

	return -1.f;
}

}

// 衝突情報
struct Ball::ColData
{
	ColData(float t, const vec3f& v, Ball* b, bool f) : time_(t), vel_(v), another_(b), fall_(f) {}
	float time_; // 衝突時刻 0.f～1.f
	vec3f vel_; // 衝突後の速度
	Ball* another_; // 衝突相手 ボール以外はnull
	bool fall_; // ポケットに落下
};

// ColData用のイテレータ
class ColDataIterator
{
	struct Elem {
		vec3f pos_, vel_;
		float begin_time_, end_time_;
	};
public:
	ColDataIterator(Ball* b) : ball_(b), counter_(0) {
	}

	void begin() {
		counter_ = 0;
		elem_.pos_ = ball_->pos_;
		elem_.vel_ = ball_->vel_;
		elem_.begin_time_ = 0.f;
		it = ball_->col_.begin();
		if (it != ball_->col_.end()) {
			elem_.end_time_ = it->time_;
			if (elem_.end_time_ == elem_.begin_time_) {
				this->operator++();
			}
		} else {
			elem_.end_time_ = 1.f;
		}
	}
	bool end() {
		return counter_ == ball_->col_.size() + 1;
	}
	void last() {
		begin();
		while (counter_ != ball_->col_.size()) this->operator++();
	}
	Range<float> time_range() const {
		return Range<float>(elem_.begin_time_, elem_.end_time_);
	}
	bool is_fall() const {
		if (ball_->col_.empty()) return false;
		return it->fall_;
	}

	ColDataIterator& operator++() {
		assert(counter_ <= static_cast<int>(ball_->col_.size()));
		++counter_;
		elem_.pos_ += elem_.vel_ * (elem_.end_time_ - elem_.begin_time_);
		if (end()) return *this;
		elem_.vel_ = it->vel_;
		if (counter_ == ball_->col_.size()) {
			elem_.begin_time_ = elem_.end_time_;
			elem_.end_time_ = 1.f;
			return *this;
		}
		++it;
		elem_.begin_time_ = elem_.end_time_;
		elem_.end_time_ = it->time_;
		if (elem_.end_time_ == elem_.begin_time_) {
			return this->operator++();
		}
		return *this;
	}
	Elem& operator*() {
		return elem_;
	}
	Elem* operator->() {
		return &elem_;
	}

private:
	Ball* ball_;
	Elem elem_;
	std::list<Ball::ColData>::iterator it;
	int counter_;
};

Ball::Ball(const vec3f& pos, int num) 
	: GameObject(BALL, pos), radius_(0.36f), number_(num), remove_flag_(false), falling_(0)
{
	pos_.y += radius_;
	model_ = std::make_shared<Model>("ball/" + std::to_string(num) + ".pmd");
	model_->SetPosition(pos_);
	// ランダムに回転
	vec3f rot_axis = vec3f(Random::GetFloat(-1.f, 1.f), Random::GetFloat(0.01f, 1.f), Random::GetFloat(-1.f, 1.f)).Normalize();
	model_->AddRotation(rot_axis, Random::GetFloat(PI2));
}

void Ball::Update()
{
	// 落下中の処理
	if (falling_ > 0) {
		if (falling_ < 5) {
			col_.clear();
			my_scene_->RegisterCollider(this);
			++falling_;
		} else {
			vel_ = vec3f();
			model_->SetVisible(false);
		}
		return;
	}

	// 当たり判定登録
	col_.clear();
	my_scene_->RegisterCollider(this);

	// 摩擦
	float f = 0.001f;
	if (vel_.LengthSq() < f*f) {
		vel_.x = vel_.y = vel_.z = 0.f;
	} else {
		vel_ -= vel_.Normalize() * f;
	}
}

void Ball::UpdatePosition()
{
	ColDataIterator it(this);
	for (it.begin(); !it.end(); ++it) {
	}
	vel_ = it->vel_;
	pos_ = it->pos_;
	model_->SetPosition(pos_);
	if (it.is_fall() && falling_ == 0) {
		falling_ = 1;
		Sound::GetInstance().Play3D("pocket", pos_);
	}
	bool col_ball = false;
	for (auto& e: col_) {
		if (e.another_ != nullptr) {
			col_ball = true;
			break;
		}
	}
	if (col_ball) Sound::GetInstance().Play3D("col", pos_);

	// 回転
	if (falling_ == 0 && !vel_.IsZero()) {
		float rot_angle = vel_.Length() / radius_;
		if (rot_angle > 0.5f) {
			rot_angle = 0.5f;
		}
		vec3f rot_axis = vec3f(vel_.z, vel_.y, -vel_.x).Normalize();
		if (!rot_axis.IsZero()) model_->AddRotation(rot_axis, rot_angle);
	}
}

void Ball::Draw() const
{
	model_->Draw();
}

void Ball::DrawShadowMap() const
{
	Draw();
}

bool Ball::CollideWith(Collidable* c)
{
	return c->Response(this);
}

// ボールとボールの衝突判定
// 移動区間はt=0~1の間で, 衝突地点の情報がColDataに入る
// 区間毎に衝突判定を行う
//    col_の要素が2個でtime_がそれぞれ0.2, 0.6だったら
//    [0.0, 0.2] [0.2, 0.6] [0.6, 1.0] の区間で判定を行う
bool Ball::Response(Ball* ball)
{
	// 落下中は何もしない
	if (falling_ || ball->falling_) return true;
	// 両方停止している場合は何もしない
	if (vel_.IsZero() && col_.empty() && 
		ball->vel_.IsZero() && ball->col_.empty()) return true;
	
	float col_time, min_col_time = 1.f;
	vec3f col_vel1, col_vel2;
	Range<float> r1, r2;
	ColDataIterator it1(this), it2(ball);
	// it1の最後の区間とit2の各区間で判定
	it1.last(); 
	if (it1.is_fall()) return true;
	r1 = it1.time_range();
	for (it2.begin(); !it2.end(); ++it2) {
		if (it1->begin_time_ > it2->end_time_) continue;
		if (it2.is_fall()) break;
		r2 = it2.time_range().Intersect(r1);
		col_time = CalcCollisionTimeOfBalls(it1->pos_, it2->pos_, it1->vel_, it2->vel_, radius_, r2.GetHigh() - r2.GetLow());
		if (col_time > -epsilon) {
			col_time = r2.GetLow() + col_time * (r2.GetHigh() - r2.GetLow());
			// 区間の境界とほぼ同時刻なら同時衝突とする
			if (std::fabsf(r2.GetLow() - col_time) < epsilon) {
				col_time = r2.GetLow();
			} else if (std::fabsf(r2.GetHigh()) < epsilon) {
				col_time = r2.GetHigh();
			}
		}
		// 衝突地点が現在の区間内なら衝突
		if (!r2.Within(col_time, epsilon)) continue;
		// 衝突後の速度を計算
		float e = GetRestitution() * ball->GetRestitution();
		vec3f dp = (ball->pos_ - pos_).Normalize();
		vec3f c = (1.f + e) * 0.5f * (it1->vel_ - it2->vel_).Dot(dp) * dp;
		vec3f cv1 = it1->vel_ - c;
		vec3f cv2 = it2->vel_ + c;
		// 衝突リストに未登録なら追加
		if (min_col_time >= col_time && 
			!Contains(col_time, cv1, ball) && !ball->Contains(col_time, cv2, this)) {
			min_col_time = col_time;
			col_vel1 = cv1;
			col_vel2 = cv2;
		}
	}

	if (min_col_time < 1.f) {
		AddColList(min_col_time, col_vel1, ball);
		ball->AddColList(min_col_time, col_vel2, this);
		return false;
	}

	return true;
}

// ボールとテーブルの衝突判定
bool Ball::Response(Table* t)
{
	if (falling_) {
		if (!t->IsPocketX(pos_.x + vel_.x)) vel_.x = -vel_.x;
		if (!t->IsPocketZ(pos_.z + vel_.z)) vel_.z = -vel_.z;
		t->SetBallState(number_, 0);
		return true;
	}
	float tw = t->GetWidth();
	float e = t->GetRestitution() * GetRestitution();
	Range<float> t_range(0.f, 1.f);
	float col_time, min_col_time = 1.f;
	vec3f dv, dp, col_vel;
	bool fall = false;
	t->SetBallState(number_, 1);
	ColDataIterator it(this);
	for (it.begin(); !it.end(); ++it) {
		// 現フレームでの残り移動距離
		dv = it->vel_ * (it->end_time_ - it->begin_time_);
		// ボールとテーブルの距離
		dp = it->pos_ - t->GetPos();

		// テーブルの枠までの距離をボールの移動距離で割る
		// その値が0から1の間なら現フレームで衝突
		if (dv.z > 0.f && t_range.Within(col_time = (tw - dp.z - radius_) / dv.z, epsilon)) {
			col_time = it->begin_time_ + col_time * (it->end_time_ - it->begin_time_);
			if (col_time - it->begin_time_ < epsilon) col_time = it->begin_time_;
			vec3f vel = vec3f(it->vel_.x, it->vel_.y, -it->vel_.z)*e;
			if (min_col_time >= col_time && !Contains(col_time, vel, nullptr)) {
				min_col_time = col_time;
				fall = t->IsPocketX(it->pos_.x + it->vel_.x * (col_time - it->begin_time_));
				col_vel = fall ? it->vel_ : vel;
			}
		} else if (dv.z < 0.f && t_range.Within(col_time = (-tw - dp.z + radius_) / dv.z, epsilon)) {
			col_time = it->begin_time_ + col_time * (it->end_time_ - it->begin_time_);
			if (col_time - it->begin_time_ < epsilon) col_time = it->begin_time_;
			vec3f vel = vec3f(it->vel_.x, it->vel_.y, -it->vel_.z)*e;
			if (min_col_time >= col_time && !Contains(col_time, vel, nullptr)) {
				min_col_time = col_time;
				fall = t->IsPocketX(it->pos_.x + it->vel_.x * (col_time - it->begin_time_));
				col_vel = fall ? it->vel_ : vel;
			}
		}
		if (dv.x > 0.f && t_range.Within(col_time = (tw*2.f - dp.x - radius_) / dv.x, epsilon)) {
			col_time = it->begin_time_ + col_time * (it->end_time_ - it->begin_time_);
			if (col_time - it->begin_time_ < epsilon) col_time = it->begin_time_;
			vec3f vel = vec3f(-it->vel_.x, it->vel_.y, it->vel_.z)*e;
			if (min_col_time >= col_time && !Contains(col_time, vel, nullptr)) {
				min_col_time = col_time;
				fall = t->IsPocketZ(it->pos_.z + it->vel_.z * (col_time - it->begin_time_));
				col_vel = fall ? it->vel_ : vel;
			}
		} else if (dv.x < 0.f && t_range.Within(col_time = (-tw*2.f - dp.x + radius_) / dv.x, epsilon)) {
			col_time = it->begin_time_ + col_time * (it->end_time_ - it->begin_time_);
			if (col_time - it->begin_time_ < epsilon) col_time = it->begin_time_;
			vec3f vel = vec3f(-it->vel_.x, it->vel_.y, it->vel_.z)*e;
			if (min_col_time >= col_time && !Contains(col_time, vel, nullptr)) {
				min_col_time = col_time;
				fall = t->IsPocketZ(it->pos_.z + it->vel_.z * (col_time - it->begin_time_));
				col_vel = fall ? it->vel_ : vel;
			}
		}
	}

	if (min_col_time < 1.f) {
		if (fall) {
			// ポケットに落ちる(速度は適当)
			col_vel = col_vel.Normalize() * 0.4f;
			col_vel.y -= radius_ * 0.5f;
		}
		AddColList(min_col_time, col_vel, nullptr, fall);
		return fall;
	}

	return true;
}

bool Ball::IsStatic() const
{
	return vel_.IsZero();
}

float Ball::GetRadius() const
{
	return radius_;
}

int Ball::GetNumber() const
{
	return number_;
}

const vec3f& Ball::GetVel() const
{
	return vel_;
}

void Ball::SetVel(const vec3f& vel)
{
	vel_ = vel;
}

bool Ball::IsFalling() const
{
	return falling_ > 0;
}

void Ball::Reset(const vec3f& pos)
{
	// 再設置
	// posの位置に他のボールが無いか注意
	col_.clear();
	falling_ = 0;
	vel_ = vec3f();
	pos_ = pos;
	pos_.y += radius_;
	model_->SetPosition(pos_);
	model_->SetVisible(true);
}

void Ball::AddColList(float time, const vec3f& vel, Ball* another, bool fall)
{
	vec3f v = vel;
	RemoveColList(time);
	col_.emplace_back(time, v, another, fall);
}

// timeより後で衝突するはずだったデータを削除
void Ball::RemoveColList(float time)
{
	if (col_.empty() || remove_flag_) return;

	remove_flag_ = true;
	for (auto it = col_.begin(); it != col_.end(); ++it) {
		if (it->time_ > time) {
			while (it != col_.end()) {
				auto rm = it->another_;
				it = col_.erase(it);
				if (rm) rm->RemoveColList(time);
				
			}
			break;
		} 
	}
	remove_flag_ = false;
}

bool Ball::Contains(float time, const vec3f& vel, Ball* another)
{
	for (auto& cd : col_) {
		if (cd.time_ == time && cd.vel_ == vel && cd.another_ == another) 
			return true;
	}
	return false;
}
