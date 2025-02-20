#pragma once
#include "SceneBase.h"
#include<memory>

class Map;
class SelectManager;
class MapManager;

//シーンタイトルのクラス
class SceneTitle : public SceneBase
{
public:
	//コンストラクタ
	SceneTitle();
	//デストラクタ
	virtual ~SceneTitle();

	//初期化処理
	virtual void Init();
	//更新処理
	virtual std::shared_ptr<SceneBase> Update();

	/// <summary>
	/// 選択中の場合
	/// </summary>
	/// <param name="select">列挙型</param>
	/// <param name="now">選択してるもの</param>
	/// <param name="other1">それ以外１</param>
	/// <param name="other2">それ以外２</param>
	void SelectBlend(int select, int now, int other1, int other2);

	//描画処理
	virtual void Draw();

	//終了処理
	virtual void End();

private:
	XINPUT_STATE m_xpad;  //パッド入力
	int m_start;      //スタートボタン画像格納変数
	int m_setting;    //設定ボタン画像格納変数
	int m_end;        //終了ボタン画像格納変数
	int m_hand;       //選択してる画像描画
	int m_BButton;    //Bボタン画像格納変数
	int m_AButton;    //Aボタン画像格納変数
	int m_button;    //押した時間を得る
	int m_select[3];    //選択用の変数
	int m_pal[3];      //ブレンド描画用
	bool m_one;      //単発入力
	bool m_blend;    //ブレンド判定
	bool m_setButton;    //設定の判定
	int m_waitTime;      //待つための時間
	VECTOR m_cameraPos;    //カメラポジション
	VECTOR m_cameraTarget;     //カメラのターゲットポジション

	//モデル用変数
	int m_playerHandle;          //プレイヤーのモデル格納変数
	int m_anim;                  //プレイヤーのアニメーション格納変数
	int m_animation;
	float m_totalAnimationTime;
	float m_playTime;
	VECTOR m_pos;

	//スマートポインタ
	//std::shared_ptr<Map> pmap = std::make_shared<Map>();
	std::shared_ptr<SEManager> pse = std::make_shared<SEManager>();
	std::shared_ptr<SelectManager> pselect = std::make_shared<SelectManager>();
	std::shared_ptr<MapManager> m_pMap = std::make_shared<MapManager>();

};



