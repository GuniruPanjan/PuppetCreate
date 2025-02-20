#include "MapFirst.h"
#include "MapRest.h"
#include "Manager/EffectManager.h"

namespace
{
	//マップのパス
	constexpr const char* cMapName = "Data/Map/Map.mv1";
	//マップのコリジョンパス
	constexpr const char* cMapCollisionName = "Data/Map/Collision.mv1";
	//コアのパス
	constexpr const char* cCoreName = "Data/Object/Core.mv1";
	//休息地点の半径
	constexpr float cRestRadius = 50.0f;
	//ボス部屋入り口の半径
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
	//ボス部屋の入り口初期化
	bool cOne = false;

	//エフェクトの再生時間
	int cEffectTime = 50;
	//エフェクト再生
	bool cEffectOne = false;
	//エフェクトの再生位置
	VECTOR cEffectPos = VGet(-10.0f, 50.0f, 0.0f);

	//シングルトン
	auto& cEffect = EffectManager::GetInstance();
}

/// <summary>
/// コンストラクタ
/// </summary>
MapFirst::MapFirst()
{
}

/// <summary>
/// デストラクタ
/// </summary>
MapFirst::~MapFirst()
{
	//メモリ解放
	MV1DeleteModel(m_handle);
	MV1DeleteModel(m_collisionHandle);
	MV1DeleteModel(m_coreHandle);
	DeleteLightHandle(m_light);
}

/// <summary>
/// データ初期化
/// </summary>
void MapFirst::DataInit()
{
	//データロード
	LoadData(cMapName, cMapCollisionName, cCoreName);

	m_stageName = "stage1";
}

/// <summary>
/// 初期化処理
/// </summary>
void MapFirst::Init(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pPhysics = physics;

	//マップのサイズ
	m_size = 0.12f;
	m_width = 20.0f;
	m_hight = 100.0f;
	m_depth = 100.0f;

	//モデルのサイズ変更
	MV1SetScale(m_handle, VGet(m_size, m_size, m_size));
	MV1SetScale(m_collisionHandle, VGet(m_size, m_size, m_size));

	//ポジション設定
	m_mapPos = VGet(0.0f, 250.0f, 0.0f);
	m_mapCollisionPos = VGet(-241.0f, -277.0f, -173.0f);
	m_mapCorePos = VGet(-830.0f, 50.0f, 0.0f);
	m_mapRestPos = MyLibrary::LibVec3(100.0f, 0.0f, -75.0f);
	m_mapBossRoomPos = MyLibrary::LibVec3(-80.0f, 0.0f, 0.0f);
	m_mapBossEnterPos = MyLibrary::LibVec3(-10.0f, 50.0f, 0.0f);
	//m_mapBossEnterPos = MyLibrary::LibVec3(0.0f, 400.0f, 0.0f);
	m_mapCoreCollisionePos = MyLibrary::LibVec3(m_mapCorePos.x, 0.0f, m_mapCorePos.z);
	m_mapBossEnterTriggerPos = MyLibrary::LibVec3(10.0f, 50.0f, 0.0f);

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

	cOne = false;
}

/// <summary>
/// 更新処理
/// </summary>
/// <returns>現在のマップ</returns>
std::shared_ptr<MapBase> MapFirst::Update(bool warp, bool enter, bool Dead)
{
	MyLibrary::LibVec3::Size size = MyLibrary::LibVec3::Size(m_width, m_hight, m_depth);
	MyLibrary::LibVec3::Size triggerSize = MyLibrary::LibVec3::Size(cBossWidth, cBossHight, cBossDepth);

	m_pSearch->Update(m_mapRestPos);
	m_pBossRoom->Update(m_mapBossRoomPos);
	m_pRect->Update(m_mapBossEnterPos, size);
	m_pRectTrigger->Update(m_mapBossEnterTriggerPos, triggerSize);

	//ボスが死んだとき
	if (Dead)
	{
		m_mapBossRoomPos = MyLibrary::LibVec3(-80.0f, 400.0f, 0.0f);
		m_mapBossEnterTriggerPos = MyLibrary::LibVec3(10.0f, 400.0f, 0.0f);

		m_pCore->Update(m_mapCoreCollisionePos);

	}
	else if (!Dead)
	{
		m_pCore->Update(MyLibrary::LibVec3(-1000.0f, -1000.0f, -1000.0f));
	}

	if (enter || Dead)
	{
		//一回だけ実行
		if (!cOne)
		{
			//ボス部屋入り口のポジション
			m_mapBossEnterPos = MyLibrary::LibVec3(-10.0f, 400.0f, 0.0f);

			cOne = true;
		}
	}
	else
	{
		//一回だけ実行
		if (cOne)
		{
			//ボス部屋入り口のポジション
			m_mapBossEnterPos = MyLibrary::LibVec3(-10.0f, 50.0f, 0.0f);

			cOne = false;
		}
	}

	//エフェクトの生成
	if (cEffectTime >= 20 && !Dead)
	{
		cEffect.EffectCreate("Smoke", cEffectPos);

		cEffectTime = 0;
	}
	else
	{
		cEffectTime++;
	}
	//エフェクト生成
	if (!cEffectOne && enter && !Dead)
	{
		cEffect.EffectCreate("Distortion", cEffectPos);

		cEffectOne = true;
	}
	else if(!enter)
	{
		cEffectOne = false;
	}

	return shared_from_this();   //自身のポインタ
}

/// <summary>
/// ワープの更新処理
/// </summary>
/// <param name="warp"></param>
/// <returns></returns>
std::shared_ptr<MapBase> MapFirst::WarpUpdate(bool warp)
{
	if (warp)
	{
		return std::make_shared<MapRest>();
	}

	return shared_from_this();   //自身のポインタ
}

/// <summary>
/// 当たったかの判定
/// </summary>
void MapFirst::JudgeUpdate()
{
	if (m_pSearch->GetIsStay())
	{
		int a = 0;
	}
}

/// <summary>
/// ボスが死んだら出す
/// </summary>
void MapFirst::CoreUpdate()
{
	m_angle += 0.001f;

	if (m_pCore->GetIsStay())
	{
		int a = 1;
	}
}

/// <summary>
/// 描画処理
/// </summary>
void MapFirst::Draw()
{
	//3Dモデルのポジション設定
	MV1SetPosition(m_handle, m_mapPos);
	MV1SetPosition(m_collisionHandle, m_mapCollisionPos);

	//3Dモデル描画
	MV1DrawModel(m_handle);
}

/// <summary>
/// コア描画
/// </summary>
void MapFirst::CoreDraw()
{
	//3Dモデルのポジション設定
	MV1SetPosition(m_coreHandle, m_mapCorePos);

	//3Dモデルの回転
	MV1SetRotationXYZ(m_coreHandle, VGet(0.0f, m_angle, 0.0f));
	//大きさを変える
	MV1SetScale(m_coreHandle, VGet(cCoreSize, cCoreSize, cCoreSize));

	//3Dモデル描画
	MV1DrawModel(m_coreHandle);
}

/// <summary>
/// 終了処理
/// </summary>
void MapFirst::End(std::shared_ptr<MyLibrary::Physics> physics, bool title)
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
