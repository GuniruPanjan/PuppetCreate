#pragma once
#include "DxLib.h"
#include<memory>

/// <summary>
/// BGMを管理するクラス
/// </summary>
class BgmManager
{
public:
	//コンストラクタ
	BgmManager();
	//デストラクタ
	virtual ~BgmManager();

	//タイトルシーンBGM初期化
	void TitleInit();
	//ゲームシーン休息BGM初期化
	void GameRestInit();
	//ゲームシーン1BGM初期化
	void GameOneInit();
	//ゲームシーン2BGM初期化
	void GameTwoInit();
	//ゲームシーン3BGM初期化
	void GameThreeInit();
	//ゲームシーン4BGM初期化
	void GameFourInit();
	//ゲームシーン5BGM初期化
	void GameFiveInit();
	//クリアシーンBGM初期化
	void ClearInit();
	//更新処理
	void Update(int volume);
	//タイトルBGM更新処理
	void TitleBGM();
	//ゲームBGM更新処理
	void GameBGM();
	//ボスBGM更新処理
	void BossBGM();
	//死亡した時にBGMを止める処理
	void BossStopBGM();
	//クリアBGM更新処理
	void ClearBGM();
	//ゲーム内BGM初期化処理
	void GameEnd();
	//終了処理
	void End();

	//ボスのBGMを流すかの判定をとる
	int GetBossBGM() { return m_bossBGM; }

private:
	//BGM関係
	int m_titleBGM;
	int m_gameBGM;
	int m_bossBGM;
	int m_clearBGM;
	int m_volumePal;
};

