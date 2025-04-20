#include "PlayerStateJump.h"
#include "Character/CharacterBase.h"
#include "External/CsvLoad.h"
#include "Input/Input.h"
#include "Character/Player.h"

namespace
{
	//ジャンプ力
	constexpr float cJumpPower = 7.0f;

	//ジャンプフレーム数
	constexpr int cJumpFrame = 26;

	//着地アニメーションの再生速度
	constexpr float cLandingAnimSpeed = 1.0f;
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="chara"></param>
PlayerStateJump::PlayerStateJump(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara),
	m_updateFund(),
	m_jumpFrame(0)
{
	//現在のステートをジャンプ状態にする
	m_nowState = StateKind::Jump;

	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), "JumpUp"), true, 1.0f, false, 22.0f);

	//このステートに入った瞬間ジャンプ力を足す
	auto vel = chara->GetRigidbody()->GetVelocity();
	vel = MyLibrary::LibVec3(vel.x / 2, vel.y + cJumpPower, vel.z / 2);
	chara->GetRigidbody()->SetVelocity(vel);
	//ジャンプ状態にする
	chara->GetRigidbody()->SetJump(true);
	chara->GetRigidbody()->SetJumpNow(true);
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerStateJump::~PlayerStateJump()
{
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="md"></param>
void PlayerStateJump::Init(int md)
{
	m_updateFund = &PlayerStateJump::UpUpdate;

	m_stageCol = md;
}

/// <summary>
/// 更新
/// </summary>
void PlayerStateJump::Update()
{
	//持ち主がプレイヤーかどうかをチェックする
	if (!CheckState()) return;

	//メンバ関数ポインタの更新
	(this->*m_updateFund)();
}

/// <summary>
/// ジャンプ上昇状態
/// </summary>
void PlayerStateJump::UpUpdate()
{
	//ジャンプフレームが上昇したアニメーションの終了フレーム以上ならジャンプ中状態にする
	if (m_jumpFrame >= 5)
	{
		//アニメーション変更
		m_pChara.lock()->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData("Player", "JumpUp"), true, 0.01f, false, 31.0f);
		//ジャンプフレームを初期化する
		m_jumpFrame = 0;
		//ジャンプ中状態にする
		m_updateFund = &PlayerStateJump::LoopUpdate;
	}

	//ジャンプフレームを更新する
	m_jumpFrame++;

}

/// <summary>
/// ジャンプ中状態
/// </summary>
void PlayerStateJump::LoopUpdate()
{
	//自身の足元座標と移動速度でカプセルを作る
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//ジャンプフレーム規定数以上なら
	if (m_jumpFrame > cJumpFrame)
	{
		
		auto pos = m_pChara.lock()->GetRigidbody()->GetPos();
		auto vel = m_pChara.lock()->GetRigidbody()->GetVelocity();
		auto modelBottomPos = pos;
		modelBottomPos.y -= own->GetCapsuleY();
		auto underPos = modelBottomPos;
		underPos.y -= (own->GetCapsuleRadius() + own->GetCapsuleY()) * 2.0f * (0.4f - vel.y);

		//ステージとのカプセルとで当たり判定を取る
		auto hit = MV1CollCheck_Line(m_stageCol, -1, modelBottomPos.ConversionToVECTOR(), underPos.ConversionToVECTOR());

		own->SetStart(31.0f);
		own->SetReset(33.0f);
		own->SetLoop(true);

		//ジャンプ終了
		own->GetRigidbody()->SetJump(false);

		//ステージとカプセルが当たっていたらジャンプ下降状態にする
		if (hit.HitFlag)
		{
			//アニメーション変更
			m_pChara.lock()->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData("Player", "JumpUp"), true, 1.2f, false, 33.0f);
			//ジャンプフレームを初期化する
			m_jumpFrame = 0;
			own->SetStart(0.0f);
			own->SetReset(0.0f);
			//ループしなくする
			own->SetLoop(false);
			//ジャンプ下降状態にする
			m_updateFund = &PlayerStateJump::DownUpdate;
		}
	
	}

	//強攻撃ボタンが押されていたらStateを強攻撃にする
	if (Input::GetInstance().GetIsPushedTriggerButton(true) || Input::GetInstance().GetIsPushedTriggerButton(true))
	{
		//スタミナ切れじゃなかった場合
		if (!own->GetStaminaBreak())
		{
			//ジャンプ終了
			own->GetRigidbody()->SetJump(false);
			//フレームも初期化する
			own->SetStart(0.0f);
			own->SetReset(0.0f);
			//アニメーションループをやめる
			own->SetLoop(false);

			ChangeState(StateKind::StrongAttack);
			return;
		}
	}


	//フレーム更新
	m_jumpFrame++;
}

/// <summary>
/// ジャンプ下降状態
/// </summary>
void PlayerStateJump::DownUpdate()
{
	//ダウンキャスト
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	if (m_jumpFrame >= 15.0f)
	{

		//ジャンプ終了
		own->GetRigidbody()->SetJump(false);
		own->GetRigidbody()->SetJumpNow(false);

		//左スティックが入力されていなかったらStateをIdleにする
		if (Input::GetInstance().GetInputStick(false).first == 0.0f &&
			Input::GetInstance().GetInputStick(false).second == 0.0f)
		{
			ChangeState(StateKind::Idle);
			return;
		}

		//左スティックが入力されていたらStateをWalkかDashにする
		if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
			Input::GetInstance().GetInputStick(false).second != 0.0f)
		{
			//ダッシュボタンが長押しされてたらダッシュ
			if (Input::GetInstance().IsPushed("Input_Dash") && !own->GetStaminaBreak())
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

		//ジャンプボタンを押したらジャンプへ遷移する
		if (Input::GetInstance().IsPushed("Input_Jump") && !own->GetStaminaBreak())
		{
			ChangeState(StateKind::Jump);
			return;
		}
	}
	else
	{
		//強攻撃ボタンが押されていたらStateを強攻撃にする
		if (Input::GetInstance().GetIsPushedTriggerButton(true) || Input::GetInstance().GetIsPushedTriggerButton(true))
		{
			//スタミナ切れじゃなかった場合
			if (!own->GetStaminaBreak())
			{
				ChangeState(StateKind::StrongAttack);
				return;
			}
		}
	}

	//ジャンプフレームを更新する
	m_jumpFrame++;
}
