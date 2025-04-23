#include "Message.h"
#include "Object/MessageObject.h"
#include "Manager/EffectManager.h"
#include "Character/Player.h"
#include "External/Font.h"

namespace
{
	//シングルトン
	auto& cEffect = EffectManager::GetInstance();

	//定数の定義
	const int cEffectTimeThreshold = 50;
	const int cBackgroundX = 130;
	const int cBackgroundY = 100;
	const int cTextColor = 0xffffff;
	const int cFontSize = 50;
	const int cLStickX = 200;
	const int cLStickY = 150;
	const int cAButtonX = 180;
	const int cAButtonY = 340;
	const int cStaminaBarX = 370;
	const int cStaminaBarY = 660;
	const int cRStickX = 180;
	const int cRStickY = 270;
	const int cRBButtonX = 180;
	const int cRBButtonY = 200;
	const int cRTButtonX = 180;
	const int cRTButtonY = 450;
	const int cYButtonX = 180;
	const int cYButtonY = 200;
	const int cStartButtonX = 180;
	const int cStartButtonY = 450;
	const int cLBButtonX = 180;
	const int cLBButtonY = 300;
	const int cXButtonX = 200;
	const int cXButtonY = 250;
	const int cItemX = 200;
	const int cItemY = 500;
	const int cBearTextX = 450;
	const int cBearTextY = 400;
	const int cDrawStringX1 = 400;
	const int cDrawStringY1 = 150;
	const int cDrawStringY2 = 250;
	const int cDrawStringX2 = 500;
	const int cDrawStringY3 = 350;
	const int cDrawStringY4 = 450;
	const int cDrawStringY5 = 550;
	const int cDrawStringX3 = 700;
	const int cDrawStringY6 = 650;
	const int cDrawStringY7 = 180;
	const int cDrawStringX4 = 350;
	const int cDrawStringY8 = 600;
	const int cDrawStringY9 = 400;
	const int cDrawStringX5 = 250;
	const int cDrawStringX6 = 600;
	const int cDrawStringY10 = 200;
	const int cDrawStringY11 = 270;
	const int cDrawStringY12 = 300;
	const int cDrawStringY13 = 480;
	const int cDrawStringY14 = 580;
	const int cDrawStringY15 = 320;
	const int cDrawStringY16 = 520;
}

Message::Message() :
	m_xpad(),
	m_effectTime(50),
	m_official(0),
	m_one(0),
	m_two(0),
	m_three(0),
	m_draw(false),
	m_can(false)
{
	m_pFont = std::make_shared<Font>();
}

Message::~Message()
{
}

void Message::DataInit()
{
	
}

void Message::Init(float posX, float posY, float posZ, int official, int one, int two, int three, std::shared_ptr<MyLibrary::Physics> physics)
{
	m_messageUI = MyLoadGraph("Data/UI/Operation.png", 3, 3);
	m_lStick = MyLoadGraph("Data/UI/LStick.png", 1, 1);
	m_rStick = MyLoadGraph("Data/UI/RStick.png", 1, 1);
	m_bButton = MyLoadGraph("Data/UI/BButton.png", 1, 1);
	m_yButton = MyLoadGraph("Data/UI/YButton.png", 1, 1);
	m_xButton = MyLoadGraph("Data/UI/XButton.png", 1, 1);
	m_staminaBar = MyLoadGraph("Data/UI/StaminaBar.png", 1, 2);
	m_rbButton = MyLoadGraph("Data/UI/RBButton.png", 2, 2);
	m_rtButton = MyLoadGraph("Data/UI/RTButton.png", 2, 2);
	m_lbButton = MyLoadGraph("Data/UI/LBButton.png", 2, 2);
	m_startButton = MyLoadGraph("Data/UI/START.png", 2, 2);

	m_pMessage = std::make_shared<MessageObject>(50.0f);

	m_centerPos = MyLibrary::LibVec3(posX, posY, posZ);

	m_pMessage->Init(physics, m_centerPos);

	m_official = official;
	m_one = one;
	m_two = two;
	m_three = three;

	m_pFont->FontInit(cFontSize);
}

