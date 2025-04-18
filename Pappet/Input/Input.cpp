#include "Input.h"
#include <cassert>

Input* Input::m_instance = nullptr;

namespace
{
	enum PadCheckMask : int
	{
		A = 0x00000010,	//Aボタン
		B = 0x00000020,	//Bボタン
		X = 0x00000040,	//Xボタン
		Y = 0x00000080,	//Yボタン

		L = 0x00000100,	//Lボタン
		R = 0x00000200,	//Rボタン

		RT = 0x00000400,	//RTボタン
		LT = 0x00001000,	//LTボタン

		RStickPush = 0x00002000,	//Rスティック押し込み
		test = 0x00008000,	//Rスティック押し込み

		P = 0x00000800,	//ホームボタン

		UP = 0x00000008,	//上ボタン
		RIGHT = 0x00000004,	//右ボタン
		DOWN = 0x00000001,	//下ボタン
		LEFT = 0x00000002,	//左ボタン
	};

	//トリガーボタンのデッドゾーン
	constexpr int cTriggerDeadZone = 60;
}

/// <summary>
/// コンストラクタ
/// </summary>
Input::Input() :
	m_padState(),
	XInputState(new XINPUT_STATE),
	m_rightTriggerPushFrameCount(0),
	m_leftTriggerPushFrameCount(0)
{
	//コマンドテーブルにコマンドを追加する

	m_commandTable["Input_Jump"] = { PadCheckMask::A };
	m_commandTable["Input_Dash"] = { PadCheckMask::B };
	m_commandTable["Input_Roll"] = { PadCheckMask::B };
	m_commandTable["Input_Attack"] = { PadCheckMask::R };
	m_commandTable["Input_Shield"] = { PadCheckMask::L };
	m_commandTable["Input_StrongAttack"] = { PadCheckMask::RT };
	m_commandTable["Input_Parry"] = { PadCheckMask::LT };
	m_commandTable["X"] = { PadCheckMask::X };
	m_commandTable["Y"] = { PadCheckMask::Y };
	m_commandTable["Pause"] = { PadCheckMask::P };
	m_commandTable["Ok"] = { PadCheckMask::A };
	m_commandTable["Cancel"] = { PadCheckMask::B };

	m_commandTable["Up"] = { PadCheckMask::UP };
	m_commandTable["Down"] = { PadCheckMask::DOWN };
	m_commandTable["Left"] = { PadCheckMask::LEFT };
	m_commandTable["Right"] = { PadCheckMask::RIGHT };


}

/// <summary>
/// 入力情報を更新する
/// </summary>
void Input::Update()
{
	//直前入力をコピーしておく(押した瞬間を取得するよう)
	m_lastInputData = m_inputData;
	//パッド情報の取得
	int padState = GetJoypadInputState(DX_INPUT_PAD1);

	//登録された情報とハードの情報を照らし合わせながら
	//inputData_の内容を更新していく
	for (const auto& cmd : m_commandTable)
	{
		//コマンドの名前から入力データを作る
		auto& input = m_inputData[cmd.first];

		//コントローラーのチェック
		input = false;
		if (padState & cmd.second)
		{
			input = true;
		}

		//ボタンの入力を取得
		GetJoypadDirectInputState(DX_INPUT_PAD1, &m_padState);
		//ZR,ZLの入力を取得
		GetJoypadXInputState(DX_INPUT_PAD1, XInputState);

		//ZRの入力情報を返す
		if (XInputState->RightTrigger > cTriggerDeadZone)
		{
			m_rightTriggerPushFrameCount++;
		}
		else
		{
			m_rightTriggerPushFrameCount = 0;
		}
		//LRの入力情報を返す
		if (XInputState->LeftTrigger > cTriggerDeadZone)
		{
			m_leftTriggerPushFrameCount++;
		}
		else
		{
			m_leftTriggerPushFrameCount = 0;
		}

	}
}

/// <summary>
/// 指定のコマンドが押された瞬間なのか
/// </summary>
/// <param name="command"></param>
/// <returns></returns>
bool Input::IsTriggered(const char* command) const
{
	auto it = m_inputData.find(command);
	if (it == m_inputData.end())   //情報がなければfalseを返す
	{
		assert(0 && "指定されたコマンドは登録されていません");
		return false;
	}
	return (m_inputData.at(command) && !m_lastInputData.at(command));
}
/// <summary>
/// 指定のコマンドが押し続けられているかどうか
/// </summary>
/// <param name="command"></param>
/// <returns></returns>
bool Input::IsPushed(const char* command) const
{
	auto it = m_inputData.find(command);
	if (it == m_inputData.end())  //情報がなければfalseを返す
	{
		assert(0 && "指定されたコマンドが登録されていません");
		return false;
	}
	return (m_inputData.at(command) && m_lastInputData.at(command));
}

/// <summary>
/// 指定のコマンドが離された瞬間をとる
/// </summary>
/// <param name="command"></param>
/// <returns></returns>
bool Input::IsReleased(const char* command) const
{
	auto it = m_inputData.find(command);
	if (it == m_inputData.end())  //情報がなければfalseを返す
	{
		assert(0 && "指定されたコマンドが登録されていません");
		return false;
	}
	return (!m_inputData.at(command) && m_lastInputData.at(command));
}
/// <summary>
/// スティックの入力情報を取得
/// </summary>
/// <param name="isRight"></param>
/// <returns></returns>
std::pair<float, float> Input::GetInputStick(bool isRight) const
{
	if (isRight)
	{
		//右スティックの入力情報を返す
		return std::make_pair(static_cast<float>(m_padState.Rx), static_cast<float>(m_padState.Ry));
	}
	else
	{
		//左スティックの入力情報を返す
		return std::make_pair(static_cast<float>(m_padState.X), static_cast<float>(m_padState.Y));
	}
}
/// <summary>
/// ZR.ZLボタンの入力情報を取得
/// </summary>
/// <param name="isRight"></param>
/// <returns></returns>
bool Input::GetIsPushedTriggerButton(bool isRight) const
{
	if (isRight)
	{
		return m_rightTriggerPushFrameCount > 0;
	}
	else
	{
		return m_leftTriggerPushFrameCount > 0;
	}
}

bool Input::GetIsTriggeredTriggerButton(bool isRight) const
{
	if (isRight)
	{
		return m_rightTriggerPushFrameCount == 1;
	}
	else
	{
		return m_leftTriggerPushFrameCount == 1;
	}
}

bool Input::IsStickNeutral(bool isRight) const
{
	if (isRight)
	{
		//スティックの入力値がXYそれぞれ規定値以下かどうか
		if (abs(m_padState.Rx) < 60 && abs(m_padState.Ry) < 60)	return true;

		return false;
	}
	else
	{
		//スティックの入力値がXYそれぞれ規定値以下かどうか
		if (abs(m_padState.X) < 60 && abs(m_padState.Y) < 60)	return true;

		return false;
	}
}
