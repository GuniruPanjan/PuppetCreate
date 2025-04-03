#pragma once
#include "Library/MyLibrary.h"
#include "Object/SearchObject.h"
#include "Object/RectObject.h"
#include "Object/RectObjectTrigger.h"
#include <string>

/// <summary>
/// マップの基底クラス
/// </summary>
class MapBase : public std::enable_shared_from_this<MapBase>
{
public:
	//コンストラクタ
	MapBase();
	//デストラクタ
	virtual ~MapBase();

	//データ初期化
	virtual void DataInit() = 0;
	//初期化
	virtual void Init(std::shared_ptr<MyLibrary::Physics> physics) = 0;
	//終了
	virtual void Finalize(std::shared_ptr<MyLibrary::Physics> physics);
	//更新
	virtual std::shared_ptr<MapBase> Update(bool warp, bool enter, bool Dead) = 0;
	//ワープ更新
	virtual std::shared_ptr<MapBase> WarpUpdate(bool warp, bool rest) = 0;
	//当たったかの判定更新
	virtual void JudgeUpdate() = 0;
	//ボスが死んだら出す
	virtual void CoreUpdate() = 0;
	//判定を初期化する
	void TriggerReset() { m_pBossRoom->IsTriggerReset(); }
	//描画
	virtual void Draw() = 0;
	//コアの描画
	virtual void CoreDraw() = 0;
	//終了
	virtual void End(std::shared_ptr<MyLibrary::Physics> physics, bool title) = 0;

	//終了処理
	void MapEnd(std::shared_ptr<MyLibrary::Physics> physics);

	//マップハンドルを返す
	int GetMap() { return m_handle; }
	//コリジョンのマップハンドルを返す
	int GetCollisionMap() { return m_collisionHandle; }
	//休息できるように当たっているかを返す
	bool GetRest() { return m_pSearch->GetIsStay(); }
	//ボス部屋の入り口
	bool GetBossRoom() { return m_pBossRoom->GetIsTrigger(); }
	//コアの当たり判定を返す
	bool GetCore() { return m_pCore->GetIsStay(); }
	//マップのボス部屋判定を返す
	bool GetBossEnter() { return m_pRectTrigger->GetIsStay(); }
	//マップの休息地点を返す
	VECTOR GetRestPos() { return m_mapRestPos.ConversionToVECTOR(); }
	//ステージ名を返す
	const char* GetStageName() { return m_stageName; }
protected:
	//モデルを読み込む
	void LoadData(std::string mapPath, std::string collisionPath, std::string corePath);
	//モデルパーツを読み込む
	void LoadPartsData(std::string floorPath, std::string wallPath, std::string ceilingPath, std::string restPath);
	//索敵判定をする当たり判定を作成
	void InitSearch(float radius, MyLibrary::LibVec3 pos);
	//ボス部屋の入り口
	void InitBossRoom(float radius, MyLibrary::LibVec3 pos);
	//ボス部屋の入口
	void InitRect(float width, float hight, float depth, MyLibrary::LibVec3 pos);
	//コアの当たり判定
	void InitCore(float radius, MyLibrary::LibVec3 pos);
	//ボス部屋に入るための判定
	void InitBossEnter(float width, float hight, float depth, MyLibrary::LibVec3 pos);
	//モデルパーツ毎に描画する時に使う関数
	void PartDrawSet(int handle, VECTOR scale, VECTOR position, VECTOR rotation);
protected:
	//物理クラスのポインタ
	std::shared_ptr<MyLibrary::Physics> m_pPhysics;

	std::shared_ptr<SearchObject> m_pSearch;             //索敵判定
	std::shared_ptr<SearchObject> m_pBossRoom;           //ボス部屋の入り口判定
	std::shared_ptr<SearchObject> m_pCore;               //コアの当たり判定
	std::shared_ptr<RectObject> m_pRect;                 //矩形判定
	std::shared_ptr<RectObjectTrigger> m_pRectTrigger;   //矩形のトリガー判定

	int m_light;                            //ライト

	int m_handle;                           //モデルのハンドル
	int m_collisionHandle;                  //モデルの当たり判定用のハンドル
	int m_coreHandle;                       //コアのハンドル
	int m_floorHandle;                      //床のハンドル
	int m_wallHandle;                       //壁のハンドル
	int m_ceilingHandle;                    //天井のハンドル
	int m_restObjectHandle;                 //休息のハンドル
	int m_effect;                           //エフェクトの再生
	float m_size;                           //マップのサイズ
	float m_width;                          //幅
	float m_hight;                          //高さ
	float m_depth;                          //奥行
	float m_angle;                          //回転させるアングル
	const char* m_stageName;                //ステージ名

	VECTOR m_mapPos;                                  //マップのポジション
	VECTOR m_mapCollisionPos;                         //マップのコリジョンのポジション
	VECTOR m_mapCorePos;                              //マップのコアポジション
	MyLibrary::LibVec3 m_mapRestPos;                  //休息ポイントのポジション
	MyLibrary::LibVec3 m_mapBossRoomPos;              //ボス部屋入り口のポジション
	MyLibrary::LibVec3 m_mapBossEnterPos;             //ボス部屋入り口のポジション
	MyLibrary::LibVec3 m_mapCoreCollisionePos;        //コアのポジション
	MyLibrary::LibVec3 m_mapBossEnterTriggerPos;      //ボス部屋に入るためのポジション
};

