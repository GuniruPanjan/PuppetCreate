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

	//���b�Z�[�W���������擾����
	CsvLoad::GetInstance().MessageDataLoad(m_stageName, m_pGenerateInfo);

	//csv���Ń}�b�v���w�肵�Ă��邽�߃}�b�v�̕␳������
	for (auto& generate : m_pGenerateInfo)
	{
		//�}�b�v��1�Ȃ�
		if (generate->mapNumber == 1)
		{
			//���̃}�b�v�̏o�����郁�b�Z�[�W�̐����X�V����
			int num = m_MessageGenerationCountPerOneMap[generate->mapNumber];
			num++;
			m_MessageGenerationCountPerOneMap[generate->mapNumber];
		}
	}
}

/// <summary>
/// �Q�[���̎d�l��ł̏���������
/// </summary>
/// <param name="physics"></param>
/// <param name="gameManager"></param>
void MessageManager::GameInit(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager)
{
	auto thisMapName = gameManager->GetThisMapName();

	//���b�Z�[�W���������܂킵��
	for (auto& generate : m_pGenerateInfo)
	{
		//���̃}�b�v����v���Ă���Ƃ�
		if (generate->mapNumber == thisMapName)
		{
			//�����ς݂̃��b�Z�[�W������������
			if (generate->isCreated)
			{
				CreateMessage(generate->posx, generate->posy, generate->posz, generate->official, generate->one, generate->two, generate->three, physics);
			}
		}
	}
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="physics"></param>
/// <param name="gameManager"></param>
void MessageManager::Update(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, Player& player)
{
	//���̃}�b�v���ǂ̃}�b�v���擾����
	auto thisMapName = gameManager->GetThisMapName();

	//���b�Z�[�W���������܂킵��
	for (auto& generate : m_pGenerateInfo)
	{
		//���̃}�b�v����v�Ă���Ƃ�
		if (generate->mapNumber == thisMapName)
		{
			//�����ς݂łȂ����
			if (!generate->isCreated)
			{
				generate->isCreated = true;
				CreateMessage(generate->posx, generate->posy, generate->posz, generate->official, generate->one, generate->two, generate->three, physics);
			}
		}
	}

	//�}�b�v�̃��b�Z�[�W�Ƃ��čX�V����
	m_pMessage->Update(player);
}

void MessageManager::Draw()
{
	if (cMessage)
	{
		//���������O�ɂ������������ŗ�O�X���[�����
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
