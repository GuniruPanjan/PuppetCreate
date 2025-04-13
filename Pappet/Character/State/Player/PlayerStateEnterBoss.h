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
	~PlayerStateEnterBoss();
	//初期化
	void Init(std::string md) override;
	//更新
	virtual void Update() override;
private:
	MyLibrary::LibVec3 m_moveVector;
};

