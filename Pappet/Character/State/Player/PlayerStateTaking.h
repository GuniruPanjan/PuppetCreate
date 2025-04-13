#pragma once
#include "Character/StateBase.h"

/// <summary>
/// プレイヤーのアイテム取得状態
/// </summary>
class PlayerStateTaking : public StateBase
{
public:
	//コンストラクタ
	PlayerStateTaking(std::shared_ptr<CharacterBase> chara);
	//デストラクタ
	~PlayerStateTaking();
	//初期化
	void Init (std::string md) override;
	//更新
	virtual void Update() override;
};

