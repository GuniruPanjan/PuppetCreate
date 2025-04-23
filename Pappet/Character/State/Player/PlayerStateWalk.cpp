#include "PlayerStateWalk.h"
#include "Input/Input.h"
#include "Character/Player.h"

#include "External/CsvLoad.h"


namespace
{
	/*アナログスティックによる移動関連*/
	constexpr float cAnalogRangeMin = 0.0f;		//アナログスティックの入力判定最小範囲
	constexpr float cAnalogRangeMax = 0.8f;		//アナログスティックの入力判定最大範囲
	constexpr float cAnalogInputMax = 1000.0f;	//アナログスティックから入力されるベクトルの最大

	constexpr float cWalkAnimSpeed = 0.5f;

	//歩きにより代入される速度
	constexpr float cWalkSpeed = 1.5f;

	//インプットのxyを格納する
	float cInputX = 0.0f;
	float cInputY = 0.0f;
}

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="chara"></param>
PlayerStateWalk::PlayerStateWalk(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara),
	m_dir(),
	m_walkCount(0),
	m_noInputFrame(0),
	m_input(0)
{
	//現在のステートを歩き状態にする
	m_nowState = StateKind::Walk;

	//このシーンに遷移した瞬間の左スティックの入力角度を取得しておく
	auto input = Input::GetInstance().GetInputStick(false);
	m_dir = GetDirection(input.first, -input.second);

	m_equipmentState = chara->GetEquipment();
	m_equipmentShield = chara->GetShield();

	auto animName = GetWalkAnim(m_dir);
	chara->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), animName), false);
	chara->NotInitAnim(false);
	//速度を決める
	chara->SetSpeed(cWalkSpeed);
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerStateWalk::~PlayerStateWalk()
{
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="md"></param>
void PlayerStateWalk::Init(int md)
{
	m_stageCol = md;
}

/// <summary>
/// 更新
/// </summary>
void PlayerStateWalk::Update()
{
	//持ち主がプレイヤーかどうかチェックする
	if (!CheckState()) return;

	//ダウンキャスト
	auto own = std::dynamic_pointer_cast<Player>(m_pChara.lock());

	//休息中かスタミナ切れだった場合は動けなくする
	if (!own->GetRest() && !own->GetStaminaBreak())
	{
		//左スティックが入力されていなかったらStateをIdleにする
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

		//メニューが開かれていなくてジャンプボタンが押されていたらStateをJumpにする
		if (Input::GetInstance().IsTriggered("Input_Jump") && !own->GetMenu() && own->GetAction())
		{
			ChangeState(StateKind::Jump);
			return;
		}

		//メニューが開かれていなくて攻撃ボタンが押されていたらStateを攻撃にする
		if (Input::GetInstance().IsTriggered("Input_Attack") && !own->GetMenu())
		{
			ChangeState(StateKind::Attack);
			return;
		}

		//強攻撃ボタンが押されていたらStateを強攻撃にする
		if (Input::GetInstance().GetIsPushedTriggerButton(true) || Input::GetInstance().GetIsPushedTriggerButton(true))
		{
			//メニューが開かれていなかったら
			if (!own->GetMenu())
			{
				ChangeState(StateKind::StrongAttack);
				return;
			}

		}


		if (m_input < 30)
		{
			//メニューが開かれていなくて回避ボタンが押されたらStateを回避にする
			if (Input::GetInstance().IsReleased("Input_Roll") && !own->GetMenu() && own->GetAction())
			{
				ChangeState(StateKind::Roll);
				return;
			}
		}

		//メニューが開かれていなくてダッシュボタンが押されたらStateをダッシュにする
		if (Input::GetInstance().IsPushed("Input_Dash") && !own->GetMenu() && own->GetAction())
		{
			m_input++;
			if (m_input >= 30)
			{
				ChangeState(StateKind::Dash);
				return;
			}
		}
		else
		{
			//初期化する
			m_input = 0;
		}

		//メニューが開かれていなくてアイテムボタンが押されたらアイテムを使用する
		if (Input::GetInstance().IsTriggered("X") && !own->GetMenu())
		{
			ChangeState(StateKind::Item);
			return;
		}

		//メニューが開かれていなくてシールドを装備していた場合ガードボタンを押したらStateをガードにする
		if (Input::GetInstance().IsTriggered("Input_Shield") && m_equipmentShield && !own->GetMenu())
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
		//左スティックが入力されていなかったらStateをIdleにする
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
	}

	

	//左スティックが入力されていたら
	if (m_noInputFrame == 0)
	{
		//コントローラーの左スティックの入力を取得
		auto input = Input::GetInstance().GetInputStick(false);
		auto dirLog = m_dir;
		m_dir = GetDirection(input.first, input.second);

		//直前の入力方向と異なるとき
		if (dirLog != m_dir)
		{
			//アニメーションを変更する
			auto animName = GetWalkAnim(m_dir);
			own->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData("Player", animName), false, cWalkAnimSpeed, m_revese);
			own->NotInitAnim(false);
		}

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

		//ターゲットしていない時
		if (!m_targetState)
		{
			//キャラクターのアングルを決める
			own->SetModelAngle(atan2f(-moveVec.z, moveVec.x) - DX_PI_F / 2);
		}

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

/// <summary>
/// 入力方向から歩きアニメーションを取得する
/// </summary>
/// <param name="dir"></param>
/// <returns></returns>
std::string PlayerStateWalk::GetWalkAnim(eDir dir)
{
	//ターゲット状態だと
	if (m_targetState)
	{
		//装備をしていない時
		if (!m_equipmentState)
		{
			if (dir == eDir::Forward)
			{
				//逆再生するかどうか
				m_revese = false;
				return std::string("Walk");
			}
			else if (dir == eDir::Right || dir == eDir::ForwardRight || dir == eDir::BackRight)
			{
				//逆再生するかどうか
				m_revese = false;
				return std::string("RightWalk");
			}
			else if (dir == eDir::Left || dir == eDir::ForwardLeft || dir == eDir::BackLeft)
			{
				//逆再生するかどうか
				m_revese = false;
				return std::string("LeftWalk");
			}
			else if (dir == eDir::Back)
			{
				//逆再生するかどうか
				m_revese = true;
				return std::string("Walk");
			}
		}
		//装備をしている時
		else
		{
			if (dir == eDir::Forward)
			{
				//逆再生するかどうか
				m_revese = false;
				return std::string("ShieldWalk");
			}
			else if (dir == eDir::Right || dir == eDir::ForwardRight || dir == eDir::BackRight)
			{
				//逆再生するかどうか
				m_revese = false;
				return std::string("WeaponSideWalk");
			}
			else if (dir == eDir::Left || dir == eDir::ForwardLeft || dir == eDir::BackLeft)
			{
				//逆再生するかどうか
				m_revese = true;
				return std::string("WeaponSideWalk");
			}
			else if (dir == eDir::Back)
			{
				//逆再生するかどうか
				m_revese = true;
				return std::string("ShieldWalk");
			}
		}
		
	}
	//ターゲットしていない状態
	else
	{
		if (!m_equipmentState)
		{
			return std::string("Walk");
			//逆再生するかどうか
			m_revese = false;
		}
		else
		{
			return std::string("ShieldWalk");
			//逆再生するかどうか
			m_revese = false;
		}
	}
	

}
