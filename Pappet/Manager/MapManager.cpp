#include "MapManager.h"
#include "Map/MapFirst.h"
#include "Map/MapRest.h"
#include "Map/MapTutorial.h"
#include "Map/MapSecond.h"

/// <summary>
/// コンストラクタ
/// </summary>
MapManager::MapManager() :
	m_pMap(nullptr)
{

}

/// <summary>
/// デストラクタ
/// </summary>
MapManager::~MapManager()
{
}

/// <summary>
/// データの初期化
/// </summary>
void MapManager::DataInit(int map)
{
	if (map == 0)
	{
		//メモリを読み込む
		m_pMap = std::make_shared<MapRest>();

		m_pMap->DataInit();
	}
	else if (map == 1)
	{
		//メモリを読み込む
		m_pMap = std::make_shared<MapFirst>();

		m_pMap->DataInit();
	}
	else if (map == 2)
	{
		//メモリを読み込む
		m_pMap = std::make_shared<MapSecond>();

		m_pMap->DataInit();
	}
	else if (map == 6)
	{
		//メモリを読み込む
		m_pMap = std::make_shared<MapTutorial>();

		m_pMap->DataInit();
	}
	
}

/// <summary>
/// 初期化処理
/// </summary>
void MapManager::Init(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pMap->Init(physics);
}

/// <summary>
/// 更新処理
/// </summary>
void MapManager::Update(std::shared_ptr<MyLibrary::Physics> physics, bool warp, bool enter, bool Dead)
{
	std::shared_ptr<MapBase> pNext = m_pMap->Update(warp, enter, Dead);
}

/// <summary>
/// ワープでの更新処理
/// </summary>
/// <param name="physics"></param>
/// <param name="warp"></param>
void MapManager::WarpUpdate(std::shared_ptr<MyLibrary::Physics> physics, bool warp, bool warpSecond, bool title, bool rest)
{
	std::shared_ptr<MapBase> pNext = m_pMap->WarpUpdate(warp, warpSecond, rest);
	if (pNext != m_pMap)
	{
		//現在のマップの終了処理
		m_pMap->End(physics, title);
		m_pMap->MapEnd(physics);

		m_pMap = pNext;

		//データダウンロード
		m_pMap->DataInit();
	}
}

/// <summary>
/// 当たったかの判定更新処理
/// </summary>
void MapManager::JudgeUpdate()
{
	m_pMap->JudgeUpdate();
}

/// <summary>
/// コア更新
/// </summary>
void MapManager::CoreUpdate()
{
	m_pMap->CoreUpdate();
}

/// <summary>
/// 判定を初期化する
/// </summary>
void MapManager::TriggerReset()
{
	m_pMap->TriggerReset();
}

/// <summary>
/// 描画処理
/// </summary>
void MapManager::Draw()
{
	m_pMap->Draw();
}

/// <summary>
/// コア描画
/// </summary>
void MapManager::CoreDraw()
{
	m_pMap->CoreDraw();
}

/// <summary>
/// 終了処理
/// </summary>
void MapManager::End(std::shared_ptr<MyLibrary::Physics> physics, bool title)
{
	m_pMap->End(physics, title);
}
