#pragma once
#include "Character/StateBase.h"

/// <summary>
/// プレイヤーの歩き状態
/// </summary>
class PlayerStateWalk : public StateBase
{
public:
	//コンストラクタ
	PlayerStateWalk(std::shared_ptr<CharacterBase> chara);
	//デストラクタ
	~PlayerStateWalk() override;
	//初期化
	void Init(int md) override;
	//更新
	virtual void Update() override;

	//入力方向から歩きアニメーションを取得する
	std::string GetWalkAnim(eDir dir);

private:
	eDir m_dir;
	int m_walkCount;
	int m_noInputFrame;
	int m_input;
};

