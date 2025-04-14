#pragma once
#include "Character/StateBase.h"

/// <summary>
/// プレイヤーの攻撃状態
/// </summary>
class PlayerStateAttack : public StateBase
{
public:
	//コンストラクタ
	PlayerStateAttack(std::shared_ptr<CharacterBase> chara);
	//デストラクタ
	~PlayerStateAttack();
	//初期化
	void Init(std::string md) override;
	//更新
	virtual void Update() override;
	//アニメーション変更
	std::string GetAttackAnim();

	//装備が剣などの場合
	bool SetEquipment(bool set) { return m_equipmentSword = set; }
private:
	float m_leftX, m_leftZ;
	//アニメーションの段階
	int m_attackNumber;
	//どの種類の装備をしているか
	bool m_equipmentSword = false;
	//入力受付できるかどうか
	bool m_input;
	//アニメーションが段階毎に終わったか
	bool m_endAnim;
	//アタック時動くベクター
	MyLibrary::LibVec3 m_attackMove;
	//動くスピード
	float m_move;
};

