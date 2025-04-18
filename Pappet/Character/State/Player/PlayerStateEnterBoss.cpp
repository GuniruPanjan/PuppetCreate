#include "PlayerStateEnterBoss.h"
#include "Character/CharacterBase.h"
#include "External/CsvLoad.h"
#include "Input/Input.h"
#include "Character/Player.h"

namespace
{
	//行動での移動距離
	constexpr float cMove = 0.5f;
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="chara"></param>
PlayerStateEnterBoss::PlayerStateEnterBoss(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara),
	m_moveVector()
{
	m_equipmentShield = chara->GetShield();

	//現在のステートをボス部屋に入る状態にする
	m_nowState = StateKind::EnterBoss;
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), "BossEnter"), true);
	chara->NotInitAnim(true);
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerStateEnterBoss::~PlayerStateEnterBoss()
{
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="md"></param>
void PlayerStateEnterBoss::Init(int md)
{
	m_stageCol = md;
}

/// <summary>
/// 更新
/// </summary>
void PlayerStateEnterBoss::Update()
{
	//持ち主がプレイヤーかどうかをチェックする
	if (!CheckState()) return;

	//持っているキャラクターベースクラスをプレイヤークラスにキャストする(ダウンキャスト)
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//ボス部屋に入る行動で移動する距離
	m_moveVector = VScale(VGet(sinf(own->GetAngle()), 0.0f, cosf(own->GetAngle())), cMove);

	//アングルの方向に一定距離移動させたい
	MyLibrary::LibVec3 prevVelocity = own->GetRigidbody()->GetVelocity();
	MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(-m_moveVector.x, prevVelocity.y, -m_moveVector.z);
	own->GetRigidbody()->SetVelocity(newVelocity);

	//アニメーションが終了したら
	if (own->GetEndAnim())
	{
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

		//ガードボタンを押したらStateをガードにする
		if (Input::GetInstance().IsTriggered("Input_Shield") && m_equipmentShield)
		{
			ChangeState(StateKind::Guard);
			return;
		}
	}
}
