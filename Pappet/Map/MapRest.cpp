#include "MapRest.h"
#include "MapFirst.h"
#include "Manager/EffectManager.h"

namespace
{
	//マップのパス
	constexpr const char* cMapName = "Data/Map/RestMap.mv1";
	//マップのコリジョンパス
	constexpr const char* cMapCollisionName = "Data/Map/RestMapCollision.mv1";
	//コアのパス
	constexpr const char* cCoreName = "Data/Object/Core.mv1";
	//休息地点の半径
	constexpr float cRestRadius = 100.0f;
	//ボス部屋入口の半径
	constexpr float cBossRommRadius = 60.0f;
	//コアの半径
	constexpr float cCoreRadius = 70.0f;
	//コアのサイズ
	constexpr float cCoreSize = 0.5f;
	//ボス部屋の幅
	constexpr float cBossWidth = 20.0f;
	//ボス部屋の横
	constexpr float cBossHight = 100.0f;
	//ボス部屋の奥行
	constexpr float cBossDepth = 30.0f;

	int cEffectPlay = 500;

	//シングルトン
	auto& cEffect = EffectManager::GetInstance();
}

/// <summary>
/// コンストラクタ
/// </summary>
MapRest::MapRest()
{
}

/// <summary>
/// デストラクタ
/// </summary>
MapRest::~MapRest()
{
	//メモリ解放
	MV1DeleteModel(m_handle);
	MV1DeleteModel(m_collisionHandle);
	MV1DeleteModel(m_coreHandle);
	DeleteLightHandle(m_light);
}

/// <summary>
/// データの初期化
/// </summary>
void MapRest::DataInit()
{
	//データロード
	LoadData(cMapName, cMapCollisionName, cCoreName);

	m_stageName = "stageRest";
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="physics"></param>
void MapRest::Init(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pPhysics = physics;

	//マップのサイズ
	m_size = 0.01f;

	//モデルのサイズ変更
	MV1SetScale(m_handle, VGet(m_size, m_size, m_size));
	MV1SetScale(m_collisionHandle, VGet(m_size, m_size, m_size));

	m_mapPos = VGet(-200.0f, -25.0f, 0.0f);
	m_mapCollisionPos = VGet(-220.0f, -40.0f, 395.0f);
	//m_mapCollisionPos = VGet(0.0f, 0.0f, 0.0f);
	m_mapCorePos = VGet(-910.0f, 0.0f, 380.0f);
	m_mapRestPos = MyLibrary::LibVec3(-180.0f, 0.0f, -200.0f);
	m_mapBossRoomPos = MyLibrary::LibVec3(0.0f, 0.0f, 0.0f);
	m_mapBossEnterPos = MyLibrary::LibVec3(0.0f, 1000.0f, 0.0f);
	m_mapCoreCollisionePos = MyLibrary::LibVec3(m_mapCorePos.x, 0.0f, m_mapCorePos.z);
	m_mapBossEnterTriggerPos = MyLibrary::LibVec3(10.0f, 1000.0f, 0.0f);

	//炎のエフェクト
	cEffect.EffectLoad("Fire", "Data/Effect/MagicFire1.efkefc", 600, 90.0f);

	//ライト関係
	ChangeLightTypeDir(VGet(-1.0f, 0.0f, 0.0f));
	m_light = CreateDirLightHandle(VGet(1.0f, 0.0f, 0.0f));

	//索敵判定初期化
	InitSearch(cRestRadius, m_mapRestPos);
	//ボス部屋入り口
	InitBossRoom(cBossRommRadius, m_mapBossEnterPos);
	//ボス部屋入り口
	InitRect(m_width, m_hight, m_depth, m_mapBossEnterPos);
	//コアの判定初期化
	InitCore(cCoreRadius, m_mapCoreCollisionePos);
	//ボス部屋の判定初期化
	InitBossEnter(cBossWidth, cBossHight, cBossDepth, m_mapBossEnterTriggerPos);

	cEffectPlay = 500.0f;
}

/// <summary>
/// 更新処理
/// </summary>
/// <returns></returns>
std::shared_ptr<MapBase> MapRest::Update(bool warp, bool enter, bool Dead)
{
	m_pSearch->Update(m_mapRestPos);
	m_pCore->Update(m_mapCoreCollisionePos);

	if (cEffectPlay >= 500)
	{
		cEffect.EffectCreate("Fire", VGet(m_mapRestPos.x, m_mapRestPos.y - 30, m_mapRestPos.z - 50));

		cEffectPlay = 0;
	}
	else
	{
		cEffectPlay++;
	}

	return shared_from_this();  //自身のポインタ
}

/// <summary>
/// ワープの更新処理
/// </summary>
/// <param name="warp"></param>
/// <returns></returns>
std::shared_ptr<MapBase> MapRest::WarpUpdate(bool warp, bool rest)
{
	if (warp)
	{
		return std::make_shared<MapFirst>();
	}
	else if (rest)
	{
		return std::make_shared<MapRest>();
	}

	return shared_from_this();  //自身のポインタ
}

/// <summary>
/// 当たったかの判定
/// </summary>
void MapRest::JudgeUpdate()
{
}

/// <summary>
/// ボスが死んだら出す
/// </summary>
void MapRest::CoreUpdate()
{
}

/// <summary>
/// 描画処理
/// </summary>
void MapRest::Draw()
{
	//3Dモデルのポジション設定
	MV1SetPosition(m_handle, m_mapPos);
	MV1SetPosition(m_collisionHandle, m_mapCollisionPos);

	//3Dモデル描画
	MV1DrawModel(m_handle);
	//MV1DrawModel(m_collisionHandle);
}

/// <summary>
/// コア描画処理
/// </summary>
void MapRest::CoreDraw()
{
}

/// <summary>
/// 終了処理
/// </summary>
/// <param name="physics"></param>
void MapRest::End(std::shared_ptr<MyLibrary::Physics> physics, bool title)
{
	//メモリ解放
	MV1DeleteModel(m_handle);
	MV1DeleteModel(m_collisionHandle);
	MV1DeleteModel(m_coreHandle);
	DeleteLightHandle(m_light);

	if (!title)
	{
		Finalize(physics);
	}
}
