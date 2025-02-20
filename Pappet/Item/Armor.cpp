#include "Armor.h"

namespace
{
	//アイテム名
	constexpr const char* cItemName = "Armor";
}

Armor::Armor()
{
}

Armor::~Armor()
{
}

/// <summary>
/// 初期化
/// </summary>
void Armor::Init()
{
	//平凡な防具
	if (m_common.sa_equipment)
	{
		m_common.sa_defense = 30.0f;

		//モデルパスを書く
		m_path = "Data/Armor/CommonArmor.mv1";
	}
}

/// <summary>
/// マップアイテムとしての初期化処理
/// </summary>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="posZ"></param>
/// <param name="physics"></param>
void Armor::ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
	//代入
	m_pPhysics = physics;

	InitSearch(MyLibrary::LibVec3(posX, posY, posZ), 80.0f);

	m_centerPos = MyLibrary::LibVec3(posX, posY, posZ);
}

void Armor::Update()
{
}

void Armor::ItemUpdate(bool taking)
{
	//アイテムがまだ取られてない時の処理
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
	//アイテムが取られた時の処理
	else if (m_itemPick && taking)
	{
		//一回だけ実行
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
