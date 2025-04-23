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
	//シールドを装備しているかどうか
	m_equipmentShield = chara->GetShield();

	//現在のステートを待機状態にする
	m_nowState = StateKind::Idle;
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), "Idle"), false);
	chara->NotInitAnim(false);
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
void PlayerStateIdle::Init(int md)
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

	//持っているキャラクターベースクラスをプレイヤークラスにキャストする(ダウンキャスト)
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//プレイヤーの速度を0にする
	auto prevVel = own->GetRigidbody()->GetVelocity();
	own->GetRigidbody()->SetVelocity(MyLibrary::LibVec3(0.0f, prevVel.y, 0.0f));

	//休息中とスタミナがない時は動けなくする
	if (!own->GetRest() && !own->GetStaminaBreak())
	{
		//左スティックが入力されていたらStateをWalkかDashにする
		if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
			Input::GetInstance().GetInputStick(false).second != 0.0f)
		{
			//メニューが開かれていなくてダッシュボタンが長押しされてたらダッシュ
			if (Input::GetInstance().IsPushed("Input_Dash") && !own->GetMenu() && own->GetAction())
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

		//メニューが開かていなくてジャンプボタンが押されていたらStateをJumpにする
		if (Input::GetInstance().IsTriggered("Input_Jump") && !own->GetMenu() && own->GetAction())
		{
			ChangeState(StateKind::Jump);
			return;
		}

		//メニューが開かていなくて攻撃ボタンが押されていたらStateを攻撃にする
		if (Input::GetInstance().IsTriggered("Input_Attack") && !own->GetMenu())
		{
			ChangeState(StateKind::Attack);
			return;
		}

		//強攻撃ボタンが押されていたらStateを強攻撃にする
		if (Input::GetInstance().GetIsPushedTriggerButton(true) || Input::GetInstance().GetIsPushedTriggerButton(true))
		{
			//メニューが開かていなかったら
			if (!own->GetMenu())
			{
				ChangeState(StateKind::StrongAttack);
				return;
			}

		}

		//メニューが開かれていなく回避ボタンが押されたらStateを回避にする
		if (Input::GetInstance().IsReleased("Input_Roll") && !own->GetMenu() && own->GetAction())
		{
			ChangeState(StateKind::Roll);
			return;
		}

		//メニューが開かれていなくてアイテムボタンが押されたらアイテムを使用する
		if (Input::GetInstance().IsTriggered("X") && !own->GetMenu())
		{
			ChangeState(StateKind::Item);
			return;
		}

		//メニューが開かれていなくてシールドを装備していた場合ガードボタンを押したらStateをガードにする
		if (Input::GetInstance().IsTriggered("Input_Shield") && m_equipmentShield && !own->GetMenu())
		{
			ChangeState(StateKind::Guard);
			return;
		}
	}
	//スタミナ切れの時は特定のアニメーションだけできる
	else if (!own->GetRest() && own->GetStaminaBreak())
	{
		//左スティックが入力されていたらStateをWalkかにする
		if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
			Input::GetInstance().GetInputStick(false).second != 0.0f)
		{
			ChangeState(StateKind::Walk);
			return;
		}

		//メニューが開かれていなくてアイテムボタンが押されたらアイテムを使用する
		if (Input::GetInstance().IsTriggered("X") && !own->GetMenu())
		{
			ChangeState(StateKind::Item);
			return;
		}

		//メニューが開かれていなくてシールドを装備していた場合ガードボタンを押したらStateをガードにする
		if (Input::GetInstance().IsTriggered("Input_Shield") && m_equipmentShield && !own->GetMenu())
		{
			ChangeState(StateKind::Guard);
			return;
		}
	}
}
