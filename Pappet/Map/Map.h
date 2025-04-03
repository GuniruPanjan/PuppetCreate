#pragma once
#include "DxLib.h"
#include<memory>

class Player;
class Effect;

/// <summary>
/// マップを管理するクラス
/// </summary>
class Map
{
public:
	//コンストラクタ
	Map();
	//デストラクタ
	virtual ~Map();

	//初期化処理
	void Init();

	//描画処理
	void Draw();

	//終了処理
	void End();

	//マップのコリジョンハンドルを返す
	int GetCollisionMap() { return m_collisionHandle; }

	//マップのボス入り口に入ったか返す
	bool GetRoomEntered() { return m_bossRoomEntered; }

	//マップの休息場所を返す
	bool GetSavePossible() { return m_saveSpot; }

	//マップのポジションを返す
	VECTOR GetVectorMapPos() { return m_MapPosition; }

	//マップの休息ポイントを返す
	VECTOR GetRestPos() { return m_restPos; }

private:
	int m_handle;   //マップのデータを入れる変数
	int m_collisionHandle;   //マップのコリジョンのデータを入れる変数
	int m_light;    //ライトを作る変数
	float m_size;   //モデルのサイズ変数
	float m_Xposition;    //Xポジション
	float m_Yposition;    //Yポジション
	float m_Zposition;    //Zポジション
	float m_XCollisionposition;    //Xポジション
	float m_YCollisionposition;    //Yポジション
	float m_ZCollisionposition;    //Zポジション
	bool m_bossRoomEntered;    //ボス部屋に入った判定
	bool m_saveSpot;        //回復スポットを使える判定
	bool m_oneInit;         //一回だけ初期化する
	VECTOR m_MapPosition;   //マップのポジション
	VECTOR m_collisionMapPosition;   //コリジョンのポジション設定
	VECTOR m_restPos;        //休息するポジション

};




