#pragma once
#include "EnemyBase.h"

class Weapon;

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
	void Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial, EnemyWeapon& weapon) override;
	//ゲームの仕様上での初期化
	void GameInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial, EnemyWeapon& weapon) override;
	//更新
	void Update(MyLibrary::LibVec3 playerPos, MyLibrary::LibVec3 shieldPos, bool isChase, SEManager& se, std::shared_ptr<MyLibrary::Physics> physics, EnemyWeapon& weapon);
	//行動
	void Action(MyLibrary::LibVec3 playerPos, bool isChase, SEManager& se, EnemyWeapon& weapon);
	//ボスの時の行動
	void BossAction(MyLibrary::LibVec3 playerPos, bool isChase, SEManager& se, EnemyWeapon& weapon);
	//描画
	void Draw(EnemyWeapon& weapon);
	//UI描画
	void DrawUI(UI& ui) override;

private:
	MyLibrary::LibVec3 m_attackPos;       //攻撃判定の座標

	int m_moveFrameRightHand;             //右手のフレーム
	int m_daggerFrame[2];                 //ダガーの当たり判定
	int m_ligLeftLegFrame[2];             //左足のフレーム
	int m_ligRightLegFrame[2];            //右足のフレーム

	VECTOR m_frameRightHand;              //右手のベクター
	VECTOR m_daggerPos[2];                //ダガーの当たり判定
	VECTOR m_ligLeftLegPos[2];            //左足のベクター
	VECTOR m_ligRightLegPos[2];           //右足のベクター

	VECTOR m_attackMove;                  //攻撃時の移動
	VECTOR m_avoidanceMove;               //回避時の移動
	VECTOR m_hitMove;                     //攻撃ヒット時の移動

	MATRIX m_weaponFrameMatrix;
};

