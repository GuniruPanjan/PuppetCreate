#include "PlayerStateGuard.h"
#include "Character/CharacterBase.h"
#include "External/CsvLoad.h"
#include "Input/Input.h"
#include "Character/Player.h"

namespace
{
	/*アナログスティックによる移動関連*/
	constexpr float cAnalogRangeMin = 0.0f;		//アナログスティックの入力判定最小範囲
	constexpr float cAnalogRangeMax = 0.8f;		//アナログスティックの入力判定最大範囲
	constexpr float cAnalogInputMax = 1000.0f;	//アナログスティックから入力されるベクトルの最大

	constexpr float cWalkAnimSpeed = 0.5f;

	//歩きにより代入される速度
	constexpr float cWalkSpeed = 1.5f;
	//走りにより代入される速度
	constexpr float cDashSpeed = 4.0f;

	//一回だけ行う
	bool cOne = false;
	bool cIdle = false;
	bool cFrame = true;
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="chara"></param>
PlayerStateGuard::PlayerStateGuard(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara),
	m_dir(),
	m_run(false)
{
	//現在のステートを待機状態にする
	m_nowState = StateKind::Guard;
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), "ShieldStart"), true);
	chara->NotInitAnim(true);

	cOne = false;
	cIdle = false;
	cFrame = true;
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
void PlayerStateGuard::Init(int md)
{
	m_stageCol = md;
}

