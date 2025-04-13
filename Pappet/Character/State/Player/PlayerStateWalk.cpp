#include "PlayerStateWalk.h"
#include "Input/Input.h"
#include "Character/Player.h"

#include "External/CsvLoad.h"


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
PlayerStateWalk::PlayerStateWalk(std::shared_ptr<CharacterBase> chara) :
	StateBase(chara),
	m_dir(),
	m_walkCount(0),
	m_targetState(false),
	m_equipmentState(false)
{
	//現在のステートを歩き状態にする
	m_nowState = StateKind::Walk;

	//このシーンに遷移した瞬間の左スティックの入力角度を取得しておく
	auto input = Input::GetInstance().GetInputStick(false);
	m_dir = GetDirection(input.first, -input.second);
	auto animName = GetWalkAnim(m_dir);
	m_pChara.lock()->ChangeStateAnim(CsvLoad::GetInstance().GetAnimData(chara->GetCharacterName(), animName));
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
void PlayerStateWalk::Init(std::string md)
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

	//左スティックが入力されていなかったらStateをIdleにする
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

	//コントローラーの左スティックの入力を取得
	auto input = Input::GetInstance().GetInputStick(false);
	auto dirLog = m_dir;
	m_dir = GetDirection(input.first, -input.second);

	//直前の入力方向と異なるとき
	if (dirLog != m_dir)
	{
		//アニメーションを変更する
		auto animName = GetWalkAnim(m_dir);
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
				return std::string("Walk");
				//逆再生するかどうか
				m_revese = false;
			}
			if (dir == eDir::Right)
			{
				return std::string("RightWalk");
				//逆再生するかどうか
				m_revese = false;
			}
			else if (dir == eDir::Left)
			{
				return std::string("LeftWalk");
				//逆再生するかどうか
				m_revese = false;
			}
			else if (dir == eDir::Back)
			{
				return std::string("Walk");
				//逆再生するかどうか
				m_revese = true;
			}
		}
		//装備をしている時
		else
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
