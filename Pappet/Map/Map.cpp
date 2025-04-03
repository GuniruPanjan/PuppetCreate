#include "Map.h"
#include "Manager/HandleManager.h"

namespace
{
	int effectPlayBack;    //エフェクトを再生させる時間

	//シングルトン
	auto& handle = HandleManager::GetInstance();
}

/// <summary>
/// コンストラクタ
/// </summary>
Map::Map() :
	m_handle(-1),
	m_collisionHandle(-1),
	m_light(0),
	m_size(0.0f),
	m_Xposition(0.0f),
	m_Yposition(0.0f),
	m_Zposition(0.0f),
	m_XCollisionposition(0.0f),
	m_YCollisionposition(0.0f),
	m_ZCollisionposition(0.0f),
	m_bossRoomEntered(false),
	m_saveSpot(false),
	m_oneInit(false)
{
	m_MapPosition = VGet(0.0f, 0.0, 0.0f);
	m_collisionMapPosition = VGet(0.0f, 0.0f, 0.0f);
	m_restPos = VGet(0.0f, 0.0f, 0.0f);
}

/// <summary>
/// デストラクタ
/// </summary>
Map::~Map()
{
	//メモリ解放
	MV1DeleteModel(m_handle);
	MV1DeleteModel(m_collisionHandle);
	DeleteLightHandle(m_light);

	//メモリ解放
	handle.Clear();
}

/// <summary>
/// 初期化処理
/// </summary>
void Map::Init()
{
	m_bossRoomEntered = false;

	effectPlayBack = 50;

	//モデル読み込み
	m_handle = handle.GetModelHandle("Data/Map/Map.mv1");
	m_collisionHandle = handle.GetModelHandle("Data/Map/Collision.mv1");

	//モデルのサイズ
	m_size = 0.12f;

	//ポジション初期化
	m_Xposition = 0.0f;
	m_Yposition = 250.0f;
	m_Zposition = 0.0f;

	m_XCollisionposition = -241.0f;
	m_YCollisionposition = -277.0f;
	m_ZCollisionposition = -173.0f;

	m_restPos = VGet(100.0f, 50.0f, -75.0f);

	//モデルのサイズ変更
	MV1SetScale(m_handle, VGet(m_size, m_size, m_size));
	MV1SetScale(m_collisionHandle, VGet(m_size, m_size, m_size));

	//マップのポジション設定
	m_MapPosition = VGet(m_Xposition, m_Yposition, m_Zposition);
	m_collisionMapPosition = VGet(m_XCollisionposition, m_YCollisionposition, m_ZCollisionposition);

	//ライト関係
	ChangeLightTypeDir(VGet(-1.0f, 0.0f, 0.0f));
	m_light = CreateDirLightHandle(VGet(1.0f, 0.0f, 0.0f));

	if (m_oneInit == false)
	{
		

		m_oneInit = true;
	}

}

void Map::Draw()
{

#if false
	float halfW = m_rectSize.width * 0.5f;
	float halfH = m_rectSize.height * 0.5f;
	float halfD = m_rectSize.depth * 0.5f;

	float right = m_rectPos.x + halfW;
	float left = m_rectPos.x - halfW;
	float top = m_rectPos.y + halfH;
	float bottom = m_rectPos.y - halfH;
	float front = m_rectPos.z - halfD;
	float back = m_rectPos.z + halfD;

	// 横の線
	DrawLine3D(VGet(left, bottom, front), VGet(right, bottom, front), m_color);
	DrawLine3D(VGet(left, top, front), VGet(right, top, front), m_color);
	DrawLine3D(VGet(left, bottom, back), VGet(right, bottom, back), m_color);
	DrawLine3D(VGet(left, top, back), VGet(right, top, back), m_color);
	// 縦の線
	DrawLine3D(VGet(left, top, front), VGet(left, bottom, front), m_color);
	DrawLine3D(VGet(right, top, front), VGet(right, bottom, front), m_color);
	DrawLine3D(VGet(left, top, back), VGet(left, bottom, back), m_color);
	DrawLine3D(VGet(right, top, back), VGet(right, bottom, back), m_color);
	// 前後の線
	DrawLine3D(VGet(left, top, front), VGet(left, top, back), m_color);
	DrawLine3D(VGet(left, bottom, front), VGet(left, bottom, back), m_color);
	DrawLine3D(VGet(right, top, front), VGet(right, top, back), m_color);
	DrawLine3D(VGet(right, bottom, front), VGet(right, bottom, back), m_color);

#endif

#if false
	DrawSphere3D(m_spherePos.GetVector(), m_sphereRadius, 16, m_sphereColor, m_sphereColor, false);

	for (int i = 0; i < 3; i++)
	{
		DrawSphere3D(m_itemPos[i].GetVector(), m_itemRadius, 16, m_itemColor[i], m_itemColor[i], false);
	}

#endif
	//3Dモデルのポジション設定
	MV1SetPosition(m_handle, m_MapPosition);
	MV1SetPosition(m_collisionHandle, m_collisionMapPosition);

	//モデル描画
	MV1DrawModel(m_handle);
}

/// <summary>
/// 終了処理
/// </summary>
void Map::End()
{
	//メモリ解放
	MV1DeleteModel(m_handle);
	MV1DeleteModel(m_collisionHandle);
	DeleteLightHandle(m_light);

	//メモリ解放
	handle.Clear();
}
