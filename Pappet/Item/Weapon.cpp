#include "Weapon.h"
#include "Manager/HandleManager.h"
#include "Manager/EffectManager.h"

namespace
{
	//アイテム名
	constexpr const char* cItemName = "Weapon";

	//シングルトン
	auto& handle = HandleManager::GetInstance();
	auto& effect = EffectManager::GetInstance();
}

/// <summary>
/// コンストラクタ
/// </summary>
Weapon::Weapon() :
	m_weaponPatternFrame(0),
	m_weaponTipFrame(0),
	m_weaponPatternName(),
	m_weaponTipName(),
	m_size(0.0f)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Weapon::~Weapon()
{
	//メモリ解放
	MV1DeleteModel(m_itemHandle);
	MV1DeleteModel(m_enemyItemHandle);

	//メモリ解放
	handle.Clear();
}

/// <summary>
/// 初期化処理
/// </summary>
void Weapon::Init()
{
	//黒い剣装備
	if (m_black.sw_equipment)
	{
		m_black.sw_attack = 30.0f;
		m_black.sw_muscle = 2.0f;   //補正B
		m_black.sw_skill = 1.5f;    //補正C
		m_black.sw_radius = 25.0f;
		m_itemHandle = handle.GetModelHandle("Data/Weapon/BlackSword.mv1");

		m_size = 0.1f;

		m_pos = VGet(-3.256f, -1.628f, 4.884f);

		m_weaponMatrixX = 0.224f;
		m_weaponMatrixY = 2.827f;
		m_weaponMatrixZ = 1.616f;

		m_weaponPatternName = "Pattern";
		m_weaponTipName = "Tip";
	}
	//バット装備
	else if (m_bat.sw_equipment)
	{
		m_bat.sw_attack = 20.0f;
		m_bat.sw_muscle = 2.5f;    //補正A
		m_bat.sw_skill = 0.5f;     //補正E
		m_bat.sw_radius = 25.0f;
		m_itemHandle = handle.GetModelHandle("Data/Weapon/Bat.mv1");

		m_size = 1.0f;

		m_pos = VGet(4.884f, 1.628f, -1.628f);

		m_weaponMatrixX = 0.314f;
		m_weaponMatrixY = 0.180f;
		m_weaponMatrixZ = 4.892f;

		m_weaponPatternName = "Sphere";
		m_weaponTipName = "Tip";
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

	MATRIX scale = MGetScale(VGet(m_size, m_size, m_size));
	
	//アタッチするモデルをフレームの座標を原点にするための平行移動行列を作成
	m_transMatrix = MGetTranslate(VScale(m_pos, -1.0f));

	m_transMatrix = MMult(m_transMatrix, MGetRotX(m_weaponMatrixX));
	m_transMatrix = MMult(m_transMatrix, MGetRotY(m_weaponMatrixY));
	m_transMatrix = MMult(m_transMatrix, MGetRotZ(m_weaponMatrixZ));

	m_transMatrix = MMult(m_transMatrix, scale);
	m_mixMatrix = MMult(m_transMatrix, mat);

	MV1SetMatrix(m_itemHandle, m_mixMatrix);
}

/// <summary>
/// 武器の持ちて検索
/// </summary>
/// <returns></returns>
VECTOR Weapon::WeaponPattern()
{
	m_weaponPatternFrame = MV1SearchFrame(m_itemHandle, m_weaponPatternName);

	return MV1GetFramePosition(m_itemHandle, m_weaponPatternFrame);
}

/// <summary>
/// 武器の先端を検索
/// </summary>
/// <returns></returns>
VECTOR Weapon::WeaponTip()
{
	m_weaponTipFrame = MV1SearchFrame(m_itemHandle, m_weaponTipName);

	return MV1GetFramePosition(m_itemHandle, m_weaponTipFrame);
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
/// マップアイテムとしての描画処理
/// </summary>
void Weapon::ItemDraw()
{
	if (m_effectTime <= 30)
	{
		m_effectTime++;
	}
	else
	{
		effect.EffectCreate("Item", m_centerPos.ConversionToVECTOR());

		m_effectTime = 0;
	}
}

/// <summary>
/// 終了処理
/// </summary>
void Weapon::End()
{
	//メモリ解放
	MV1DeleteModel(m_itemHandle);
	MV1DeleteModel(m_enemyItemHandle);

	//メモリ解放
	handle.Clear();
}
