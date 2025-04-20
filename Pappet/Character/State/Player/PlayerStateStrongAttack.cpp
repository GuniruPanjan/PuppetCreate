#include "PlayerStateStrongAttack.h"
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

	//回避での移動距離
	constexpr float cStrongMove1 = 0.5f;
	constexpr float cStrongMove2 = 1.0f;
	constexpr float cStrongMove3 = 4.0f;
	constexpr float cStrongMove4 = 0.0f;

	//回避の方向を一回入れる
	bool cOneAvoidance = false;
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="chara"></param>
PlayerStateStrongAttack::PlayerStateStrongAttack(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara),
	m_move(0.0f),
	m_strongMove()
{
	//ジャンプ状態から遷移してきたら可能にする　
	m_jumping = chara->GetJumpCan();

	//現在のステートを強攻撃状態にする
	m_nowState = StateKind::StrongAttack;

	m_equipmentSword = chara->GetSword();
	m_equipmentFist = chara->GetFist();
	m_equipmentShield = chara->GetShield();

	auto animName = GetStrongAnim();
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), animName), true);
	chara->NotInitAnim(true);
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerStateStrongAttack::~PlayerStateStrongAttack()
{
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="md"></param>
void PlayerStateStrongAttack::Init(int md)
{
	m_stageCol = md;
}

/// <summary>
/// 更新
/// </summary>
void PlayerStateStrongAttack::Update()
{
	//持ち主がプレイヤーかどうかをチェックする
	if (!CheckState()) return;

	//持っているキャラクターベースクラスをプレイヤークラスにキャストする(ダウンキャスト)
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//一回だけ受け付ける
	if (!cOneAvoidance)
	{
		//コントローラーの左スティックの入力を取得
		auto input = Input::GetInstance().GetInputStick(false);

		m_leftX = input.first;
		m_leftZ = input.second;

		cOneAvoidance = true;
	}

	//素手の強攻撃
	if (m_equipmentFist && !m_jumping)
	{
		//強攻撃判定を返す
		own->SetStrongAttack(true);

		//移動方向を決定する
		auto moveDir = VGet(m_leftX, 0.0f, -m_leftZ);
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
		own->SetAngle(atan2f(-moveVec.z, moveVec.x) - DX_PI_F / 2);

		//フレームでの移動
		if (own->GetFrame() >= 0.0f && own->GetFrame() <= 20.0f)
		{
			m_move = cStrongMove1;
		}
		else if (own->GetFrame() >= 20.0f && own->GetFrame() <= 40.0f)
		{
			m_move = cStrongMove2;
		}
		else if (own->GetFrame() >= 40.0f && own->GetFrame() <= 45.0f)
		{
			m_move = -cStrongMove3;
		}
		else
		{
			m_move = cStrongMove4;
		}

		m_strongMove = VScale(VGet(sinf(own->GetAngle()), 0.0f, cosf(own->GetAngle())), m_move);

		//移動速度を決定する
		MyLibrary::LibVec3 prevVelocity = own->GetRigidbody()->GetVelocity();
		MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(-m_strongMove.x, prevVelocity.y, -m_strongMove.z);
		own->GetRigidbody()->SetVelocity(newVelocity);;
	}
	//武器の強攻撃
	else if (m_equipmentSword && !m_jumping)
	{
		//強攻撃判定を返す
		own->SetStrongAttack(true);

		//移動速度を決定する
		MyLibrary::LibVec3 prevVelocity = own->GetRigidbody()->GetVelocity();
		MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(0.0f, prevVelocity.y, 0.0f);
		own->GetRigidbody()->SetVelocity(newVelocity);
	}
	//ジャンプの強攻撃
	else if (m_jumping)
	{
		//ジャンプ攻撃判定を返す
		own->SetJumpAttack(true);

		//移動方向を決定する
		auto moveDir = VGet(m_leftX, 0.0f, -m_leftZ);
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
		own->SetAngle(atan2f(-moveVec.z, moveVec.x) - DX_PI_F / 2);

		//フレームでの移動
		if (own->GetFrame() >= 0.0f && own->GetFrame() <= 20.0f)
		{
			m_move = cStrongMove2;
		}
		else if (own->GetFrame() >= 20.0f && own->GetFrame() <= 30.0f)
		{
			m_move = cStrongMove1;
		}
		else
		{
			m_move = cStrongMove4;
		}


		m_strongMove = VScale(VGet(sinf(own->GetAngle()), 0.0f, cosf(own->GetAngle())), m_move);

		//移動速度を決定する
		MyLibrary::LibVec3 prevVelocity = own->GetRigidbody()->GetVelocity();
		MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(-m_strongMove.x, prevVelocity.y, -m_strongMove.z);
		own->GetRigidbody()->SetVelocity(newVelocity);
	}

	//アニメーションが終了したら
	if (own->GetEndAnim())
	{
		//初期化する
		own->SetStrongAttack(false);
		own->SetJumpAttack(false);
		own->GetRigidbody()->SetJumpNow(false);

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
					m_jumping = false;

					ChangeState(StateKind::Dash);
					return;
				}
				//押されていなかったらwalkにする
				else
				{
					m_jumping = false;

					ChangeState(StateKind::Walk);
					return;
				}

			}

			//左スティックが入力されてなかったらStateをIdleにする
			if (Input::GetInstance().GetInputStick(false).first == 0.0f ||
				Input::GetInstance().GetInputStick(false).second == 0.0f)
			{
				m_jumping = false;

				ChangeState(StateKind::Idle);
				return;
			}

			//ジャンプボタンが押されていたらStateをJumpにする
			if (Input::GetInstance().IsTriggered("Input_Jump"))
			{
				m_jumping = false;

				ChangeState(StateKind::Jump);
				return;
			}

			//攻撃ボタンが押されていたらStateを攻撃にする
			if (Input::GetInstance().IsTriggered("Input_Attack"))
			{
				m_jumping = false;

				ChangeState(StateKind::Attack);
				return;
			}

			//強攻撃ボタンが押されていたらStateを強攻撃にする
			if (Input::GetInstance().GetIsPushedTriggerButton(true) || Input::GetInstance().GetIsPushedTriggerButton(true))
			{
				m_jumping = false;

				ChangeState(StateKind::StrongAttack);
				return;
			}

			//回避ボタンが押されたらStateを回避にする
			if (Input::GetInstance().IsTriggered("Input_Roll"))
			{
				m_jumping = false;

				ChangeState(StateKind::Roll);
				return;
			}

			//アイテムボタンが押されたらアイテムを使用する
			if (Input::GetInstance().IsTriggered("X"))
			{
				m_jumping = false;

				ChangeState(StateKind::Item);
				return;
			}

			//シールドを装備していた場合ガードボタンを押したらStateをガードにする
			if (Input::GetInstance().IsTriggered("Input_Shield") && m_equipmentShield)
			{
				m_jumping = false;

				ChangeState(StateKind::Guard);
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
				m_jumping = false;

				ChangeState(StateKind::Walk);
				return;

			}

			//左スティックが入力されてなかったらStateをIdleにする
			if (Input::GetInstance().GetInputStick(false).first == 0.0f ||
				Input::GetInstance().GetInputStick(false).second == 0.0f)
			{
				m_jumping = false;

				ChangeState(StateKind::Idle);
				return;
			}

			//アイテムボタンが押されたらアイテムを使用する
			if (Input::GetInstance().IsTriggered("X"))
			{
				m_jumping = false;

				ChangeState(StateKind::Item);
				return;
			}

			//シールドを装備していた場合ガードボタンを押したらStateをガードにする
			if (Input::GetInstance().IsTriggered("Input_Shield") && m_equipmentShield)
			{
				m_jumping = false;

				ChangeState(StateKind::Guard);
				return;
			}
		}

		
	}
}

/// <summary>
/// 強攻撃アニメーションを変更する
/// </summary>
/// <returns></returns>
std::string PlayerStateStrongAttack::GetStrongAnim()
{
	//空中での強攻撃
	if (m_jumping)
	{
		return std::string("JumpStrongAttack");
	}
	//素手の強攻撃
	else if (m_equipmentFist)
	{
		return std::string("StrongAttack1");
	}
	//剣の強攻撃
	else if (m_equipmentSword)
	{
		return std::string("StrongAttack2");

	}

}
