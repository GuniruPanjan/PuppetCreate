#pragma once
#include "Library/MyLibrary.h"
#include "Map/MapBase.h"
#include <memory>

/// <summary>
/// マップを管理するクラス
/// </summary>
class MapManager
{
public:
	//コンストラクタ
	MapManager();
	//デストラクタ
	virtual ~MapManager();

	//データの初期化
	void DataInit(int map);
	//初期化処理
	void Init(std::shared_ptr<MyLibrary::Physics> physics);
	//更新処理
	void Update(std::shared_ptr<MyLibrary::Physics> physics, bool warp, bool enter, bool Dead);
	//ワープの更新処理
	void WarpUpdate(std::shared_ptr<MyLibrary::Physics> physics, bool warp, bool warpSecond, bool title, bool rest);
	//当たったかの判定処理
	void JudgeUpdate();
	//コアの更新
	void CoreUpdate();
	//判定を初期化する
	void TriggerReset();
	//描画処理
	void Draw();
	//コアの描画
	void CoreDraw();
	//終了処理
	void End(std::shared_ptr<MyLibrary::Physics> physics, bool title);

	//マップハンドルを返す
	int GetMap() { return m_pMap->GetMap(); }

	//コリジョンマップを返す
	int GetCollisionMap() { return m_pMap->GetCollisionMap(); }

	//休息地点を返す
	bool GetRest() { return m_pMap->GetRest(); }

	//ボス部屋
	bool GetBossRoom() { return m_pMap->GetBossRoom(); }

	//コアの当たり判定を返す
	bool GetCore() { return m_pMap->GetCore(); }

	//エリア移動の当たり判定を返す
	bool GetMapSecond() { return m_pMap->GetMapSecond(); }

	//ボス部屋に入るための判定を返す
	bool GetBossEnter() { return m_pMap->GetBossEnter(); }

	//休息地点を返す
	VECTOR GetRestPos() { return m_pMap->GetRestPos(); }

	//ステージ名を返す
	const char* GetStageName() { return m_pMap->GetStageName(); }
private:
	//スマートポインタ
	std::shared_ptr<MapBase> m_pMap;
};

