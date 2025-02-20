#include "Armor.h"

namespace
{
	//�A�C�e����
	constexpr const char* cItemName = "Armor";
}

Armor::Armor()
{
}

Armor::~Armor()
{
}

/// <summary>
/// ������
/// </summary>
void Armor::Init()
{
	//���}�Ȗh��
	if (m_common.sa_equipment)
	{
		m_common.sa_defense = 30.0f;

		//���f���p�X������
		m_path = "Data/Armor/CommonArmor.mv1";
	}
}

/// <summary>
/// �}�b�v�A�C�e���Ƃ��Ă̏���������
/// </summary>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="posZ"></param>
/// <param name="physics"></param>
void Armor::ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
	//���
	m_pPhysics = physics;

	InitSearch(MyLibrary::LibVec3(posX, posY, posZ), 80.0f);

	m_centerPos = MyLibrary::LibVec3(posX, posY, posZ);
}

void Armor::Update()
{
}

void Armor::ItemUpdate(bool taking)
{
	//�A�C�e�����܂�����ĂȂ����̏���
	if (!m_isTaking && !taking)
	{
		if (m_pSearch->GetIsStay())
		{
			m_itemPick = true;
		}
		else
		{
			m_itemPick = false;
		}
	}
	//�A�C�e�������ꂽ���̏���
	else if (m_itemPick && taking)
	{
		//��񂾂����s
		ItemFinalize(m_pPhysics);

		m_itemPick = false;
		m_isTaking = true;
		m_itemBox = true;
	}

	m_pSearch->Update(m_centerPos);

}

void Armor::Draw()
{
}

void Armor::End()
{
}
