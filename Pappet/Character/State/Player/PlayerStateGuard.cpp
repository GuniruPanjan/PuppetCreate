#include "PlayerStateGuard.h"
#include "Character/CharacterBase.h"
#include "External/CsvLoad.h"
#include "Input/Input.h"
#include "Character/Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="chara"></param>
PlayerStateGuard::PlayerStateGuard(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara)
{
	//現在のステートを待機状態にする
	m_nowState = StateKind::Guard;
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), "ShieldStart"));
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerStateGuard::~PlayerStateGuard()
{
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="md"></param>
void PlayerStateGuard::Init(std::string md)
{
	m_stageCol = md;
}

/// <summary>
/// 更新
/// </summary>
void PlayerStateGuard::Update()
{

}

std::string PlayerStateGuard::GetGuardAnim(eDir dir)
{
	return std::string();
}
