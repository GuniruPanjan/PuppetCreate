#pragma once
#include "Character/StateBase.h"

/// <summary>
/// プレイヤーがボス部屋に入る状態
/// </summary>
class PlayerStateEnterBoss : public StateBase
{
public:
	//コンストラクタ
	PlayerStateEnterBoss(std::shared_ptr<CharacterBase> chara);
	//デストラクタ
	~PlayerStateEnterBoss() override;
	//初期化
	void Init(int md) override;
	//更新
	virtual void Update() override;
private:
	VECTOR m_moveVector;
};

