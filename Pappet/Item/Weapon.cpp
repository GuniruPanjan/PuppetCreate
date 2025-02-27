#include "Weapon.h"
#include "Manager/HandleManager.h"

namespace
{
	//アイテム名
	constexpr const char* cItemName = "Weapon";

	//行列
	constexpr float cWeaponMatrixX = 0.0f;
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
	MV1DeleteModel(m_enemyItemHandle);

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
/// 敵の装備初期化処理
/// </summary>
void Weapon::EnemyInit(std::string path, VECTOR pos, float x, float y, float z)
{
	m_enemyItemHandle = handle.GetModelHandle(path);

	m_pos = pos;

	m_weaponMatrixX = x;
	m_weaponMatrixY = y;
	m_weaponMatrixZ = z;
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

/// <summary>
/// 敵の装備更新処理
/// </summary>
/// <param name="mat"></param>
void Weapon::EnemyUpdate(MATRIX mat, const char* frame)
{
	MV1SetMatrix(m_enemyItemHandle, MGetIdent());
	//フレーム検索
	m_frameIndex = MV1SearchFrame(m_enemyItemHandle, frame);
	//フレームのポジション
	m_framePos = MV1GetFramePosition(m_enemyItemHandle, m_frameIndex);

	m_pos = VAdd(m_framePos, m_pos);
	//アタッチするモデルをフレームの座標を原点にするための平行移動行列を作成
	m_transMatrix = MGetTranslate(VScale(m_pos, -1.0f));

	m_transMatrix = MMult(m_transMatrix, MGetRotX(m_weaponMatrixX));
	m_transMatrix = MMult(m_transMatrix, MGetRotY(m_weaponMatrixY));
	m_transMatrix = MMult(m_transMatrix, MGetRotZ(m_weaponMatrixZ));

	m_mixMatrix = MMult(m_transMatrix, mat);

	MV1SetMatrix(m_enemyItemHandle, m_mixMatrix);
	//MV1SetPosition(m_enemyItemHandle, m_pos);
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
/// 敵の描画処理
/// </summary>
void Weapon::EnemyDraw(float size)
{
	MV1SetScale(m_enemyItemHandle, VGet(size, size, size));
	//モデル描画
	MV1DrawModel(m_enemyItemHandle);
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
