#include "PlayerStateGuard.h"
#include "Character/CharacterBase.h"
#include "External/CsvLoad.h"
#include "Input/Input.h"
#include "Character/Player.h"

namespace
{
	/*アナログスティックによる移動関連*/
	constexpr float cAnalogRangeMin = 0.1f;		//アナログスティックの入力判定最小範囲
	constexpr float cAnalogRangeMax = 0.8f;		//アナログスティックの入力判定最大範囲
	constexpr float cAnalogInputMax = 1000.0f;	//アナログスティックから入力されるベクトルの最大

	constexpr float cWalkAnimSpeed = 0.5f;

	//歩きにより代入される速度
	constexpr float cWalkSpeed = 1.5f;
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="chara"></param>
PlayerStateGuard::PlayerStateGuard(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara),
	m_dir()
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
	//持ち主がプレイヤーかどうかをチェックする
	if (!CheckState()) return;

	//持っているキャラクターベースクラスをプレイヤークラスにキャストする(ダウンキャスト)
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//盾を構えた状態で左スティックが入力されていたらシールドを構えた状態にする
	if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
		Input::GetInstance().GetInputStick(false).second != 0.0f)
	{
		own->FrameStateChangeAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame());
	}
	//盾を構えた状態で止まった場合リグによるアニメーション分離をやめる
	else
	{
		own->FrameEndStateAnim(CsvLoad::GetInstance().GetAnimData(own->GetCharacterName(), "ShieldTransition"), own->GetShieldFrame());
	}

	//盾を構えてる状態を外した場合
	if (!Input::GetInstance().IsPushed("Input_Shield"))
	{
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

	//コントローラーの左スティックの入力を取得
	auto input = Input::GetInstance().GetInputStick(false);
	auto dirLog = m_dir;
	m_dir = GetDirection(input.first, -input.second);

	//直前の入力方向と異なるとき
	if (dirLog != m_dir)
	{
		//アニメーションを変更する
		auto animName = GetGuardAnim(m_dir);
		own->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData("Player", animName), cWalkAnimSpeed, m_revese);
	}

	//移動方向を決定する
	auto moveDir = MyLibrary::LibVec3(input.first, 0.0f, -input.second);
	//移動ベクトルの長さを取得する
	float len = moveDir.Length();

	//ベクトルの長さを0.0～1.0の割合に変換する
	float rate = len / cAnalogInputMax;

	//アナログスティック無効な範囲を除外する(デッドゾーン)
	rate = (rate - cAnalogRangeMin) / (cAnalogRangeMax - cAnalogRangeMin);
	rate = min(rate, 1.0f);
	rate = max(rate, 0.0f);

	//速度が決定できるので移動ベクトルに反映する
	moveDir = moveDir.Normalize();
	float speed = own->GetStatus().s_speed * rate;

	//方向ベクトルと移動力をかけて移動ベクトルを生成する
	auto moveVec = moveDir * speed;

	//cameraの角度から
	//コントローラーによる移動方向を決定する
	MATRIX mtx = MGetRotY(own->GetCameraAngle() + DX_PI_F);
	moveVec.GetVector() = VTransform(moveVec.ConversionToVECTOR(), mtx);

	//ライブラリのベクターに変換する
	MyLibrary::LibVec3 move = MyLibrary::LibVec3(static_cast<float>(moveVec.x), static_cast<float>(moveVec.y), static_cast<float>(moveVec.z));

	//ターゲットしていない時
	if (!m_targetState)
	{
		//キャラクターのアングルを決める
		own->SetAngle(atan2f(-moveVec.z, moveVec.x) - DX_PI_F / 2);
	}


	//移動速度を決定する
	MyLibrary::LibVec3 prevVelocity = own->GetRigidbody()->GetVelocity();
	MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(move.x, prevVelocity.y, move.z);
	own->GetRigidbody()->SetVelocity(newVelocity);

}

std::string PlayerStateGuard::GetGuardAnim(eDir dir)
{
	if (dir == eDir::Forward)
	{
		return std::string("ShieldWalk");
		//逆再生するかどうか
		m_revese = false;
	}
	if (dir == eDir::Right)
	{
		return std::string("ShieldSideWalk");
		//逆再生するかどうか
		m_revese = true;
	}
	else if (dir == eDir::Left)
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

	return std::string();
}
