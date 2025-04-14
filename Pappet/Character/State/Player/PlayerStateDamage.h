#pragma once
#include "Character/StateBase.h"

/// <summary>
/// プレイヤーのダメージ状態
/// </summary>
class PlayerStateDamage : public StateBase
{
public:
	//コンストラクタ
	PlayerStateDamage(std::shared_ptr<CharacterBase> chara);
	//デストラクタ
	~PlayerStateDamage() override;
	//初期化
	void Init(int md) override;
	//更新
	virtual void Update() override;
	//ヒット時のアニメーション変更
	std::string GetHitAnim();
};

