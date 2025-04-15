#pragma once
#include "Character/StateBase.h"

/// <summary>
/// プレイヤーのジャンプ状態
/// </summary>
class PlayerStateJump : public StateBase
{
public:
	//コンストラクタ
	PlayerStateJump(std::shared_ptr<CharacterBase> chara);
	//デストラクタ
	~PlayerStateJump();
	//初期化
	void Init(int md) override;
	//更新
	virtual void Update() override;

private:
	//状態遷移のためのメンバ関数ポインタ
	using UpdateFunc_t = void (PlayerStateJump::*)();
	UpdateFunc_t m_updateFund;

	//ジャンプ上昇状態
	void UpUpdate();
	//ジャンプ中状態
	void LoopUpdate();
	//ジャンプ下降状態
	void DownUpdate();
	//ジャンプフレーム数
	int m_jumpFrame;
};

