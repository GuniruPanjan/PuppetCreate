#include "MapBase.h"
#include "Manager/HandleManager.h"

namespace
{
	//シングルトン
	auto& handle = HandleManager::GetInstance();
}

/// <summary>
/// コンストラクタ
/// </summary>
MapBase::MapBase() :
	m_light(0),
	m_handle(-1),
	m_collisionHandle(-1),
	m_coreHandle(-1),
	m_size(0.0f),
	m_width(0.0f),
	m_hight(0.0f),
	m_depth(0.0f),
	m_angle(0.0f),
	m_stageName(""),
	m_mapPos(VGet(0.0f,0.0f,0.0f)),
	m_mapCollisionPos(VGet(0.0f,0.0f,0.0f)),
	m_mapCorePos(VGet(0.0f,0.0f,0.0f)),
	m_mapRestPos(),
	m_mapBossEnterPos(),
	m_mapCoreCollisionePos()
{
}

/// <summary>
/// デストラクタ
/// </summary>
MapBase::~MapBase()
{
	//メモリ解放
	MV1DeleteModel(m_handle);
	MV1DeleteModel(m_collisionHandle);
	MV1DeleteModel(m_coreHandle);
}

void MapBase::Finalize(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pSearch->Finalize(physics);
	m_pBossRoom->Finalize(physics);
	m_pCore->Finalize(physics);
	m_pRect->Finalize(physics);
	m_pRectTrigger->Finalize(physics);
}

/// <summary>
/// モデルを読み込む
/// </summary>
/// <param name="mapPath">モデルのパス</param>
/// <param name="collisionPath">コリジョンのモデルパス</param>
void MapBase::LoadData(std::string mapPath, std::string collisionPath, std::string corePath)
{
	m_handle = handle.GetModelHandle(mapPath);
	m_collisionHandle = handle.GetModelHandle(collisionPath);
	m_coreHandle = handle.GetModelHandle(corePath);
}

/// <summary>
/// 索敵をする当たり判定を作成
/// </summary>
/// <param name="radius">半径</param>
/// <param name="posX">X座標</param>
/// <param name="posY">Y座標</param>
/// <param name="posZ">Z座標</param>
void MapBase::InitSearch(float radius, MyLibrary::LibVec3 pos)
{
	m_pSearch = std::make_shared<SearchObject>(radius);
	m_pSearch->Init(m_pPhysics, pos, false, true);
}

/// <summary>
/// ボス部屋の入り口判定を作成
/// </summary>
/// <param name="radius">半径</param>
/// <param name="pos">ポジション</param>
void MapBase::InitBossRoom(float radius, MyLibrary::LibVec3 pos)
{
	m_pBossRoom = std::make_shared<SearchObject>(radius);
	m_pBossRoom->Init(m_pPhysics, pos, false, false, false, true);
}

/// <summary>
/// 矩形の当たり判定を作成
/// </summary>
/// <param name="width">幅</param>
/// <param name="hight">高さ</param>
/// <param name="depth">奥行</param>
void MapBase::InitRect(float width, float hight, float depth, MyLibrary::LibVec3 pos)
{
	m_pRect = std::make_shared<RectObject>(width, hight, depth);
	m_pRect->Init(m_pPhysics, pos, true);
}

/// <summary>
/// コアの当たり判定を作成
/// </summary>
/// <param name="radius">半径</param>
/// <param name="pos">ポジション</param>
void MapBase::InitCore(float radius, MyLibrary::LibVec3 pos)
{
	m_pCore = std::make_shared<SearchObject>(radius);
	m_pCore->Init(m_pPhysics, pos, false, false, false, false, true);
}

/// <summary>
/// ボス部屋入り口の当たり判定を作成
/// </summary>
/// <param name="width"></param>
/// <param name="hight"></param>
/// <param name="depth"></param>
/// <param name="pos"></param>
void MapBase::InitBossEnter(float width, float hight, float depth, MyLibrary::LibVec3 pos)
{
	m_pRectTrigger = std::make_shared<RectObjectTrigger>(width, hight, depth);
	m_pRectTrigger->Init(m_pPhysics, pos, true);
}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="physics"></param>
void MapBase::MapEnd(std::shared_ptr<MyLibrary::Physics> physics)
{
	handle.Clear();
}
