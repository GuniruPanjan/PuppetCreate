#pragma once
#include "EnemyBase.h"

/// <summary>
/// アサシンクラス
/// </summary>
class Assassin : public EnemyBase
{
public:
	//コンストラクタ
	Assassin();
	//デストラクタ
	virtual ~Assassin();

	//初期化
	void Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial) override;
	//ゲームの仕様上での初期化
	void GameInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial) override;
	//更新
	void Update(MyLibrary::LibVec3 playerPos, MyLibrary::LibVec3 shieldPos, bool isChase, SEManager& se, std::shared_ptr<MyLibrary::Physics> physics);
	//行動
	void Action(MyLibrary::LibVec3 playerPos, bool isChase, SEManager& se);
	//ボスの時の行動
	void BossAction(MyLibrary::LibVec3 playerPos, bool isChase, SEManager& se);
	//描画
	void Draw(UI& ui);

private:
	MyLibrary::LibVec3 m_attackPos;   //攻撃判定の座標

	int m_moveFrameRightHand;         //右手のフレーム
	int m_moveFrameLeftLeg;           //左足のフレーム
	int m_moveFrameRightLeg;          //右足のフレーム

	VECTOR m_frameRightHand;          //右手のベクター
	VECTOR m_frameLeftLeg;            //左足のベクター
	VECTOR m_frameRightLeg;           //右足のベクター
};

