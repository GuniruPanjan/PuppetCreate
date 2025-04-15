#include "PlayerStateDeath.h"
#include "Character/Player.h"
#include "External/CsvLoad.h"
#include "Character/CharacterBase.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="chara"></param>
PlayerStateDeath::PlayerStateDeath(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara)
{
	//現在のステートを死亡状態にする
	m_nowState = StateKind::Death;
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), "Death"), true);
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerStateDeath::~PlayerStateDeath()
{
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="md"></param>
void PlayerStateDeath::Init(int md)
{
	m_stageCol = md;
}

/// <summary>
/// 更新
/// </summary>
void PlayerStateDeath::Update()
{
	//持ち主がプレイヤーかどうかをチェックする
	if (!CheckState()) return;

	//持っているキャラクターベースクラスをプレイヤークラスにキャストする(ダウンキャスト)
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//プレイヤーの速度を0にする
	auto prevVel = own->GetRigidbody()->GetVelocity();
	own->GetRigidbody()->SetVelocity(MyLibrary::LibVec3(0.0f, prevVel.y, 0.0f));
}
