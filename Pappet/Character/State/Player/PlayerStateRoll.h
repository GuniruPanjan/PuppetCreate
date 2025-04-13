#pragma once
#include "Character/StateBase.h"

/// <summary>
/// プレイヤーの回避状態
/// </summary>
class PlayerStateRoll : public StateBase
{
public:
	//コンストラクタ
	PlayerStateRoll(std::shared_ptr<CharacterBase> chara);
	//デストラクタ
	~PlayerStateRoll();
	//初期化
	void Init(std::string md) override;
	//更新
	virtual void Update() override;

private:
	float m_leftX, m_leftZ;
	float m_avoidanceMove;
	MyLibrary::LibVec3 m_rollMove;
};

