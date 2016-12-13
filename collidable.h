#pragma once

class Ball;
class Table;
class ImageBallGuide;

// 衝突判定を行うオブジェクトのインターフェース
class Collidable
{
public:
	// 判定処理が終わったらtrue
	virtual bool CollideWith(Collidable*) = 0;
	virtual bool Response(Ball*) { return true; }
	virtual bool Response(Table*) { return true; }
	virtual bool Response(ImageBallGuide*) { return true; }
	// 停止しているか
	virtual bool IsStatic() const { return true; }
	// 反発係数
	virtual float GetRestitution() const { return 1.f; }
};