#include "PlayerStateDamage.h"
#include "Character/CharacterBase.h"
#include "External/CsvLoad.h"
#include "Input/Input.h"
#include "Character/Player.h"


namespace
{
	bool cFrame = false;
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="chara"></param>
PlayerStateDamage::PlayerStateDamage(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara)
{
	m_equipmentShield = chara->GetShield();

	//リグが分離された状態できたら
	cFrame = chara->GetGuard();

	//最初にヒット状態を入れる
	m_hit = chara->GetHit();
	//現在のステートを攻撃ヒット状態にする
	m_nowState = StateKind::Damage;
	auto animName = GetHitAnim();
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), animName), true);
	chara->NotInitAnim(false);
	chara->GetRigidbody()->SetJump(false);
	chara->GetRigidbody()->SetJumpNow(false);
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerStateDamage::~PlayerStateDamage()
{
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="md"></param>
void PlayerStateDamage::Init(int md)
{
	m_stageCol = md;
}

/// <summary>
/// 更新
/// </summary>
void PlayerStateDamage::Update()
{
	//持ち主がプレイヤーかどうかをチェックする
	if (!CheckState()) return;

	//持っているキャラクターベースクラスをプレイヤークラスにキャストする(ダウンキャスト)
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//リグ分離終了
	own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);

	//フレーム回避終了
	own->SetAvoidance(false);

	//攻撃などの判定を強制終了する
	own->SetJumpAttack(false);
	own->SetStrongAttack(false);

	//プレイヤーの速度を0にする
	auto prevVel = own->GetRigidbody()->GetVelocity();
	own->GetRigidbody()->SetVelocity(MyLibrary::LibVec3(0.0f, prevVel.y, 0.0f));

	//アニメーションが終了したら
	if (own->GetEndAnim())
	{
		//スタミナ切れじゃなかった場合
		if (!own->GetStaminaBreak())
		{
			//シールドを装備していた場合ガードボタンを押したらStateをガードにする
			if (m_equipmentShield)
			{
				//ガードボタンを押したときか押し続けていた場合
				if (Input::GetInstance().IsTriggered("Input_Shield") || Input::GetInstance().IsPushed("Input_Shield"))
				{
					ChangeState(StateKind::Guard);
					return;
				}
			}

			//左スティックが入力されていたらStateをWalkにする
			if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
				Input::GetInstance().GetInputStick(false).second != 0.0f)
			{
				//ダッシュボタンが押されたらStateをダッシュにする
				if (Input::GetInstance().IsPushed("Input_Dash"))
				{
					ChangeState(StateKind::Dash);
					return;
				}
				//押されていなかったらwalkにする
				else
				{
					ChangeState(StateKind::Walk);
					return;
				}

			}

			//左スティックが入力されてなかったらStateをIdleにする
			if (Input::GetInstance().GetInputStick(false).first == 0.0f ||
				Input::GetInstance().GetInputStick(false).second == 0.0f)
			{
				ChangeState(StateKind::Idle);
				return;
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
		}
		//スタミナ切れだった場合
		else
		{
			//シールドを装備していた場合ガードボタンを押したらStateをガードにする
			if (m_equipmentShield)
			{
				//ガードボタンを押したときか押し続けていた場合
				if (Input::GetInstance().IsTriggered("Input_Shield") || Input::GetInstance().IsPushed("Input_Shield"))
				{
					ChangeState(StateKind::Guard);
					return;
				}
			}

			//左スティックが入力されていたらStateをWalkにする
			if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
				Input::GetInstance().GetInputStick(false).second != 0.0f)
			{
				ChangeState(StateKind::Walk);
				return;

			}

			//左スティックが入力されてなかったらStateをIdleにする
			if (Input::GetInstance().GetInputStick(false).first == 0.0f ||
				Input::GetInstance().GetInputStick(false).second == 0.0f)
			{
				ChangeState(StateKind::Idle);
				return;
			}

			//アイテムボタンが押されたらアイテムを使用する
			if (Input::GetInstance().IsTriggered("X"))
			{
				ChangeState(StateKind::Item);
				return;
			}
		}

		
	}
}

std::string PlayerStateDamage::GetHitAnim()
{
	if (m_hit)
	{
		return std::string("Hit");
	}
	else
	{
		return std::string("ShieldImpact");
	}
}
