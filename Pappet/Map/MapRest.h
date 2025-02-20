#pragma once
#include "MapBase.h"

/// <summary>
/// 休息地点のマップクラス
/// </summary>
class MapRest : public MapBase
{
public:
	//コンストラクタ
	MapRest();
	//デストラクタ
	virtual ~MapRest();

	//データの初期化
	virtual void DataInit();
	//初期化
	virtual void Init(std::shared_ptr<MyLibrary::Physics> physics);
	//更新
	virtual std::shared_ptr<MapBase> Update(bool warp, bool enter, bool Dead = true);
	//ワープの更新
	virtual std::shared_ptr<MapBase> WarpUpdate(bool warp);
	//当たったかの判定
	virtual void JudgeUpdate();
	//ボスが死んだら出す
	virtual void CoreUpdate();
	//描画
	virtual void Draw();
	//コア描画
	virtual void CoreDraw();
	//終了
	virtual void End(std::shared_ptr<MyLibrary::Physics> physics, bool title);
};

