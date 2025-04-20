#include "PlayerStateDash.h"
#include "Input/Input.h"
#include "Character/CharacterBase.h"
#include "Character/Player.h"
#include "External/CsvLoad.h"

namespace
{
	/*アナログスティックによる移動関連*/
	constexpr float cAnalogRangeMin = 0.0f;		//アナログスティックの入力判定最小範囲
	constexpr float cAnalogRangeMax = 0.8f;		//アナログスティックの入力判定最大範囲
	constexpr float cAnalogInputMax = 1000.0f;	//アナログスティックから入力されるベクトルの最大

	//ダッシュにより代入される速度
	constexpr float cDashSpeed = 4.0f;
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="chara"></param>
PlayerStateDash::PlayerStateDash(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara),
	m_noInputFrame(0)
{
	m_equipmentShield = chara->GetShield();
	m_equipmentState = chara->GetEquipment();

	//現在のステートをダッシュ状態にする
	m_nowState = StateKind::Dash;
	auto animName = GetRunAnim();
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), animName), false);
	chara->NotInitAnim(false);
	//速度を設定する
	chara->SetSpeed(cDashSpeed);
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerStateDash::~PlayerStateDash()
{
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="md"></param>
void PlayerStateDash::Init(int md)
{
	m_stageCol = md;
}

/// <summary>
/// 更新
/// </summary>
void PlayerStateDash::Update()
{
	//持ち主がプレイヤーかどうかをチェックする
	if (!CheckState()) return;

	//持っているキャラクターベースクラスをプレイヤークラスにキャストする
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//休息中とスタミナ切れじゃなかった場合は動けなくする
	if (!own->GetRest() && !own->GetStaminaBreak())
	{
		//左スティックが入力されてなかったらStateをIdleにする
		if (Input::GetInstance().GetInputStick(false).first == 0.0f &&
			Input::GetInstance().GetInputStick(false).second == 0.0f)
		{
			if (m_noInputFrame == 2)
			{
				ChangeState(StateKind::Idle);
				return;
			}

			m_noInputFrame++;
		}
		else
		{
			m_noInputFrame = 0;
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

		//ダッシュボタンが押されてなかったらStateをWalkにする
		if (!Input::GetInstance().IsPushed("Input_Dash"))
		{
			ChangeState(StateKind::Walk);
			return;
		}

		//アイテムボタンが押されたらアイテムを使用する
		if (Input::GetInstance().IsTriggered("X"))
		{
			ChangeState(StateKind::Item);
			return;
		}

		//シールドを装備していた場合ガードボタンを押したらStateをガードにする
		if (Input::GetInstance().IsTriggered("Input_Shield") && m_equipmentShield)
		{
			ChangeState(StateKind::Guard);
			return;
		}
	}
	//休息中だった場合強制的にIdle状態にする
	else if(own->GetRest())
	{
		ChangeState(StateKind::Idle);
		return;
	}
	//スタミナ切れだった場合
	else if (own->GetStaminaBreak())
	{
		//左スティックが入力されてなかったらStateをIdleにする
		if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
			Input::GetInstance().GetInputStick(false).second != 0.0f)
		{
			ChangeState(StateKind::Walk);
			return;
		}
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

		//シールドを装備していた場合ガードボタンを押したらStateをガードにする
		if (Input::GetInstance().IsTriggered("Input_Shield") && m_equipmentShield)
		{
			ChangeState(StateKind::Guard);
			return;
		}
	}

	if (m_noInputFrame == 0)
	{
		//コントローラーの左スティックの入力を取得
		auto input = Input::GetInstance().GetInputStick(false);

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
		//キャラクターのアングルを決める
		own->SetModelAngle(atan2f(-moveVec.z, moveVec.x) - DX_PI_F / 2);

		//移動速度を決定する
		MyLibrary::LibVec3 prevVelocity = own->GetRigidbody()->GetVelocity();
		MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(move.x, prevVelocity.y, move.z);
		own->GetRigidbody()->SetVelocity(newVelocity);
	}
	else
	{
		//プレイヤーの速度を0にする
		auto prevVel = own->GetRigidbody()->GetVelocity();
		own->GetRigidbody()->SetVelocity(MyLibrary::LibVec3(0.0f, prevVel.y, 0.0f));
	}
	
}

std::string PlayerStateDash::GetRunAnim()
{
	//何か装備をしていた場合のアニメーション
	if (m_equipmentState)
	{
		return std::string("ShieldRun");
	}
	//何も装備していない場合のアニメーション
	else
	{
		return std::string("Run");
	}
}
