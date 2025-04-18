#pragma once
#include "Character/StateBase.h"

/// <summary>
/// プレイヤーのガード状態
/// </summary>
class PlayerStateGuard : public StateBase
{
public:
	//コンストラクタ
	PlayerStateGuard(std::shared_ptr<CharacterBase> chara);
	//デストラクタ
	~PlayerStateGuard() override;
	//初期化
	void Init (int md) override;
	//更新
	virtual void Update() override;

	//入力方向から歩きアニメーションを取得する
	std::string GetGuardAnim(eDir dir);

private:
	eDir m_dir;
	bool m_run;

};

