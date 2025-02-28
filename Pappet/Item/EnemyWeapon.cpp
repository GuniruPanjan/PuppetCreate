#include "EnemyWeapon.h"
#include "Manager/HandleManager.h"

namespace
{
	//シングルトン
	auto& handle = HandleManager::GetInstance();
}

/// <summary>
/// コンストラクタ
/// </summary>
EnemyWeapon::EnemyWeapon()
{
	for (int i = 0; i < 2; i++)
	{
		m_weaponFrame[i] = 0;
		m_weaponFramePos[i] = VGet(0.0f, 0.0f, 0.0f);
	}
}

/// <summary>
/// デストラクタ
/// </summary>
EnemyWeapon::~EnemyWeapon()
{
	//メモリ解放
	MV1DeleteModel(m_itemHandle);

	//メモリ解放
	handle.Clear();
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="path"></param>
/// <param name="pos"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <param name="size"></param>
void EnemyWeapon::Init(std::string path, VECTOR pos, float x, float y, float z, float size)
{
	m_itemHandle = handle.GetModelHandle(path);

	m_pos = pos;

	m_weaponMatrixX = x;
	m_weaponMatrixY = y;
	m_weaponMatrixZ = z;

	MV1SetScale(m_itemHandle, VGet(size, size, size));
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="mat"></param>
/// <param name="frame"></param>
void EnemyWeapon::Update(MATRIX mat, float size)
{
	MV1SetMatrix(m_itemHandle, MGetIdent());

	MATRIX scale = MGetScale(VGet(size, size, size));

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
/// フレーム検索
/// </summary>
/// <param name="frame1"></param>
/// <param name="frame2"></param>
void EnemyWeapon::WeaponFrame(const char* frame1, const char* frame2)
{
	m_weaponFrame[0] = MV1SearchFrame(m_itemHandle, frame1);
	m_weaponFrame[1] = MV1SearchFrame(m_itemHandle, frame2);

	m_weaponFramePos[0] = MV1GetFramePosition(m_itemHandle, m_weaponFrame[0]);
	m_weaponFramePos[1] = MV1GetFramePosition(m_itemHandle, m_weaponFrame[1]);
}

/// <summary>
/// 描画処理
/// </summary>
void EnemyWeapon::Draw()
{
	//モデル描画
	MV1DrawModel(m_itemHandle);
}

/// <summary>
/// 終了処理
/// </summary>
void EnemyWeapon::End()
{
	//メモリ解放
	MV1DeleteModel(m_itemHandle);

	//メモリ解放
	handle.Clear();
}
