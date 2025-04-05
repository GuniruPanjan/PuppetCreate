#pragma once
#include "DxLib.h"
#include<memory>

/// <summary>
/// SEを管理するクラス
/// </summary>
class SEManager
{
public:
	//コンストラクタ
	SEManager();
	//デストラクタ
	virtual ~SEManager();

	//シーンのSE初期化処理
	void SceneInit();
	//キャラSE初期化処理
	void CharaInit();
	//ボスSE初期化処理
	void BossInit();
	//更新処理
	void Update(int volume);
	//終了処理
	void End();

	int GetButtonSE() { return m_buttonSE; }              //ボタンのSEをとる
	int GetSelectSE() { return m_selectSE; }              //選択のSEをとる
	int GetHitSE() { return m_hitSE; }                    //攻撃がヒットした時のSEをとる
	int GetPlayerHitSE() { return m_playerHitSE; }        //プレイヤーに攻撃が当たった時のSEをとる
	int GetAttackSE() { return m_attackSE; }              //攻撃時のSEをとる
	int GetWalkSE() { return m_walkSE; }                  //歩く時のSEをとる
	int GetHeelSE() { return m_playerHeelSE; }            //回復した時のSEをとる
	int GetRestSE() { return m_playerRestSE; }            //休息した時のSEをとる
	int GetItemSE() { return m_itemGetSE; }               //アイテムを取った時のSEをとる
	int GetBossVoiceSE() { return m_bossVoiceSE; }        //ボスの叫びのSEをとる
	int GetBossAttackSE1() { return m_bossAttackSE[0]; }  //ボスの攻撃１のSEをとる
	int GetBossAttackSE2() { return m_bossAttackSE[1]; }  //ボスの攻撃2のSEをとる
	int GetBossAttackSE3() { return m_bossAttackSE[2]; }  //ボスの攻撃3のSEをとる
	int GetBossWalkSE() { return m_bossWalkSE; }          //ボスが歩いた時のSEをとる
	int GetKickSE() { return m_kickSE; }                  //キックのSEをとる
	int GetKnifeSE() { return m_knifeSE; }                //ナイフのSEをとる
	int GetDiedSE() { return m_diedSE; }                  //死んだ時のSEをとる
	int GetLoserSE() { return m_loserSE; }                //負けた時のSEをとる
	int GetVictorySE() { return m_victorySE; }            //勝った時のSEをとる

private:
	//SE関係
	int m_buttonSE;         //ボタンのSE
	int m_selectSE;         //選択した時のSE
	int m_hitSE;            //エネミーとプレイヤーとボスのHit時のSE
	int m_playerHitSE;      //ボスの攻撃のHit時のSE
	int m_attackSE;         //エネミーとプレイヤーの攻撃時のSE
	int m_walkSE;           //エネミーとプレイヤーの歩く時のSE
	int m_playerHeelSE;     //回復した時のSE
	int m_playerRestSE;     //休息した時のSE
	int m_itemGetSE;        //アイテムを取った時のSE
	int m_bossVoiceSE;      //ボスの咆哮SE
	int m_bossAttackSE[3];  //ボスの攻撃３パターン
	int m_bossWalkSE;       //ボスが歩く時のSE
	int m_kickSE;           //キックのSE
	int m_knifeSE;          //ナイフのSE
	int m_diedSE;           //死んだ時のSE
	int m_loserSE;          //負けた時のSE
	int m_victorySE;        //勝った時のSE
	int m_volumePal;        //音量
};

