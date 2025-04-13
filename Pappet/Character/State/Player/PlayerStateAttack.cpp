#include "PlayerStateAttack.h"
#include "Character/CharacterBase.h"
#include "External/CsvLoad.h"
#include "Input/Input.h"
#include "Character/Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="chara"></param>
PlayerStateAttack::PlayerStateAttack(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara),
	m_attackNumber(0)
{
	//現在のステートを攻撃状態にする
	m_nowState = StateKind::Attack;
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), "Attack1"));
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerStateAttack::~PlayerStateAttack()
{
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="md"></param>
void PlayerStateAttack::Init(std::string md)
{
	m_stageCol = md;
}

/// <summary>
/// 更新
/// </summary>
void PlayerStateAttack::Update()
{

}
