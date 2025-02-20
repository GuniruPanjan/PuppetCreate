#include "Weapon.h"
#include "Manager/HandleManager.h"

namespace
{
	//アイテム名
	constexpr const char* cItemName = "Weapon";

	//行列
	constexpr float cWeaponMatrixY = 5.655f;
	constexpr float cWeaponMatrixZ = 1.795f;

	//シングルトン
	auto& handle = HandleManager::GetInstance();
}

/// <summary>
/// コンストラクタ
/// </summary>
Weapon::Weapon()
{
}

/// <summary>
/// デストラクタ
/// </summary>
Weapon::~Weapon()
{
	//メモリ解放
	MV1DeleteModel(m_itemHandle);

	//メモリ解放
	handle.Clear();
}

/// <summary>
/// 初期化処理
/// </summary>
void Weapon::Init()
{
	if (m_black.sw_equipment)
	{
		m_black.sw_attack = 30.0f;
		m_black.sw_muscle = 2.0f;   //補正B
		m_black.sw_skill = 1.5f;    //補正C
		m_black.sw_radius = 25.0f;
		m_itemHandle = handle.GetModelHandle("Data/Weapon/Sword.mv1");

		m_pos = VGet(-5.0f, 1.947f, -1.947f);
	}
}

/// <summary>
/// マップアイテムとしての初期化処理
/// </summary>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="posZ"></param>
/// <param name="physics"></param>
void Weapon::ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
	//代入
	m_pPhysics = physics;

	InitSearch(MyLibrary::LibVec3(posX, posY, posZ), 80.0f);

	m_centerPos = MyLibrary::LibVec3(posX, posY, posZ);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="mat">行列</param>
void Weapon::Update(MATRIX mat)
{
	MV1SetMatrix(m_itemHandle, MGetIdent());
	//フレーム検索
	m_frameIndex = MV1SearchFrame(m_itemHandle, "2:Sphere");
	//フレームのポジション
	m_framePos = MV1GetFramePosition(m_itemHandle, m_frameIndex);

	m_pos = VAdd(m_framePos, m_pos);
	//アタッチするモデルをフレームの座標を原点にするための平行移動行列を作成
	m_transMatrix = MGetTranslate(VScale(m_pos, -1.0f));

	m_transMatrix = MMult(m_transMatrix, MGetRotY(cWeaponMatrixY));
	m_transMatrix = MMult(m_transMatrix, MGetRotZ(cWeaponMatrixZ));

	m_mixMatrix = MMult(m_transMatrix, mat);

	MV1SetMatrix(m_itemHandle, m_mixMatrix);
}

void Weapon::ItemUpdate(bool taking)
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

/// <summary>
/// 描画処理
/// </summary>
/// <param name="mat">行列</param>
void Weapon::Draw()
{
	//モデル描画
	MV1DrawModel(m_itemHandle);
}

/// <summary>
/// 終了処理
/// </summary>
void Weapon::End()
{
	//メモリ解放
	MV1DeleteModel(m_itemHandle);

	//メモリ解放
	handle.Clear();
}
