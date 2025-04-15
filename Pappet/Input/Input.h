#pragma once
#include "DxLib.h"
#include<unordered_map>
#include<map>
#include<string>

enum class InputType
{
	KeyBoard,     //キーボード
	Pad           //パッド
};

//エイリアス宣言
using InputTable_t = std::unordered_map<std::string, int>;

/// <summary>
/// 入力情報管理クラス
/// </summary>
class Input
{
private:
	//コンストラクタ
	Input();
	//デストラクタ
	~Input() {};

	static Input* m_instance;   //インスタンス

public:
	//コピーコンストラクタを禁止する
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;
	Input(Input&&) = delete;
	Input& operator= (const Input&&) = delete;

	/// <summary>
	/// インスタンスを取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static Input& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new Input;
		}
		return *m_instance;
	}

	/// <summary>
	/// インスタンスを削除
	/// </summary>
	static void Destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:
	//入力情報を更新する
	void Update();

	//指定のコマンドが押された瞬間なのか
	bool IsTriggered(const char* command) const;
	//指定のコマンドが押し続けられているかどうか
	bool IsPushed(const char* command) const;
	//指定のコマンドが離されたかどうか
	bool IsReleased(const char* command) const;
	//スティックの入力情報を取得
	std::pair<float, float> GetInputStick(bool isRight) const;
	//ZR,ZLボタンの入力情報を取得
	bool GetIsPushedTriggerButton(bool isRight) const;

	bool GetIsTriggeredTriggerButton(bool isRight) const;

	bool IsStickNeutral(bool isRight) const;
private:
	InputTable_t m_commandTable;	//コマンド配列

	//コマンドの入力を覚えておくための配列
	std::map < std::string, int> m_inputData;		//現在の入力
	std::map < std::string, int> m_lastInputData;	//直前の入力

	//コントローラーの入力情報
	DINPUT_JOYSTATE m_padState;
	XINPUT_STATE* XInputState;

	int m_rightTriggerPushFrameCount;	//右トリガーの押されたフレーム数
	int m_leftTriggerPushFrameCount;	//左トリガーの押されたフレーム数

};

