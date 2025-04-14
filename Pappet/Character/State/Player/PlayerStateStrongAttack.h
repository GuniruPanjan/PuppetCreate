#pragma once
#include "Character/StateBase.h"

/// <summary>
/// プレイヤーの強攻撃状態
/// </summary>
class PlayerStateStrongAttack : public StateBase
{
public:
	//コンストラクタ
	PlayerStateStrongAttack(std::shared_ptr<CharacterBase> chara);
	//デストラクタ
	~PlayerStateStrongAttack();
	//初期化
	void Init (std::string md) override;
	//更新
	virtual void Update() override;
};

