#include "Message.h"
#include "Object/MessageObject.h"
#include "Manager/EffectManager.h"
#include "Character/Player.h"

namespace
{
	//エフェクト再生時間
	int cEffectTime = 40;

	//シングルトン
	auto& cEffect = EffectManager::GetInstance();
}

Message::Message() :
	m_xpad(),
	m_official(false),
	m_one(0),
	m_two(0),
	m_three(0),
	m_draw(false),
	m_can(false)
{
	
}

Message::~Message()
{
}

void Message::DataInit()
{
	
}

void Message::Init(float posX, float posY, float posZ, bool official, int one, int two, int three, std::shared_ptr<MyLibrary::Physics> physics)
{
	m_messageUI = MyLoadGraph("Data/UI/操作説明ウィンドウ.png", 3, 3);

	m_pMessage = std::make_shared<MessageObject>(50.0f);

	m_centerPos = MyLibrary::LibVec3(posX, posY, posZ);

	m_pMessage->Init(physics, m_centerPos);

	m_official = official;
	m_one = one;
	m_two = two;
	m_three = three;
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
	if (cEffectTime >= 50)
	{
		cEffect.EffectCreate("Message", m_centerPos.ConversionToVECTOR());

		cEffectTime = 0;
	}
	else
	{
		cEffectTime++;
	}

	//説明描画
	if (m_draw)
	{
		//公式メッセージ描画
		if (m_official)
		{
			DrawGraph(130, 100, m_messageUI, false);
		}
		else
		{

		}
	}
}

void Message::End()
{
	DeleteGraph(m_messageUI);
}
