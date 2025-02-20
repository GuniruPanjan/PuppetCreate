#include "ItemManager.h"
#include "GameManager.h"
#include "External/CsvLoad.h"
#include "Item/Weapon.h"
#include "Item/Shield.h"
#include "Item/Armor.h"
#include "Item/Tool.h"
#include "EffectManager.h"

namespace
{
	//�G�t�F�N�g�̍Đ�����
	int effectPlayerBack = 0;

	//�V���O���g��
	auto& effect = EffectManager::GetInstance();
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
ItemManager::ItemManager():
	m_stageName(""),
	m_itemPick(false)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ItemManager::~ItemManager()
{
	//�z��̑S�v�f�폜
}

/// <summary>
/// ����������
/// </summary>
/// <param name="stageName">�X�e�[�W��</param>
void ItemManager::Init(const char* stageName)
{
	m_pGenerateInfo.clear();
	//m_pItems.clear();

	m_stageName = stageName;

	//�A�C�e�����������擾����
	CsvLoad::GetInstance().ItemDataLoad(m_stageName, m_pGenerateInfo);

	//csv���Ń}�b�v���w�肵�Ă��邽�߃}�b�v�̕␳������
	for (auto& generate : m_pGenerateInfo)
	{
		//�}�b�v��1�Ȃ�
		if (generate->mapNumber == 1)
		{
			//���̃}�b�v�̏o������A�C�e���̐����X�V����
			int num = m_itemGenerationCountPerOneMap[generate->mapNumber];
			num++;
			m_itemGenerationCountPerOneMap[generate->mapNumber];
		}
	}
}

/// <summary>
/// �Q�[���̎d�l��ł̏���������
/// </summary>
/// <param name="physics">�����N���X</param>
/// <param name="gameManager">�Q�[���}�l�W���[</param>
void ItemManager::GameInit(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager)
{
	//�A�C�e���̓����蔻�������
	//for (auto& item : m_pItems)
	//{
	//	//�}�b�v�Ɍ������Ă�����
	//	if (!item->GetItemTaking())
	//	{
	//		item->ItemFinalize(physics);
	//	}
	//}

	auto thisMapName = gameManager->GetThisMapName();

	if (thisMapName == 1 || thisMapName == 2 || thisMapName == 3 ||
		thisMapName == 4 || thisMapName == 5)
	{
		//�A�C�e�����������܂킵��
		for (auto& generate : m_pGenerateInfo)
		{
			//���̃}�b�v����v���Ă���Ƃ�
			if (generate->mapNumber == thisMapName)
			{
				//�����ς݂̃A�C�e��������������
				if (generate->isCreated && !generate->isPickUp)
				{
					CreateItem(generate->posX, generate->posY, generate->posZ, generate->itemName, physics);

					//���̃A�C�e�����𔻒f����
					CheckItem(generate->itemName, generate->SmallCore, generate->MediumCore, generate->Rubbish, generate->BlackSword, generate->Distorted, generate->ArmorNormal);
				}
			}
			
		}
	}
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="physics">�t�B�W�b�N�X</param>
void ItemManager::Update(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, bool taking)
{

	//���̃}�b�v���ǂ̃}�b�v���擾����
	auto thisMapName = gameManager->GetThisMapName();

	if (thisMapName == 1 || thisMapName == 2 || thisMapName == 3 ||
		thisMapName == 4 || thisMapName == 5)
	{
		//�A�C�e�����������܂킵��
		for (auto& generate : m_pGenerateInfo)
		{
			//���̃}�b�v����v���Ă���Ƃ�
			if (generate->mapNumber == thisMapName)
			{
				//�����ς݂łȂ����
				if (!generate->isCreated)
				{
					generate->isCreated = true;
					CreateItem(generate->posX, generate->posY, generate->posZ, generate->itemName, physics);

					//���̃A�C�e�����𔻒f����
					CheckItem(generate->itemName, generate->SmallCore, generate->MediumCore, generate->Rubbish, generate->BlackSword, generate->Distorted, generate->ArmorNormal);
				}
			}
		}
	}


	//�}�b�v�̃A�C�e���Ƃ��čX�V����
	for (auto& item : m_pItems)
	{
		//�A�C�e��������ĂȂ��Ƃ�
		if (!item->GetItemTaking())
		{
			if (effectPlayerBack <= 30)
			{
				effectPlayerBack++;
			}
			else
			{
				effect.EffectCreate("Item", item->GetPos().ConversionToVECTOR());

				effectPlayerBack = 0;
			}
		}

		item->ItemUpdate(taking);

		//�͋Z���g��
		//�A�C�e�������邩�͈̔�
		//���������ɂȂ��Ă���
		if (!m_itemPick)
		{
			//m_itemPick = item->GetItemPick();
		}
		//else if(!item->GetIsOut())
		//{
		//	m_itemPick = item->GetItemPick();
		//}

		//�Ƃ����ꍇ
		if (taking && item->GetItemBox())
		{
			m_item.SmallCore += item->GetItemKinds().SmallCore;
			m_item.MediumCore += item->GetItemKinds().MediumCore;
			m_item.Rubbish += item->GetItemKinds().Rubbish;
			m_item.BlackSword += item->GetItemKinds().BlackSword;
			m_item.Distorted += item->GetItemKinds().Distorted;
			m_item.ArmorNormal += item->GetItemKinds().ArmorNormal;

			m_uiItem.u_SmallCore += item->GetItemKinds().SmallCore;
			m_uiItem.u_MediumCore += item->GetItemKinds().MediumCore;
			m_uiItem.u_Rubbish += item->GetItemKinds().Rubbish;
			m_uiItem.u_BlackSword += item->GetItemKinds().BlackSword;
			m_uiItem.u_Distorted += item->GetItemKinds().Distorted;
			m_uiItem.u_ArmorNormal += item->GetItemKinds().ArmorNormal;

			item->SetItemBox(false);

			//�A�C�e�����擾���ꂽ���Ƃ��L�^
			for (auto& generate : m_pGenerateInfo)
			{
				if (generate->posX == item->GetPos().x && generate->posY == item->GetPos().y && generate->posZ == item->GetPos().z)
				{
					generate->isPickUp = true;
					break;
				}
			}

			//�G�t�F�N�g���폜
			//effect.RemoveEffect("Item", item->GetPos().ConversionToVECTOR());
		}

	}

	//m_pItems.clear();
}

/// <summary>
/// �`�揈��
/// </summary>
void ItemManager::Draw()
{
#if false
	DrawFormatString(200, 600, 0xffffff, "SmallCore : %d", m_item.SmallCore);
	DrawFormatString(200, 700, 0xffffff, "MediumCore : %d", m_item.MediumCore);
	DrawFormatString(200, 800, 0xffffff, "Rubbish : %d", m_item.Rubbish);
	DrawFormatString(200, 100, 0xffffff, "BlackSword : %d", m_item.BlackSword);
	DrawFormatString(200, 150, 0xffffff, "Distorted : %d", m_item.Distorted);
	DrawFormatString(200, 200, 0xffffff, "ArmorNormal : %d", m_item.ArmorNormal);
#endif
}

/// <summary>
/// �I������
/// </summary>
void ItemManager::End()
{

}

/// <summary>
/// �A�C�e���̐���
/// </summary>
/// <param name="posX">X���W</param>
/// <param name="posY">Y���W</param>
/// <param name="posZ">Z���W</param>
/// <param name="name">�L�����N�^�[��</param>
/// <param name="physics">�����|�C���^</param>
void ItemManager::CreateItem(float posX, float posY, float posZ, std::string name, std::shared_ptr<MyLibrary::Physics> physics)
{
	if (name == "Weapon")
	{
		m_pweapon = std::make_shared<Weapon>();
		m_pweapon->ItemInit(posX, posY, posZ, physics);
		m_pItems.emplace_back(m_pweapon);
	}
	if (name == "Shield")
	{
		m_pshield = std::make_shared<Shield>();
		m_pshield->ItemInit(posX, posY, posZ, physics);
		m_pItems.emplace_back(m_pshield);
	}
	if (name == "Armor")
	{
		m_parmor = std::make_shared<Armor>();
		m_parmor->ItemInit(posX, posY, posZ, physics);
		m_pItems.emplace_back(m_parmor);
	}
	if (name == "Tool")
	{
		//m_ptool = std::make_shared<Tool>();
		//m_ptool->ItenInit(posX, posY, posZ, physics);
		//m_pItems.emplace_back(m_pweapon);
	}
}

/// <summary>
/// ���̃A�C�e�������f����
/// </summary>
/// <param name="item"></param>
void ItemManager::CheckItem(std::string name, int SmallCore, int MediumCore, int Rubbish, int BlackSword, int Distorted, int ArmorNormal)
{
	if (name == "Weapon")
	{
		m_pweapon->ItemGudgment(SmallCore, MediumCore, Rubbish, BlackSword, Distorted, ArmorNormal);
	}
	if (name == "Shield")
	{
		m_pshield->ItemGudgment(SmallCore, MediumCore, Rubbish, BlackSword, Distorted, ArmorNormal);
	}
	if (name == "Armor")
	{
		m_parmor->ItemGudgment(SmallCore, MediumCore, Rubbish, BlackSword, Distorted, ArmorNormal);
	}
	if (name == "Tool")
	{
		//m_ptool = std::make_shared<Tool>();
		//m_ptool->ItenInit(posX, posY, posZ, physics);
		//m_pItems.emplace_back(m_pweapon);
	}
}
