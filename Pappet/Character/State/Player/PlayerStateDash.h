#pragma once
#include "Character/StateBase.h"

/// <summary>
/// プレイヤーのダッシュ状態
/// </summary>
class PlayerStateDash : public StateBase
{
public:
	//コンストラクタ
	PlayerStateDash(std::shared_ptr<CharacterBase> chara);
	//デストラクタ
	~PlayerStateDash() override;
	//初期化
	void Init(int md) override;
	//更新
	virtual void Update() override;

	//装備状態からアニメーションを所得する
	std::string GetRunAnim();

private:
	int m_noInputFrame;
};

