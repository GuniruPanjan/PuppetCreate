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

private:
	int m_noInputFrame;
};

