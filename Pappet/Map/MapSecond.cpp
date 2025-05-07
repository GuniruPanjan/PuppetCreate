#include "MapSecond.h"
#include "MapRest.h"
#include "Manager/EffectManager.h"

namespace
{
	//マップのコリジョンパス
	constexpr const char* cMapCollisionName = "Data/Map/Map2Collision.mv1";

	//エフェクトの再生時間
	int cEffectTime = 50;
	//エフェクト再生
	bool cEffectOne = false;
	//エフェクトの再生位置
	VECTOR cEffectPos = VGet(-10.0f, 50.0f, 0.0f);

	// エフェクトの生成時間
	constexpr int cEffectCreateTime = 20;
	constexpr int cEffectCreateInterval = 30;
	const VECTOR cRestLuminescencePos = VGet(0.0f, 0.0f + 1.0f, 0.0f - 20.0f);
	const VECTOR cRestObjectPos = VGet(0.0f, 0.0f, 0.0f - 30.0f);
	constexpr float cCoreRotationSpeed = 0.001f;
	const VECTOR cCoreScale = VGet(0.0f, 0.0f, 0.0f);

	//シングルトン
	auto& cEffect = EffectManager::GetInstance();
}

/// <summary>
/// コンストラクタ
/// </summary>
MapSecond::MapSecond()
{
}

/// <summary>
/// デストラクタ
/// </summary>
MapSecond::~MapSecond()
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
void MapSecond::DataInit()
{
	//データロード
	LoadData("", cMapCollisionName, "");
	//パーツロード
	LoadPartsData("", "", "", "");

	m_stageName = "stage2";
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="physics"></param>
void MapSecond::Init(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pPhysics = physics;

	//マップのサイズ
	m_size = 0.12f;
	m_width = -400.0f;
	m_hight = -3000.0f;
	m_depth = 0.0f;

	//モデルのサイズ変更
	MV1SetScale(m_collisionHandle, VGet(m_size, m_size, m_size));

	//ライト関係
	ChangeLightTypeDir(VGet(-1.0f, 0.0f, 0.0f));
	m_light = CreateDirLightHandle(VGet(1.0f, 0.0f, 0.0f));

	//索敵判定初期化
	InitSearch(0.0f, m_mapRestPos);
	//ボス部屋入り口
	InitBossRoom(0.0f, m_mapBossEnterPos);
	//ボス部屋入り口
	InitRect(m_width, m_hight, m_depth, m_mapBossEnterPos);
	//コアの判定初期化
	InitCore(0.0f, m_mapCoreCollisionePos);
	//２つ目のエリアに行くための判定
	InitMapSecond(0.0f, m_mapSecondArea);
	//ボス部屋の判定初期化
	InitBossEnter(0.0f, 0.0f, 0.0f, m_mapBossEnterTriggerPos);
}

std::shared_ptr<MapBase> MapSecond::Update(bool warp, bool enter, bool Dead)
{
	MyLibrary::LibVec3::Size size = MyLibrary::LibVec3::Size(m_width, m_hight, m_depth);
	MyLibrary::LibVec3::Size triggerSize = MyLibrary::LibVec3::Size(0.0f, 0.0f, 0.0f);

	m_pSearch->Update(m_mapRestPos);
	m_pBossRoom->Update(m_mapBossRoomPos);
	m_pRect->Update(m_mapBossEnterPos, size);
	m_pRectTrigger->Update(m_mapBossEnterTriggerPos, triggerSize);
	m_pMapSecond->Update(m_mapSecondArea);

	//エフェクトの生成
	if (cEffectTime >= cEffectCreateTime && !Dead)
	{
		cEffect.EffectCreate("Smoke", cEffectPos);

		cEffectTime = 0;
	}
	else
	{
		cEffectTime++;
	}

	//エフェクトの生成
	if (m_effect >= cEffectCreateInterval)
	{
		cEffect.EffectCreate("RestLuminescence", cRestLuminescencePos);

		m_effect = 0;
	}
	else
	{
		m_effect++;
	}

	//エフェクト生成
	if (!cEffectOne && enter && !Dead)
	{
		cEffect.EffectCreate("Distortion", cEffectPos);

		cEffectOne = true;
	}
	else if (!enter)
	{
		cEffectOne = false;
	}

	return shared_from_this();  //自信のポインタを返す
}

std::shared_ptr<MapBase> MapSecond::WarpUpdate(bool warp, bool secondWarp, bool rest)
{
	return shared_from_this();  //自信のポインタを返す
}

void MapSecond::JudgeUpdate()
{
}

void MapSecond::CoreUpdate()
{
}

void MapSecond::Draw()
{
	//3Dモデルのポジション設定
	MV1SetPosition(m_collisionHandle, VGet(0.0f, -200.0f, 0.0f));

	//3Dモデル描画
	MV1DrawModel(m_collisionHandle);
}

void MapSecond::CoreDraw()
{
}

void MapSecond::End(std::shared_ptr<MyLibrary::Physics> physics, bool title)
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
