#pragma once
#include "MapBase.h"

/// <summary>
/// ２つ目のマップクラス
/// </summary>
class MapSecond : public MapBase
{
public:
	//コンストラクタ
	MapSecond();
	//デストラクタ
	virtual ~MapSecond();

	//データの初期化
	virtual void DataInit();
	//初期化
	virtual void Init(std::shared_ptr<MyLibrary::Physics> physics);
	//更新
	virtual std::shared_ptr<MapBase> Update(bool warp, bool enter, bool Dead = false);
	//ワープの更新
	virtual std::shared_ptr<MapBase> WarpUpdate(bool warp, bool secondWarp, bool rest);
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

