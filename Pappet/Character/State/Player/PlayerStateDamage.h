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
	~PlayerStateDamage();
	//初期化
	void Init(std::string md) override;
	//更新
	virtual void Update() override;
};

