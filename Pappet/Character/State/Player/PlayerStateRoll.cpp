#include "PlayerStateRoll.h"
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

	constexpr float cAvoidanceSpeed = 0.8f;

	//回避での移動距離
	constexpr float cAvoidanceMove1 = 4.0f;
	constexpr float cAvoidanceMove2 = 1.0f;
	constexpr float cAvoidanceMove3 = 0.2f;
	//回避の方向を一回入れる
	bool cOneAvoidance = false;
	//回避の方向を一度決める
	bool cOneDirection = false;
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="chara"></param>
PlayerStateRoll::PlayerStateRoll(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara),
	m_avoidanceMove(0.0f),
	m_rollMove(),
	m_rollDirection(VGet(0.0f,0.0f,0.0f))
{
	m_equipmentShield = chara->GetShield();

	//現在のステートを回避状態にする
	m_nowState = StateKind::Roll;
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), "Roll"), true, cAvoidanceSpeed);
	chara->NotInitAnim(true);

	//初期化する
	cOneAvoidance = false;
	cOneDirection = false;
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerStateRoll::~PlayerStateRoll()
{
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="md"></param>
void PlayerStateRoll::Init(int md)
{
	m_stageCol = md;
}

/// <summary>
/// 更新
/// </summary>
void PlayerStateRoll::Update()
{
	//持ち主がプレイヤーかどうかをチェックする
	if (!CheckState()) return;

	//持っているキャラクターベースクラスをプレイヤークラスにキャストする
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//アニメーションが終了したら
	if (own->GetEndAnim())
	{
		cOneAvoidance = false;

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
			if (Input::GetInstance().GetInputStick(false).first == 0.0f &&
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

			//ダッシュボタンが押されたらStateをダッシュにする
			if (Input::GetInstance().IsPushed("Input_Dash"))
			{
				ChangeState(StateKind::Dash);
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
		//スタミナ切れだった場合
		else
		{
			//左スティックが入力されていたらStateをWalkにする
			if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
				Input::GetInstance().GetInputStick(false).second != 0.0f)
			{
				ChangeState(StateKind::Walk);
				return;

			}

			//左スティックが入力されてなかったらStateをIdleにする
			if (Input::GetInstance().GetInputStick(false).first == 0.0f &&
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

			//シールドを装備していた場合ガードボタンを押したらStateをガードにする
			if (Input::GetInstance().IsTriggered("Input_Shield") && m_equipmentShield)
			{
				ChangeState(StateKind::Guard);
				return;
			}
		}

		
	}

	//ターゲット状態だと可能
	if (m_targetState && own->GetFrame() <= 3.0f)
	{
		//左スティックが動いていたら
		if (Input::GetInstance().GetInputStick(false).first != 0.0f ||
			Input::GetInstance().GetInputStick(false).second != 0.0f)
		{
			//左スティック入力を受けるつける
			if (!cOneAvoidance)
			{
				//コントローラーの左スティックの入力を取得
				auto input = Input::GetInstance().GetInputStick(false);
				m_leftX = input.first;
				m_leftZ = input.second;

				cOneAvoidance = true;
			}

			//移動方向を決定する
			auto moveDir = VGet(-m_leftX, 0.0f, m_leftZ);
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
		}
	}



	//フレームで移動してフレームで回避する
	if (own->GetFrame() >= 0.0f && own->GetFrame() <= 20.0f)
	{
		m_avoidanceMove = cAvoidanceMove1;
		own->SetAvoidance(true);
	}
	else if (own->GetFrame() >= 20.0f && own->GetFrame() <= 30.0f)
	{
		m_avoidanceMove = cAvoidanceMove2;
		own->SetAvoidance(false);

	}
	else
	{
		m_avoidanceMove = cAvoidanceMove3;
		own->SetAvoidance(false);
	}

	//一回だけ方向を入れる
	if (!cOneDirection)
	{
		m_rollDirection = VGet(sinf(own->GetAngle()), 0.0f, cosf(own->GetAngle()));

		cOneDirection = true;
	}

	m_rollMove = VScale(m_rollDirection, m_avoidanceMove);

	//移動速度を決定する
	MyLibrary::LibVec3 prevVelocity = own->GetRigidbody()->GetVelocity();
	MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(-m_rollMove.x, prevVelocity.y, -m_rollMove.z);
	own->GetRigidbody()->SetVelocity(newVelocity);
}
