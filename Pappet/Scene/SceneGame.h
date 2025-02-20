#pragma once
#include "SceneBase.h"
#include "Manager/GameManager.h"

/// <summary>
/// ゲームシーンを管理するクラス
/// </summary>
class SceneGame : public SceneBase
{
public:
	//コンストラクタ
	SceneGame();

	//デストラクタ
	virtual ~SceneGame();

	//初期化処理
	virtual void Init();

	//更新処理
	virtual std::shared_ptr<SceneBase> Update();

	//描画処理
	virtual void Draw();

	//終了処理
	virtual void End();

private:
	//スマートポインタ管理
	std::shared_ptr<GameManager> game = std::make_shared<GameManager>();
};
