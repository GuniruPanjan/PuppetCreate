#pragma once

/// <summary>
/// メニュー画面のためのシーンクラス
/// </summary>
class MenuScene
{
public:
	//コンストラクタ
	MenuScene();
	//デストラクタ
	virtual ~MenuScene();

	//初期化処理
	void Init();
	//更新処理
	void Update();
	//描画処理
	void Draw();
	//終了処理
	void End();
};

