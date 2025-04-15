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
	~PlayerStateAttack() override;
	//初期化
	void Init(int md) override;
	//更新
	virtual void Update() override;
	//アニメーション変更
	std::string GetAttackAnim();
private:
	float m_leftX, m_leftZ;
	//アニメーションの段階
	int m_attackNumber;
	//入力受付できるかどうか
	bool m_input;
	//アニメーションが段階毎に終わったか
	bool m_endAnim;
	//アタック時動くベクター
	VECTOR m_attackMove;
	//動くスピード
	float m_move;
};

