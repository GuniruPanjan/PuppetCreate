#pragma once
#include "Character/StateBase.h"

/// <summary>
/// プレイヤーの回避状態
/// </summary>
class PlayerStateRoll : public StateBase
{
public:
	//コンストラクタ
	PlayerStateRoll(std::shared_ptr<CharacterBase> chara);
	//デストラクタ
	~PlayerStateRoll() override;
	//初期化
	void Init(int md) override;
	//更新
	virtual void Update() override;

private:
	//回避速度
	float m_avoidanceMove;
	//回避で移動するベクター
	VECTOR m_rollMove;
	//回避の方向
	VECTOR m_rollDirection;
};

