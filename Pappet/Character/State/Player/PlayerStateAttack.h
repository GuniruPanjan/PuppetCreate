#pragma once
#include "Character/StateBase.h"

/// <summary>
/// プレイヤーの攻撃状態
/// </summary>
class PlayerStateAttack : public StateBase
{
public:
	//コンストラクタ
	PlayerStateAttack(std::shared_ptr<CharacterBase> chara);
	//デストラクタ
	~PlayerStateAttack();
	//初期化
	void Init(std::string md) override;
	//更新
	virtual void Update() override;

	std::string GetAttackAnim();
private:
	int m_attackNumber;
};

