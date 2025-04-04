#include "Shield.h"
#include "Manager/HandleManager.h"
#include "Manager/EffectManager.h"

namespace
{
	//アイテム名
	constexpr const char* cItemName = "Shield";

	//シングルトン
	auto& handle = HandleManager::GetInstance();
	auto& effect = EffectManager::GetInstance();
}

/// <summary>
/// コンストラクタ
/// </summary>
Shield::Shield()
{
}

/// <summary>
/// デストラクタ
/// </summary>
Shield::~Shield()
{
	//メモリ解放
	MV1DeleteModel(m_itemHandle);

	handle.Clear();
}

/// <summary>
/// 初期化処理
/// </summary>
void Shield::Init()
{
	if (m_ugly.ss_equipment)
	{
		m_itemHandle = handle.GetModelHandle("Data/Shield/Shield.mv1");
		m_ugly.ss_cut = 90.0f;
		m_ugly.ss_strength = 100.0f;

		m_pos = VGet(0.0f, 0.0f, 7.787f);

		m_shieldMatrixX = 0.0f;
		m_shieldMatrixY = 3.142f;
		m_shieldMatrixZ = 0.0f;
	}
	else if (m_wood.ss_equipment)
	{
		m_itemHandle = handle.GetModelHandle("Data/Shield/WoodenShield.mv1");
		m_wood.ss_cut = 60.0f;
		m_wood.ss_strength = 50.0f;

		m_pos = VGet(0.0f, 0.0f, 6.512f);

		m_shieldMatrixX = 0.0f;
		m_shieldMatrixY = 3.231f;
		m_shieldMatrixZ = 1.750f;

	}
}

/// <summary>
/// マップアイテムとしての初期化処理
/// </summary>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="posZ"></param>
/// <param name="physics"></param>
void Shield::ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
	//代入
	m_pPhysics = physics;

	InitSearch(MyLibrary::LibVec3(posX, posY, posZ), 80.0f);

	m_centerPos = MyLibrary::LibVec3(posX, posY, posZ);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="mat">左手の行列</param>
void Shield::Update(MATRIX mat)
{
	MV1SetMatrix(m_itemHandle, MGetIdent());

	//アタッチするモデルをフレームの座標を原点にするための平行移動行列を作成
	m_transMatrix = MGetTranslate(VScale(m_pos, -1.0f));

	m_transMatrix = MMult(m_transMatrix, MGetRotX(m_shieldMatrixX));
	m_transMatrix = MMult(m_transMatrix, MGetRotY(m_shieldMatrixY));
	m_transMatrix = MMult(m_transMatrix, MGetRotZ(m_shieldMatrixZ));

	m_mixMatrix = MMult(m_transMatrix, mat);

	MV1SetMatrix(m_itemHandle, m_mixMatrix);
}

void Shield::ItemUpdate(bool taking)
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
void Shield::Draw()
{
	//モデル描画
	MV1DrawModel(m_itemHandle);
}

/// <summary>
/// マップアイテムとしての描画処理
/// </summary>
void Shield::ItemDraw()
{
	if (m_effectTime <= m_effectTimeMax)
	{
		m_effectTime++;
	}
	else
	{
		effect.EffectCreate("Item", m_centerPos.ConversionToVECTOR());

		m_effectTime = 0;
	}
}

void Shield::End()
{
	//メモリ解放
	MV1DeleteModel(m_itemHandle);

	handle.Clear();
}
