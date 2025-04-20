#include "PlayerStateTaking.h"
#include "Character/CharacterBase.h"
#include "External/CsvLoad.h"
#include "Input/Input.h"
#include "Character/Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="chara"></param>
PlayerStateTaking::PlayerStateTaking(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara),
	m_animEnd(false)
{
	m_equipmentShield = chara->GetShield();

	//現在のステートをアイテム取得状態にする
	m_nowState = StateKind::Taking;
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), "Taking"), true);
	chara->NotInitAnim(false);
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerStateTaking::~PlayerStateTaking()
{
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="md"></param>
void PlayerStateTaking::Init(int md)
{
	m_stageCol = md;
}

/// <summary>
/// 更新
/// </summary>
void PlayerStateTaking::Update()
{
	//持ち主がプレイヤーかどうかをチェックする
	if (!CheckState()) return;

	//持っているキャラクターベースクラスをプレイヤークラスにキャストする(ダウンキャスト)
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//プレイヤーの速度を0にする
	auto prevVel = own->GetRigidbody()->GetVelocity();
	own->GetRigidbody()->SetVelocity(MyLibrary::LibVec3(0.0f, prevVel.y, 0.0f));

	if (own->GetFrame() >= 60.0f)
	{
		m_animEnd = true;
	}

	//アニメーションが終了したら
	if (m_animEnd)
	{
		//スタミナ切れじゃなかった場合
		if (!own->GetStaminaBreak())
		{
			//左スティックが入力されていたらStateをWalkにする
			if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
				Input::GetInstance().GetInputStick(false).second != 0.0f)
			{
				//ダッシュボタンが押されたらStateをダッシュにする
				if (Input::GetInstance().IsPushed("Input_Dash"))
				{
					ChangeState(StateKind::Dash);
					m_animEnd = false;
					return;
				}
				//押されていなかったらwalkにする
				else
				{
					ChangeState(StateKind::Walk);
					m_animEnd = false;
					return;
				}

			}

			//左スティックが入力されてなかったらStateをIdleにする
			if (Input::GetInstance().GetInputStick(false).first == 0.0f ||
				Input::GetInstance().GetInputStick(false).second == 0.0f)
			{
				ChangeState(StateKind::Idle);
				m_animEnd = false;
				return;
			}

			//ジャンプボタンが押されていたらStateをJumpにする
			if (Input::GetInstance().IsTriggered("Input_Jump"))
			{
				ChangeState(StateKind::Jump);
				m_animEnd = false;
				return;
			}

			//攻撃ボタンが押されていたらStateを攻撃にする
			if (Input::GetInstance().IsTriggered("Input_Attack"))
			{
				ChangeState(StateKind::Attack);
				m_animEnd = false;
				return;
			}

			//強攻撃ボタンが押されていたらStateを強攻撃にする
			if (Input::GetInstance().GetIsPushedTriggerButton(true) || Input::GetInstance().GetIsPushedTriggerButton(true))
			{
				ChangeState(StateKind::StrongAttack);
				m_animEnd = false;
				return;
			}

			//回避ボタンが押されたらStateを回避にする
			if (Input::GetInstance().IsTriggered("Input_Roll"))
			{
				ChangeState(StateKind::Roll);
				m_animEnd = false;
				return;
			}

			//アイテムボタンが押されたらアイテムを使用する
			if (Input::GetInstance().IsTriggered("X"))
			{
				ChangeState(StateKind::Item);
				m_animEnd = false;
				return;
			}

			//シールドを装備していた場合ガードボタンを押したらStateをガードにする
			if (Input::GetInstance().IsTriggered("Input_Shield") && m_equipmentShield)
			{
				ChangeState(StateKind::Guard);
				m_animEnd = false;
				return;
			}
		}
		//スタミナ切れだった場合
		else
		{
			//左スティックが入力されていたらStateをWalkにする
			if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
				Input::GetInstance().GetInputStick(false).second != 0.0f)
			{
				ChangeState(StateKind::Walk);
				m_animEnd = false;
				return;

			}

			//左スティックが入力されてなかったらStateをIdleにする
			if (Input::GetInstance().GetInputStick(false).first == 0.0f ||
				Input::GetInstance().GetInputStick(false).second == 0.0f)
			{
				ChangeState(StateKind::Idle);
				m_animEnd = false;
				return;
			}

			//アイテムボタンが押されたらアイテムを使用する
			if (Input::GetInstance().IsTriggered("X"))
			{
				ChangeState(StateKind::Item);
				m_animEnd = false;
				return;
			}

			//シールドを装備していた場合ガードボタンを押したらStateをガードにする
			if (Input::GetInstance().IsTriggered("Input_Shield") && m_equipmentShield)
			{
				ChangeState(StateKind::Guard);
				m_animEnd = false;
				return;
			}
		}

		
	}
}
