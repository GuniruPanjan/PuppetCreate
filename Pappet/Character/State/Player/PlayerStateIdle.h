#pragma once
#include "Character/StateBase.h"

/// <summary>
/// プレイヤーの待機状態
/// </summary>
class PlayerStateIdle : public StateBase
{
public:
	//コンストラクタ
	PlayerStateIdle(std::shared_ptr<CharacterBase> chara);
	//デストラクタ
	~PlayerStateIdle() override;
	//初期化処理
	void Init(int md) override;
	//更新処理
	virtual void Update() override;
};