void Message::Update(Player& player)
{
	//パッド入力取得
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	m_pMessage->Update(m_centerPos);

	//メッセージを読む
	if (player.GetMessageRead())
	{
		m_draw = true;
	}
	if (m_draw)
	{
		//Bボタンを押すと戻る
		if (m_xpad.Buttons[13] == 1)
		{
			m_draw = false;

			player.SetMessageRead(false);
			//ローリングできないようにする
			player.SetAction(false);
		}
	}
}

void Message::Draw()
{
	if (m_effectTime >= cEffectTimeThreshold)
	{
		cEffect.EffectCreate("Message", m_centerPos.ConversionToVECTOR());

		m_effectTime = 0;
	}
	else
	{
		m_effectTime++;
	}
}

void Message::DrawString()
{
	//説明描画
	if (m_draw)
	{
		//今後何をするかの説明
		if (m_official == 1)
		{
			//背景描画
			DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

			DrawStringToHandle(cDrawStringX1, cDrawStringY1, "目の前の大釜では特殊な休息ができ", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX1, cDrawStringY2, "右下にある取得したコアを使って", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX2, cDrawStringY3, "レベルを上げることが可能", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX2, cDrawStringY4, "レベルを上げ各地に居座る", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX1, cDrawStringY5, "強大なコアを持った人形達を倒し", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX3, cDrawStringY6, "頂点に立て", cTextColor, m_pFont->GetHandle());
		}
		//移動チュートリアル
		else if (m_official == 2)
		{
			//背景描画
			DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

			//左スティック描画
			DrawGraph(cLStickX, cLStickY, m_lStick, true);
			DrawStringToHandle(cDrawStringX1, cDrawStringY7, "Lスティック　：　移動", cTextColor, m_pFont->GetHandle());
			
			//Bボタン描画
			DrawGraph(cAButtonX, cAButtonY, m_bButton, true);
			DrawStringToHandle(cDrawStringX1, cDrawStringY3, "Bボタン単押し　：　回避", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX1, cDrawStringY4, "Bボタン長押し　：　走る", cTextColor, m_pFont->GetHandle());

			//スタミナ描画
			DrawGraph(cStaminaBarX, cStaminaBarY, m_staminaBar, true);
			DrawStringToHandle(cDrawStringX4, cDrawStringY8, "回避と走る行動にはスタミナを消費する", cTextColor, m_pFont->GetHandle());
		}
		//カメラチュートリアル
		else if (m_official == 3)
		{
			//背景描画
			DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

			//右スティック描画
			DrawGraph(cRStickX, cRStickY, m_rStick, true);
			DrawStringToHandle(cDrawStringX1, cDrawStringY2, "Rスティック　：　カメラ移動", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX1, cDrawStringY9, "Rスティック押し込み　：　ターゲット集中", cTextColor, m_pFont->GetHandle());

			DrawStringToHandle(cDrawStringX5, cDrawStringY8, "ターゲット集中の間にRスティックを動かすと\n　　　　　　　ターゲット変更", cTextColor, m_pFont->GetHandle());

		}
		//攻撃チュートリアル
		else if (m_official == 4)
		{
			//背景描画
			DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

			//RBボタン描画
			DrawGraph(cRBButtonX, cRBButtonY, m_rbButton, true);
			DrawStringToHandle(cDrawStringX6, cDrawStringY10, "RBボタン　：　攻撃", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX2, cDrawStringY11, "攻撃中にもう一度攻撃を押すと\n　　最大３回コンボできる", cTextColor, m_pFont->GetHandle());

			//RTボタン描画
			DrawGraph(cRTButtonX, cRTButtonY, m_rtButton, true);
			DrawStringToHandle(cDrawStringX6, cDrawStringY4, "RTボタン　：　強攻撃", cTextColor, m_pFont->GetHandle());

			//スタミナ描画
			DrawGraph(cStaminaBarX, cStaminaBarY, m_staminaBar, true);
			DrawStringToHandle(cDrawStringX1, cDrawStringY8, "攻撃行動ではスタミナを消費する", cTextColor, m_pFont->GetHandle());
		}
		//アイテム関係チュートリアル
		else if (m_official == 5)
		{
			//背景描画
			DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

			//Yボタン描画
			DrawGraph(cYButtonX, cYButtonY, m_yButton, true);
			DrawStringToHandle(cDrawStringX1, cDrawStringY10, "マップで光る物はアイテムとして入手できる", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX6, cDrawStringY12, "Yボタン　：　アイテム入手", cTextColor, m_pFont->GetHandle());

			//STARTボタン描画
			DrawGraph(cStartButtonX, cStartButtonY, m_startButton, true);
			DrawStringToHandle(cDrawStringX2, cDrawStringY13, "STARTボタン　：　メニューを開く", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX1, cDrawStringY14, "メニューでは装備の変更やタイトルに戻れる", cTextColor, m_pFont->GetHandle());

		}
		//防御チュートリアル
		else if (m_official == 6)
		{
			//背景描画
			DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

			//LBボタン描画
			DrawGraph(cLBButtonX, cLBButtonY, m_lbButton, true);
			DrawStringToHandle(cDrawStringX1, cDrawStringY1, "盾を装備しているときは\n敵の攻撃を防ぐことができる", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX6, cDrawStringY15, "LBボタン　：　ガード", cTextColor, m_pFont->GetHandle());

			//スタミナ描画
			DrawGraph(cStaminaBarX, cStaminaBarY, m_staminaBar, true);
			DrawStringToHandle(cDrawStringX1, cDrawStringY5, "防御行動ではスタミナを消費する\n防御中はスタミナの回復が遅くなる", cTextColor, m_pFont->GetHandle());
		}
		//休息チュートリアル
		else if (m_official == 7)
		{
			//背景描画
			DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

			//Yボタン描画
			DrawGraph(cXButtonX, cXButtonY, m_yButton, true);
			DrawStringToHandle(cDrawStringX1, cDrawStringY1, "　　棺桶のようなオブジェクトは\n　　　　　　　休息が可能、\n休息をするとリスポーン地点を固定する", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX6, cDrawStringY15, "Yボタン　：　休息", cTextColor, m_pFont->GetHandle());

			//アイテム描画
			DrawGraph(cItemX, cItemY, m_xButton, true);
			DrawStringToHandle(cDrawStringX6, cDrawStringY16, "Xボタン　：　アイテム使用", cTextColor, m_pFont->GetHandle());
			DrawStringToHandle(cDrawStringX6, cDrawStringY8, "アイテムは使用すると消費する", cTextColor, m_pFont->GetHandle());

		}
		//熊のエリアに誘導する
		else if(m_official == 8)
		{
			//背景描画
			DrawGraph(cBackgroundX, cBackgroundY, m_messageUI, false);

			//熊の説明文
			DrawStringToHandle(cBearTextX, cBearTextY, "熊の傀儡人形この先の墓場に眠る\n　　　　　HARIBO", cTextColor, m_pFont->GetHandle());
		}

		
	}
}

void Message::End()
{
	DeleteGraph(m_messageUI);
	DeleteGraph(m_lStick);
	DeleteGraph(m_aButton);
	DeleteGraph(m_yButton);
	DeleteGraph(m_xButton);
	DeleteGraph(m_staminaBar);
	DeleteGraph(m_rbButton);
	DeleteGraph(m_rtButton);
	DeleteGraph(m_lbButton);
	DeleteGraph(m_startButton);
}

bool Message::GetMessageStay()
{
	return m_pMessage->GetIsStay();
}
