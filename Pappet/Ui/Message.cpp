#include "Message.h"
#include "Object/MessageObject.h"
#include "Manager/EffectManager.h"
#include "Character/Player.h"
#include "External/Font.h"

namespace
{
	//シングルトン
	auto& cEffect = EffectManager::GetInstance();
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
	m_aButton = MyLoadGraph("Data/UI/AButton.png", 1, 1);
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

	m_pFont->FontInit(50);
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
		}
	}
}

void Message::Draw()
{
	if (m_effectTime >= 50)
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
			DrawGraph(130, 100, m_messageUI, false);

			DrawStringToHandle(400, 150, "目の前の大釜では特殊な休息ができ", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(400, 250, "右下にある取得したコアを使って", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(500, 350, "レベルを上げることが可能", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(500, 450, "レベルを上げ各地に居座る", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(400, 550, "強大なコアを持った人形達を倒し", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(700, 650, "頂点に立て", 0xffffff, m_pFont->GetHandle());
		}
		//移動チュートリアル
		else if (m_official == 2)
		{
			//背景描画
			DrawGraph(130, 100, m_messageUI, false);

			//左スティック描画
			DrawGraph(200, 150, m_lStick, true);
			DrawStringToHandle(400, 180, "Lスティック　：　移動", 0xffffff, m_pFont->GetHandle());
			
			//Bボタン描画
			DrawGraph(180, 340, m_aButton, true);
			DrawStringToHandle(400, 350, "Aボタン単押し　：　回避", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(400, 450, "Aボタン長押し　：　走る", 0xffffff, m_pFont->GetHandle());

			//スタミナ描画
			DrawGraph(370, 660, m_staminaBar, true);
			DrawStringToHandle(350, 600, "回避と走る行動にはスタミナを消費する", 0xffffff, m_pFont->GetHandle());
		}
		//カメラチュートリアル
		else if (m_official == 3)
		{
			//背景描画
			DrawGraph(130, 100, m_messageUI, false);

			//右スティック描画
			DrawGraph(180, 270, m_rStick, true);
			DrawStringToHandle(400, 250, "Rスティック　：　カメラ移動", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(400, 400, "Rスティック押し込み　：　ターゲット集中", 0xffffff, m_pFont->GetHandle());

			DrawStringToHandle(250, 600, "ターゲット集中の間にRスティックを動かすと\n　　　　　　　ターゲット変更", 0xffffff, m_pFont->GetHandle());

		}
		//攻撃チュートリアル
		else if (m_official == 4)
		{
			//背景描画
			DrawGraph(130, 100, m_messageUI, false);

			//RBボタン描画
			DrawGraph(180, 200, m_rbButton, true);
			DrawStringToHandle(600, 200, "RBボタン　：　攻撃", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(500, 270, "攻撃中にもう一度攻撃を押すと\n　　最大３回コンボできる", 0xffffff, m_pFont->GetHandle());

			//RTボタン描画
			DrawGraph(180, 450, m_rtButton, true);
			DrawStringToHandle(600, 450, "RTボタン　：　強攻撃", 0xffffff, m_pFont->GetHandle());

			//スタミナ描画
			DrawGraph(370, 660, m_staminaBar, true);
			DrawStringToHandle(400, 600, "攻撃行動ではスタミナを消費する", 0xffffff, m_pFont->GetHandle());
		}
		//アイテム関係チュートリアル
		else if (m_official == 5)
		{
			//背景描画
			DrawGraph(130, 100, m_messageUI, false);

			//Yボタン描画
			DrawGraph(180, 200, m_yButton, true);
			DrawStringToHandle(400, 200, "マップで光る物はアイテムとして入手できる", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(600, 300, "Yボタン　：　アイテム入手", 0xffffff, m_pFont->GetHandle());

			//STARTボタン描画
			DrawGraph(180, 450, m_startButton, true);
			DrawStringToHandle(500, 480, "STARTボタン　：　メニューを開く", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(400, 580, "メニューでは装備の変更やタイトルに戻れる", 0xffffff, m_pFont->GetHandle());

		}
		//防御チュートリアル
		else if (m_official == 6)
		{
			//背景描画
			DrawGraph(130, 100, m_messageUI, false);

			//LBボタン描画
			DrawGraph(180, 300, m_lbButton, true);
			DrawStringToHandle(400, 150, "盾を装備しているときは\n敵の攻撃を防ぐことができる", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(600, 320, "LBボタン　：　ガード", 0xffffff, m_pFont->GetHandle());

			//スタミナ描画
			DrawGraph(370, 660, m_staminaBar, true);
			DrawStringToHandle(400, 550, "防御行動ではスタミナを消費する\n防御中はスタミナの回復が遅くなる", 0xffffff, m_pFont->GetHandle());
		}
		//休息チュートリアル
		else if (m_official == 7)
		{
			//背景描画
			DrawGraph(130, 100, m_messageUI, false);

			//Yボタン描画
			DrawGraph(200, 250, m_yButton, true);
			DrawStringToHandle(400, 150, "　　棺桶のようなオブジェクトは\n　　　　　　　休息が可能、\n休息をするとリスポーン地点を固定する", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(600, 320, "Yボタン　：　休息", 0xffffff, m_pFont->GetHandle());

			//アイテム描画
			DrawGraph(200, 500, m_xButton, true);
			DrawStringToHandle(600, 520, "Xボタン　：　アイテム使用", 0xffffff, m_pFont->GetHandle());
			DrawStringToHandle(600, 600, "アイテムは使用すると消費する", 0xffffff, m_pFont->GetHandle());

		}
		//熊のエリアに誘導する
		else if(m_official == 8)
		{
			//背景描画
			DrawGraph(130, 100, m_messageUI, false);

			//熊の説明文
			DrawStringToHandle(450, 400, "熊の傀儡人形この先の墓場に眠る\n　　　　　HARIBO", 0xffffff, m_pFont->GetHandle());
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
