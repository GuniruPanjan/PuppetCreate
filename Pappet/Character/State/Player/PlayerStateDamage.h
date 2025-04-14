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
	//ヒット時のアニメーション変更
	std::string GetHitAnim();
	//ヒット時の判定を決める
	bool SetHit(bool set) { return m_hit = set; }

private:
	//攻撃がプレイヤーにヒットしたかの判定
	bool m_hit = false;
};

