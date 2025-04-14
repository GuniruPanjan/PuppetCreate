#pragma once
#include "Character/StateBase.h"

/// <summary>
/// プレイヤーの死亡状態
/// </summary>
class PlayerStateDeath : public StateBase
{
public:
	//コンストラクタ
	PlayerStateDeath(std::shared_ptr<CharacterBase> chara);
	//デストラクタ
	~PlayerStateDeath() override;
	//初期化
	void Init(int md) override;
	//更新
	virtual void Update() override;

};

