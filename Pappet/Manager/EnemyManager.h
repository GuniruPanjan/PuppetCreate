#pragma once
#include <memory>
#include <map>
#include <string>
#include <List>
#include "Library/MyLibrary.h"

class EnemyBase;
class GameManager;
class Immortal;
class Bear;
class Assassin;
class CoreManager;
class UI;
class SEManager;
class EnemyWeapon;

/// <summary>
/// 敵を管理するクラス
/// </summary>
class EnemyManager
{
public:
	/// <summary>
	/// 敵生成情報
	/// </summary>
	struct EnemyGenerateInfo
	{
		std::string enemyName;     //敵の名前
		int mapNumber;             //マップの番号
		int posX;                  //敵のX座標
		int posY;                  //敵のY座標
		int posZ;                  //敵のZ座標

		bool isCreated = false;    //生成済みかどうか
	};
public:
	//コンストラクタ
	EnemyManager();
	//デストラクタ
	virtual ~EnemyManager();

	//初期化
	void Init(int mapNumber);
	//ゲームの仕様上での初期化
	void GameInit(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, EnemyWeapon& weapon, bool init = false, bool tutorial = false);
	//敵を作る
	void EnemyGenerate(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, EnemyWeapon& weapon, bool tutorial = false);
	//更新
	void Update(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, CoreManager& core, MyLibrary::LibVec3 playerPos, MyLibrary::LibVec3 playerDir, MyLibrary::LibVec3 shieldPos, bool isPlayerChase, SEManager& se, EnemyWeapon& weapon,
		        bool tutorial = false);
	//描画
	void Draw(EnemyWeapon& weapon);
	//UI描画
	void DrawUI(UI& ui);
	//終了処理
	void End();

	//敵のHPを返す
	const int GetHP();
	//敵の最大HPを返す
	const int GetMaxHP();

	//正面の敵の現在のHPを返す
	const int GetFrontEnemyHp() const { return m_frontEnemyHp; }
	//正面の敵の最大HPを返す
	const int GetFrontEnemyMaxHp() const { return m_frontEnemyMaxHp; }
	//敵の情報取得
	std::vector<bool> m_enemyIsDead;
	//敵の座標を返す
	const std::list<MyLibrary::LibVec3>& GetEnemyPos() const { return m_enemyPos; }
	//敵のターゲットを返す
	const std::list<bool> GetEnemyTarget() const { return m_enemyTarget; }
	//敵がプレイヤーに攻撃できるかを返す
	const std::list<bool> GetEnemyAttackHit() const { return m_enemyAttackHit; }
	//与えるダメージ取得
	const std::list<float> GetEnemyDamage() const { return m_damage; }
	//コア取得
	const std::list<int> GetDropCore() const { return m_dropCore; }
	bool SetBossRoom(bool set, int mapNumber);
	//ボスが死んだかの判定
	bool GetBossDead(int mapNumber);
	//ボスかの判断
	const std::list<bool> GetJudg() const { return m_bossJudg; }
private:
	//敵の生成
	void CreateEnemy(float posX, float posY, float posZ, std::string name, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial, EnemyWeapon& weapon);
	//敵のゲームの仕様上での初期化
	void EnemyInit(float posX, float posY, float posZ, std::string name, std::shared_ptr<MyLibrary::Physics> physics, bool tutorial, EnemyWeapon& weapon);
private:
	//敵の管理
	std::list<std::shared_ptr<EnemyBase>> m_pEnemys;
	//敵の生成情報
	std::list<std::shared_ptr<EnemyGenerateInfo>> m_pGenerateInfo;

	std::shared_ptr<Immortal> immortal;
	std::shared_ptr<Bear> bear;
	std::shared_ptr<Assassin> assassin;

	//ステージ名
	const char* m_stageName;

	//ステージ毎の敵の生成数
	std::unordered_map<int, int> m_enemyGenerationCountPerOneMap;

	//正面の敵のHP
	int m_frontEnemyHp;
	//正面の敵の最大HP
	int m_frontEnemyMaxHp;

	//敵の座標
	std::list<MyLibrary::LibVec3> m_enemyPos;
	//敵のターゲット判定
	std::list<bool> m_enemyTarget;
	//敵がプレイヤーに攻撃できるか判定
	std::list<bool> m_enemyAttackHit;
	//敵の攻撃力取得用
	std::list<float> m_damage;
	//コア取得
	std::list<int> m_dropCore;
	//ボスかの判断
	std::list<bool> m_bossJudg;
};

