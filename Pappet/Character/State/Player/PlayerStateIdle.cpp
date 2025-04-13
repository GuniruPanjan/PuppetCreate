#include "PlayerStateIdle.h"
#include "Character/CharacterBase.h"
#include "External/CsvLoad.h"
#include "Input/Input.h"
#include "Character/Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="chara"></param>
PlayerStateIdle::PlayerStateIdle(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara)
{
	//現在のステートを待機状態にする
	m_nowState = StateKind::Idle;
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), "Idle"));
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerStateIdle::~PlayerStateIdle()
{
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="md"></param>
void PlayerStateIdle::Init(std::string md)
{
	m_stageCol = md;
}

/// <summary>
/// 更新処理
/// </summary>
void PlayerStateIdle::Update()
{
	//持ち主がプレイヤーかどうかをチェックする
	if (!CheckState()) return;

	//左スティックが入力されていたらStateをWalkかDashにする
	if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
		Input::GetInstance().GetInputStick(false).second != 0.0f)
	{
		//ダッシュボタンが長押しされてたらダッシュ
		if (Input::GetInstance().IsPushed("Input_Dash"))
		{
			ChangeState(StateKind::Dash);
			return;
		}
		//押されていなかったらWalk
		else
		{
			ChangeState(StateKind::Walk);
			return;
		}
	}

	//ジャンプボタンが押されていたらStateをJumpにする
	if (Input::GetInstance().IsTriggered("Input_Jump"))
	{
		ChangeState(StateKind::Jump);
		return;
	}

	//攻撃ボタンが押されていたらStateを攻撃にする
	if (Input::GetInstance().IsTriggered("Input_Attack"))
	{
		ChangeState(StateKind::Attack);
		return;
	}

	//強攻撃ボタンが押されていたらStateを強攻撃にする
	if (Input::GetInstance().GetIsPushedTriggerButton(true) || Input::GetInstance().GetIsPushedTriggerButton(true))
	{
		ChangeState(StateKind::StrongAttack);
		return;
	}

	//回避ボタンが押されたらStateを回避にする
	if (Input::GetInstance().IsTriggered("Input_Roll"))
	{
		ChangeState(StateKind::Roll);
		return;
	}

	//アイテムボタンが押されたらアイテムを使用する
	if (Input::GetInstance().IsTriggered("X"))
	{
		ChangeState(StateKind::Item);
		return;
	}

	//持っているキャラクターベースクラスをプレイヤークラスにキャストする(ダウンキャスト)
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//プレイヤーの速度を0にする
	auto prevVel = own->GetRigidbody()->GetVelocity();
	own->GetRigidbody()->SetVelocity(MyLibrary::LibVec3(0.0f, prevVel.y, 0.0f));
}