/// <summary>
/// 更新
/// </summary>
void PlayerStateGuard::Update()
{
	//持ち主がプレイヤーかどうかをチェックする
	if (!CheckState()) return;

	//持っているキャラクターベースクラスをプレイヤークラスにキャストする(ダウンキャスト)
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//休息中は動けなくする
	if (!own->GetRest())
	{
		//動いていなかったら
		if (Input::GetInstance().GetInputStick(false).first == 0.0f &&
			Input::GetInstance().GetInputStick(false).second == 0.0f)
		{
			//シールドの構え始めが終わったら
			if (own->GetEndAnim() && !cIdle)
			{
				cOne = true;

				cIdle = true;
			}
		}
		else if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
			Input::GetInstance().GetInputStick(false).second != 0.0f)
		{
			cOne = true;
		}


		//盾を構えた状態で左スティックが入力されていたらシールドを構えた状態にする
		if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
			Input::GetInstance().GetInputStick(false).second != 0.0f)
		{
			//ターゲットしてる状態の時
			if (m_targetState)
			{
				if (m_dir == eDir::Forward || m_dir == eDir::Back)
				{
					own->FrameStateChangeAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
					own->SetGuard(cFrame);
				}
			}
			//ターゲットしていない時
			else
			{
				own->FrameStateChangeAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
				own->SetGuard(cFrame);
			}
		}
		//盾を構えた状態で止まった場合リグによるアニメーション分離をやめる
		else if (Input::GetInstance().GetInputStick(false).first == 0.0f &&
			Input::GetInstance().GetInputStick(false).second == 0.0f)
		{
			own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
			own->SetGuard(cFrame);
		}

		//盾を構えてる状態を外した場合とスタミナ切れじゃなかった場合
		if (!Input::GetInstance().IsPushed("Input_Shield") && !own->GetStaminaBreak())
		{
			//リグアニメーション分離をやめる
			own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
			own->SetGuard(cFrame);

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
			//アイドル状態にする
			else
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
		else if (own->GetStaminaBreak())
		{
			//リグアニメーション分離をやめる
			own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
			own->SetGuard(cFrame);

			//左スティックが入力されていたらStateをWalkかDashにする
			if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
				Input::GetInstance().GetInputStick(false).second != 0.0f)
			{
				ChangeState(StateKind::Walk);
				return;
			}
			//アイドル状態にする
			else
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

		//盾を構えながらジャンプできないようにする
		//ジャンプボタンが押されていたらStateをJumpにする
		if (Input::GetInstance().IsTriggered("Input_Jump"))
		{
			//リグアニメーション分離をやめる
			own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
			own->SetGuard(cFrame);

			ChangeState(StateKind::Jump);
			return;
		}
		//盾を構えながら攻撃できないようにする
		//攻撃ボタンが押されていたらStateを攻撃にする
		else if (Input::GetInstance().IsTriggered("Input_Attack"))
		{
			//リグアニメーション分離をやめる
			own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
			own->SetGuard(cFrame);

			ChangeState(StateKind::Attack);
			return;
		}
		//盾を構えながら攻撃できないようにする
		//強攻撃ボタンが押されていたらStateを強攻撃にする
		else if (Input::GetInstance().GetIsPushedTriggerButton(true) || Input::GetInstance().GetIsPushedTriggerButton(true))
		{
			//リグアニメーション分離をやめる
			own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
			own->SetGuard(cFrame);

			ChangeState(StateKind::StrongAttack);
			return;
		}
		//盾を構えながら回避できないようにする
		//回避ボタンが押されたらStateを回避にする
		else if (Input::GetInstance().IsTriggered("Input_Roll"))
		{
			//リグアニメーション分離をやめる
			own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
			own->SetGuard(cFrame);

			ChangeState(StateKind::Roll);
			return;
		}
		//盾を構えながらアイテムをつかないようにする
		//アイテムボタンが押されたらアイテムを使用する
		else if (Input::GetInstance().IsTriggered("X"))
		{
			//リグアニメーション分離をやめる
			own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
			own->SetGuard(cFrame);

			ChangeState(StateKind::Item);
			return;
		}



		//コントローラーの左スティックの入力を取得
		auto input = Input::GetInstance().GetInputStick(false);
		auto dirLog = m_dir;
		m_dir = GetDirection(input.first, input.second);

		//直前の入力方向と異なるとき
		if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
			Input::GetInstance().GetInputStick(false).second != 0.0f)
		{
			//ダッシュボタンが長押しされてたらダッシュ
			if (Input::GetInstance().IsPushed("Input_Dash"))
			{
				m_run = true;

				//スピードを設定する
				own->SetSpeed(cDashSpeed);
			}
			else
			{
				m_run = false;

				//スピードを設定する
				own->SetSpeed(cWalkSpeed);
			}

			//アニメーションを変更する
			if (m_dir == eDir::Right || m_dir == eDir::BackRight || m_dir == eDir::ForwardRight)
			{
				//ターゲットしている時は
				if (m_targetState)
				{
					//リグアニメーション分離をやめる
					own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
					own->SetGuard(cFrame);
				}
			}
			if (m_dir == eDir::Left || m_dir == eDir::BackLeft || m_dir == eDir::ForwardLeft)
			{
				//ターゲットしている時は
				if (m_targetState)
				{
					//リグアニメーション分離をやめる
					own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
					own->SetGuard(cFrame);
				}
			}

			auto animName = GetGuardAnim(m_dir);
			own->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData("Player", animName), false, cWalkAnimSpeed, m_revese);


			//移動方向を決定する
			auto moveDir = VGet(-input.first, 0.0f, input.second);
			//移動ベクトルの長さを取得する
			float len = VSize(moveDir);

			//ベクトルの長さを0.0～1.0の割合に変換する
			float rate = len / cAnalogInputMax;

			//アナログスティック無効な範囲を除外する(デッドゾーン)
			rate = (rate - cAnalogRangeMin) / (cAnalogRangeMax - cAnalogRangeMin);
			rate = min(rate, 1.0f);
			rate = max(rate, 0.0f);

			//速度が決定できるので移動ベクトルに反映する
			moveDir = VNorm(moveDir);
			float speed = own->GetStatus().s_speed * rate;

			//方向ベクトルと移動力をかけて移動ベクトルを生成する
			auto moveVec = VScale(moveDir, speed);

			//cameraの角度から
			//コントローラーによる移動方向を決定する
			MATRIX mtx = MGetRotY(own->GetCameraAngle() + DX_PI_F);
			moveVec = VTransform(moveVec, mtx);

			//ライブラリのベクターに変換する
			MyLibrary::LibVec3 move = MyLibrary::LibVec3(static_cast<float>(moveVec.x), static_cast<float>(moveVec.y), static_cast<float>(moveVec.z));

			//走ってるときとターゲットしていない時
			if (!m_targetState || m_run)
			{
				//キャラクターのアングルを決める
				own->SetModelAngle(atan2f(-moveVec.z, moveVec.x) - DX_PI_F / 2);
			}


			//移動速度を決定する
			MyLibrary::LibVec3 prevVelocity = own->GetRigidbody()->GetVelocity();
			MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(move.x, prevVelocity.y, move.z);
			own->GetRigidbody()->SetVelocity(newVelocity);

		}
		else if (Input::GetInstance().GetInputStick(false).first == 0.0f &&
			Input::GetInstance().GetInputStick(false).second == 0.0f)
		{

			m_run = false;

			if (cOne)
			{
				own->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData("Player", "ShieldIdle"), true, cWalkAnimSpeed, false);
				own->NotInitAnim(false);

				cOne = false;
			}

			//移動速度を0にする
			MyLibrary::LibVec3 prevVelocity = own->GetRigidbody()->GetVelocity();
			MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(0.0f, prevVelocity.y, 0.0f);
			own->GetRigidbody()->SetVelocity(newVelocity);
		}
	}
	//休息中だった場合強制的にIdle状態にする
	else
	{
		//リグアニメーション分離をやめる
		own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame(), cFrame);
		own->SetGuard(cFrame);

		ChangeState(StateKind::Idle);
		return;
	}

}

std::string PlayerStateGuard::GetGuardAnim(eDir dir)
{
	//ターゲットしてた場合
	if (m_targetState)
	{
		//走ってないとき
		if (!m_run)
		{
			if (dir == eDir::Forward)
			{
				return std::string("ShieldWalk");
				//逆再生するかどうか
				m_revese = false;
			}
			if (dir == eDir::Right || dir == eDir::BackRight || dir == eDir::ForwardRight)
			{
				return std::string("ShieldSideWalk");
				//逆再生するかどうか
				m_revese = true;
			}
			if (dir == eDir::Left || dir == eDir::BackLeft || dir == eDir::ForwardLeft)
			{
				return std::string("ShieldSideWalk");
				//逆再生するかどうか
				m_revese = false;
			}
			else if (dir == eDir::Back)
			{
				return std::string("ShieldWalk");
				//逆再生するかどうか
				m_revese = true;
			}
		}
		//走ってるとき
		else
		{
			return std::string("ShieldRun");
		}
		
	}
	//ターゲットしていない場合
	else
	{
		//走ってる場合
		if (m_run)
		{
			return std::string("ShieldRun");
		}
		//走ってない場合
		else
		{
			return std::string("ShieldWalk");
		}
	}
}
