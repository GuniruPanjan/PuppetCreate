#pragma once
#include "Character/StateBase.h"

class PlayerStateDash : public StateBase
{
public:
	//コンストラクタ
	PlayerStateDash(std::shared_ptr<CharacterBase> chara);
	//デストラクタ
	~PlayerStateDash() override;
	//初期化
	void Init(std::string md) override;
	//更新
	virtual void Update() override;
};

