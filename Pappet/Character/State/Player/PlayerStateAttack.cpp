#include "PlayerStateAttack.h"
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

	//攻撃による移動速度
	constexpr float cAttackMove1 = 0.5f;
	constexpr float cAttackMove2 = 0.3f;
	constexpr float cAttackMove3 = 0.2f;
	//攻撃速度
	constexpr float cAttackSpeed = 1.0f;
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="chara"></param>
PlayerStateAttack::PlayerStateAttack(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara),
	m_leftX(0),
	m_leftZ(0),
	m_attackNumber(0),
	m_input(false),
	m_endAnim(false),
	m_attackMove(),
	m_move(0.0f)
{
	//装備判定を返す
	m_equipmentSword = chara->GetSword();
	m_equipmentState = chara->GetEquipment();
	//現在のステートを攻撃状態にする
	m_nowState = StateKind::Attack;
	auto animName = GetAttackAnim();
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), animName), true, cAttackSpeed);
	chara->NotInitAnim(false);
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerStateAttack::~PlayerStateAttack()
{
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="md"></param>
void PlayerStateAttack::Init(int md)
{
	m_stageCol = md;
}

/// <summary>
/// 更新
/// </summary>
void PlayerStateAttack::Update()
{
	//持ち主がプレイヤーかどうかをチェックする
	if (!CheckState()) return;

	//持っているキャラクターベースクラスをプレイヤークラスにキャストする
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//装備が剣などの場合
	if (m_equipmentSword)
	{
		//攻撃一段階目
		if (own->GetFrame() <= 40.0f)
		{
			//入力受付時間
			if (own->GetFrame() == 0.0f) m_input = true;
			if (own->GetFrame() == 5.0f) m_input = false;

			//攻撃入力時間
			if (own->GetFrame() >= 20.0f)
			{
				//Rを入力すると次の攻撃につながる
				if (Input::GetInstance().IsTriggered("Input_Attack"))
				{
					m_attackNumber = 1;
				}
			}

			//攻撃による移動速度設定
			if (own->GetFrame() <= 25.0f) m_move = 0.0f;
			if (own->GetFrame() >= 25.0f && own->GetFrame() < 35.0f) m_move = cAttackMove1;
			if (own->GetFrame() >= 35.0f && own->GetFrame() < 40.0f) m_move = cAttackMove2;
			if (own->GetFrame() == 40.0f) m_move = 0.0f;

			//攻撃一段階目終了
			if (own->GetFrame() >= 38.0f && m_attackNumber != 1)
			{
				m_endAnim = true;
			}
		}
		//攻撃二段階目
		else if (own->GetFrame() <= 70.0f && own->GetFrame() > 40.0f && m_attackNumber == 1)
		{
			//入力受付時間
			if (own->GetFrame() == 40.0f) m_input = true;
			if (own->GetFrame() == 45.0f) m_input = false;

			//攻撃入力時間
			if (own->GetFrame() >= 50.0f)
			{
				//Rを入力すると次の攻撃につながる
				if (Input::GetInstance().IsTriggered("Input_Attack"))
				{
					m_attackNumber = 2;
				}
			}

			//攻撃による移動速度設定
			if (own->GetFrame() <= 55.0F) m_move = 0.0f;
			if (own->GetFrame() >= 55.0f && own->GetFrame() <= 65.0f) m_move = cAttackMove1;
			if (own->GetFrame() >= 65.0f && own->GetFrame() <= 70.0f) m_move = cAttackMove3;
			if (own->GetFrame() == 70.0f) m_move = 0.0f;

			//攻撃二段回目終了
			if (own->GetFrame() >= 68.0f && m_attackNumber != 2)
			{
				m_endAnim = true;
			}
		}
		//攻撃三段階目
		else if (own->GetFrame() <= 112.0f && own->GetFrame() > 70.0f && m_attackNumber == 2)
		{
			//入力受付時間
			if (own->GetFrame() == 70.0f) m_input = true;
			if (own->GetFrame() == 75.0f) m_input = false;

			//攻撃による移動速度設定
			if (own->GetFrame() <= 85.0f) m_move = 0.0f;
			if (own->GetFrame() >= 85.0f && own->GetFrame() <= 95.0f) m_move = cAttackMove1;
			if (own->GetFrame() >= 95.0f && own->GetFrame() <= 110.0f) m_move = cAttackMove3;
			if (own->GetFrame() == 110.0f) m_move = 0.0f;

			//攻撃三段回目終了
			if (own->GetFrame() >= 110.0f)
			{
				m_endAnim = true;
			}
		}
	}

	//入力受付
	if (m_input)
	{
		//コントローラーの左スティックの入力を取得
		auto input = Input::GetInstance().GetInputStick(false);

		m_leftX = input.first;
		m_leftZ = input.second;
	}

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

	m_attackMove = VScale(VGet(sinf(own->GetAngle()), 0.0f, cosf(own->GetAngle())), m_move);

	//移動速度を決定する
	MyLibrary::LibVec3 prevVelocity = own->GetRigidbody()->GetVelocity();
	MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(-m_attackMove.x, prevVelocity.y, -m_attackMove.z);
	own->GetRigidbody()->SetVelocity(newVelocity);

	//アニメーション終了後
	if (m_endAnim)
	{
		int a = 1;

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
		if (Input::GetInstance().IsTriggered("Input_Shield"))
		{
			ChangeState(StateKind::Guard);
			return;
		}
	}

}

/// <summary>
/// 攻撃アニメ更新
/// </summary>
/// <returns></returns>
std::string PlayerStateAttack::GetAttackAnim()
{
	//装備が剣などの場合
	if (m_equipmentSword)
	{
		//攻撃
		return std::string("Attack1");
	}
}
