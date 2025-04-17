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
	~PlayerStateStrongAttack() override;
	//初期化
	void Init (int md) override;
	//更新
	virtual void Update() override;
	//プレイヤーの状況によってアニメーションを所得する
	std::string GetStrongAnim();

private:
	float m_move;
	VECTOR m_strongMove;

};

