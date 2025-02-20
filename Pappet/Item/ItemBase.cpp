#include "ItemBase.h"

/// <summary>
/// �I������
/// </summary>
/// <param name="physics"></param>
void ItemBase::ItemFinalize(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pSearch->Finalize(physics);
}

void ItemBase::ItemGudgment(int SmallCore, int MediumCore, int Rubbish, int BlackSword, int Distorted, int ArmorNormal)
{
	m_item.SmallCore = SmallCore;
	m_item.MediumCore = MediumCore;
	m_item.Rubbish = Rubbish;
	m_item.BlackSword = BlackSword;
	m_item.Distorted = Distorted;
	m_item.ArmorNormal = ArmorNormal;
}

/// <summary>
/// �I������
/// </summary>
void ItemBase::ItemEnd()
{
}

bool ItemBase::GetIsOut()
{
	return m_pSearch->GetIsStay();
}

/// <summary>
/// �A�C�e������ǂݍ���
/// </summary>
/// <param name="name"></param>
void ItemBase::LoadData(std::string name)
{

}

/// <summary>
/// ���G��������铖���蔻����쐬
/// </summary>
/// <param name="pos"></param>
/// <param name="radius"></param>
void ItemBase::InitSearch(MyLibrary::LibVec3 pos, float radius)
{
	m_pSearch = std::make_shared<ItemObject>(radius);
	m_pSearch->Init(m_pPhysics, pos);
}
