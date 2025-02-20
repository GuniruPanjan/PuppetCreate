#pragma once
#include "SceneBase.h"
#include <memory>
#include <string>
#include "DxLib.h"

//class SceneBase;

/// <summary>
/// シーンを管理するクラス
/// </summary>
class SceneManager
{
public:
	//コンストラクタ
	SceneManager();
	//デストラクタ
	virtual ~SceneManager();

	//初期化処理
	void Init();

	//更新処理
	void Update();

	//描画処理
	void Draw();

	//終了処理
	void End();

	//ステージ名を設定する
	void SetStageName(std::string name) { m_stageName = name; }
	//ステージ名を取得する
	const std::string GetStageName() const { return m_stageName; }

	//プログラムを処理する判定を返す
	bool GetEnd() { return m_pScene->GetEnd(); }

private:

	//スマートポインタ
	std::shared_ptr<SceneBase> m_pScene;

	//ステージ名
	std::string m_stageName;
};




