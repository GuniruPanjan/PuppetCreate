#pragma once
#include "EnemyBase.h"
#include "Library/MyLibrary.h"
#include "DxLib.h"
#include "Manager/SEManager.h"
#include <map>

/// <summary>
/// ボス敵のクラス
/// </summary>
class Bear : public EnemyBase
{
public:
	//コンストラクタ
	Bear();
	//デストラクタ
	virtual ~Bear();

	//初期化
	void Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics) override;
	//ゲームの仕様上での初期化
	void GameInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics) override;
	//更新
	void Update(MyLibrary::LibVec3 playerPos, MyLibrary::LibVec3 shieldPos, bool isChase, SEManager& se, std::shared_ptr<MyLibrary::Physics> physics);
	//ボスの行動
	void Action(MyLibrary::LibVec3 playerPos, bool isChase, SEManager& se);
	//描画
	void Draw(UI& ui);


private:
	MyLibrary::LibVec3 m_attackPos;    //攻撃判定の座標

	int m_walk;                        //歩いた時に流すSE
};

