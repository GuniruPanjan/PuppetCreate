#pragma once
#include "Character/StateBase.h"

/// <summary>
/// プレイヤーのアイテム使用状態
/// </summary>
class PlayerStateItem : public StateBase
{
public:
	//コンストラクタ
	PlayerStateItem(std::shared_ptr<CharacterBase> chara);
	//デストラクタ
	~PlayerStateItem();
	//初期化
	void Init(std::string md) override;
	//更新
	virtual void Update() override;
};

