#include "PlayerStateJump.h"
#include "Character/CharacterBase.h"
#include "External/CsvLoad.h"
#include "Input/Input.h"
#include "Character/Player.h"

namespace
{
	//ジャンプ力
	constexpr float cJumpPower = 3.0f;

	//ジャンプフレーム数
	constexpr int cJumpFrame = 32;

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

	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), "JumpUp"), true, 1.0f);

	//このステートに入った瞬間ジャンプ力を足す
	auto vel = chara->GetRigidbody()->GetVelocity();
	vel.y += cJumpPower;
	chara->GetRigidbody()->SetVelocity(vel);
	//ジャンプ状態にする
	chara->GetRigidbody()->SetJump(true);
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
	////ジャンプフレームが上昇アニメーションの終了フレーム以上ならジャンプ中状態にする
	//if (m_jumpFrame >= m_pChara.lock()->GetEndAnim() * 0.98f)
	//{
	//	//アニメーション変更
	//	m_pChara.lock()->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData("Player", "Jumping"), true, 0.01f);
	//	//ジャンプフレームを初期化する
	//	m_jumpFrame = 0;
	//	//ジャンプ中状態にする
	//	m_updateFund = &PlayerStateJump::LoopUpdate;
	//}

	////ジャンプフレームを更新する
	//m_jumpFrame++;

	if (m_pChara.lock()->GetEndAnim())
	{
		//アニメーション変更
		m_pChara.lock()->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData("Player", "Jumping"), true, 0.01f);
		//ジャンプフレームを初期化する
		m_jumpFrame = 0;
		//ジャンプ中状態にする
		m_updateFund = &PlayerStateJump::LoopUpdate;
	}

}

/// <summary>
/// ジャンプ中状態
/// </summary>
void PlayerStateJump::LoopUpdate()
{
	//ジャンプフレーム規定数以上なら
	if (m_jumpFrame > cJumpFrame)
	{
		//自身の足元座標と移動速度でカプセルを作る
		auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());
		auto pos = m_pChara.lock()->GetRigidbody()->GetPos();
		auto vel = m_pChara.lock()->GetRigidbody()->GetVelocity();
		auto modelBottomPos = pos;
		modelBottomPos.y -= own->GetCapsuleY();
		auto underPos = modelBottomPos;
		underPos.y -= (own->GetCapsuleRadius() + own->GetCapsuleY()) * 2.0f * (0.4f - vel.y);

		//ステージとのカプセルとで当たり判定を取る
		auto hit = MV1CollCheck_Line(m_stageCol, -1, modelBottomPos.ConversionToVECTOR(), underPos.ConversionToVECTOR());

		//ステージとカプセルが当たっていたらジャンプ下降状態にする
		if (hit.HitFlag)
		{
			//アニメーションを変える
			own->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData("Player", "JumpDown"), true, cLandingAnimSpeed);
			//ジャンプフレームを初期化する
			m_jumpFrame = 0;
			//ジャンプ下降状態にする
			m_updateFund = &PlayerStateJump::DownUpdate;
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

	////ジャンプフレームが上昇アニメーションの終了フレーム以上なら入力に応じてステートを変更する
	//if (m_jumpFrame >= m_pChara.lock()->GetEndAnim() * 0.6f)
	//{
	//	//ジャンプ終了
	//	own->GetRigidbody()->SetJump(false);

	//	//左スティックが入力されていなかったらStateをIdleにする
	//	if (Input::GetInstance().GetInputStick(false).first == 0.0f &&
	//		Input::GetInstance().GetInputStick(false).second == 0.0f)
	//	{
	//		ChangeState(StateKind::Idle);
	//		return;
	//	}

	//	//左スティックが入力されていたらStateをWalkかDashにする
	//	if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
	//		Input::GetInstance().GetInputStick(false).second != 0.0f)
	//	{
	//		//ダッシュボタンが長押しされてたらダッシュ
	//		if (Input::GetInstance().IsPushed("Input_Dash"))
	//		{
	//			ChangeState(StateKind::Dash);
	//			return;
	//		}
	//		//押されていなかったらWalk
	//		else
	//		{
	//			ChangeState(StateKind::Walk);
	//			return;
	//		}
	//	}
	//}

	////ジャンプフレームを更新する
	//m_jumpFrame++;

	if (m_jumpFrame >= 26.0f)
	{

		//ジャンプ終了
		own->GetRigidbody()->SetJump(false);

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
	}

	//ジャンプフレームを更新する
	m_jumpFrame++;
}
