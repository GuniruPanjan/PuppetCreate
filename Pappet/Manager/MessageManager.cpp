#include "MessageManager.h"
#include "External/CsvLoad.h"
#include "Manager/GameManager.h"

namespace
{
	bool cMessage = false;
}

MessageManager::MessageManager() :
	m_stageName("")
{
}

MessageManager::~MessageManager()
{
}

void MessageManager::Init(const char* stageName)
{
	m_pGenerateInfo.clear();

	m_stageName = stageName;

	cMessage = false;

	//メッセージ生成情報を取得する
	CsvLoad::GetInstance().MessageDataLoad(m_stageName, m_pGenerateInfo);

	//csv側でマップを指定しているためマップの補正をする
	for (auto& generate : m_pGenerateInfo)
	{
		//マップが1なら
		if (generate->mapNumber == 1)
		{
			//そのマップの出現するメッセージの数を更新する
			int num = m_MessageGenerationCountPerOneMap[generate->mapNumber];
			num++;
			m_MessageGenerationCountPerOneMap[generate->mapNumber];
		}
	}
}

/// <summary>
/// ゲームの仕様上での初期化処理
/// </summary>
/// <param name="physics"></param>
/// <param name="gameManager"></param>
void MessageManager::GameInit(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager)
{
	auto thisMapName = gameManager->GetThisMapName();

	//メッセージ生成情報をまわして
	for (auto& generate : m_pGenerateInfo)
	{
		//今のマップが一致しているとき
		if (generate->mapNumber == thisMapName)
		{
			//生成済みのメッセージを初期化する
			if (generate->isCreated)
			{
				CreateMessage(generate->posx, generate->posy, generate->posz, generate->official, generate->one, generate->two, generate->three, physics);
			}
		}
	}
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="physics"></param>
/// <param name="gameManager"></param>
void MessageManager::Update(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, Player& player)
{
	//今のマップがどのマップか取得する
	auto thisMapName = gameManager->GetThisMapName();

	//メッセージ生成情報をまわして
	for (auto& generate : m_pGenerateInfo)
	{
		//今のマップが一致ているとき
		if (generate->mapNumber == thisMapName)
		{
			//生成済みでなければ
			if (!generate->isCreated)
			{
				generate->isCreated = true;
				CreateMessage(generate->posx, generate->posy, generate->posz, generate->official, generate->one, generate->two, generate->three, physics);
			}
		}
	}

	//マップのメッセージとして更新する
	m_pMessage->Update(player);
}

void MessageManager::Draw()
{
	if (cMessage)
	{
		//生成される前にここが動く事で例外スローされる
		m_pMessage->Draw();
	}
}

void MessageManager::End()
{
	m_pMessage->End();
}

bool MessageManager::GetDraw()
{
	if (cMessage)
	{
		return m_pMessage->GetDraw();
	}
	else
	{
		return false;
	}
}

void MessageManager::CreateMessage(float posx, float posy, float posz, bool official, int one, int two, int three, std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pMessage = std::make_shared<Message>();
	m_pMessage->Init(posx, posy, posz, official, one, two, three, physics);
	//m_pMessage->SetCan(true);
	cMessage = true;
}
