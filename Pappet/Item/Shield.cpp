#include "Shield.h"
#include "Manager/HandleManager.h"

namespace
{
	//アイテム名
	constexpr const char* cItemName = "Shield";

	//武器の行列
	constexpr float cShieldMatrixY = 3.142f;
	constexpr float cShieldMatrixZ = 0.0f;

	//シングルトン
	auto& handle = HandleManager::GetInstance();
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
		m_itemHandle = handle.GetModelHandle("Data/Weapon/Shield.mv1");
		m_ugly.ss_cut = 90.0f;
		m_ugly.ss_strength = 20.0f;

		m_pos = VGet(0.0f, 0.0f, 7.787f);
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

	//フレームを検索
	m_frameIndex = MV1SearchFrame(m_itemHandle, "0:fbx");

	m_framePos = MV1GetFramePosition(m_itemHandle, m_frameIndex);

	m_pos = VAdd(m_framePos, m_pos);

	//アタッチするモデルをフレームの座標を原点にするための平行移動行列を作成
	m_transMatrix = MGetTranslate(VScale(m_pos, -1.0f));

	m_transMatrix = MMult(m_transMatrix, MGetRotY(cShieldMatrixY));
	m_transMatrix = MMult(m_transMatrix, MGetRotZ(cShieldMatrixZ));

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

void Shield::End()
{
	//メモリ解放
	MV1DeleteModel(m_itemHandle);

	handle.Clear();
}
