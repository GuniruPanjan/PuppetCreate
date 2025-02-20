#include "EnemyManager.h"
#include "Character/Immortal.h"
#include "Character/Bear.h"
#include "External/CsvLoad.h"
#include "GameManager.h"
#include "Manager/CoreManager.h"
#include "Manager/SEManager.h"

namespace
{
	int cI = 0;

	int cEnemy = 0;
}

/// <summary>
/// コンストラクタ
/// </summary>
EnemyManager::EnemyManager() :
	m_stageName(""),
	m_frontEnemyHp(0),
	m_frontEnemyMaxHp(0)
{
	
}

/// <summary>
/// デストラクタ
/// </summary>
EnemyManager::~EnemyManager()
{
	//配列の全要素削除
	m_pEnemys.clear();
	m_pGenerateInfo.clear();
	m_enemyPos.clear();
	m_enemyTarget.clear();
	m_damage.clear();
	m_enemyAttackHit.clear();
	m_dropCore.clear();
	m_enemyIsDead.clear();
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="stageName">ステージ名前</param>
void EnemyManager::Init(const char* stageName)
{
	m_pGenerateInfo.clear();
	m_pEnemys.clear();

	m_stageName = stageName;

	//敵生成情報を取得する
	CsvLoad::GetInstance().EnemyDataLoad(m_stageName, m_pGenerateInfo);

	//csv側でマップを指定しているためマップの補正をする
	for (auto& generate : m_pGenerateInfo)
	{
		//マップが1なら
		if (generate->mapNumber == 1)
		{
			//そのマップの出現する敵の数を更新する
			int num = m_enemyGenerationCountPerOneMap[generate->mapNumber];
			num++;
			m_enemyGenerationCountPerOneMap[generate->mapNumber];
		}
	}
}

/// <summary>
/// ゲームの仕様上での初期化
/// </summary>
/// <param name="physics">物理クラス</param>
/// <param name="gameManager">ゲームマネジャー</param>
/// <param name="init">初期化するかどうか</param>
void EnemyManager::GameInit(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, bool init)
{
	//敵の当たり判定とモデル削除
	for (auto& enemy : m_pEnemys)
	{
		//生きている奴
		if (!enemy->GetIsDead())
		{
			//生きているやつを削除
			enemy->Finalize(physics);
		}

		enemy->End();
	}

	if (init)
	{
		m_pEnemys.clear();
	}


	auto thisMapName = gameManager->GetThisMapName();

	if (thisMapName == 1 || thisMapName == 2 || thisMapName == 3 ||
		thisMapName == 4 || thisMapName == 5)
	{
		//敵生成情報をまわして
		for (auto& generate : m_pGenerateInfo)
		{
			//今のマップが一致しているとき
			if (generate->mapNumber == thisMapName)
			{
				//生成済みのキャラを初期化する
				if (generate->isCreated)
				{
					EnemyInit(generate->posX, generate->posY, generate->posZ, generate->enemyName, physics);
				}
			}
		}
	}
}

/// <summary>
/// 更新
/// </summary>
/// <param name="physics">物理クラス</param>
/// <param name="gameManager">ゲームマナジャー</param>
/// <param name="playerPos">プレイヤーポジション</param>
/// <param name="playerDir">プレイヤーの方向</param>
/// <param name="isPlayerChase">プレイヤーを発見したかどうか</param>
void EnemyManager::Update(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, CoreManager& core, MyLibrary::LibVec3 playerPos, MyLibrary::LibVec3 playerDir, MyLibrary::LibVec3 shieldPos, bool isPlayerChase, SEManager& se, bool init)
{
	m_enemyPos.clear();
	m_enemyTarget.clear();
	m_damage.clear();
	m_enemyAttackHit.clear();
	m_dropCore.clear();
	m_bossJudg.clear();
	m_enemyIsDead.clear();


	//今のマップがどのマップか取得する
	auto thisMapName = gameManager->GetThisMapName();

	if (thisMapName == 0 || thisMapName == 1 || thisMapName == 2 ||
		thisMapName == 3 || thisMapName == 4 || thisMapName == 5)
	{
		//敵生成情報をまわして
		for (auto& generate : m_pGenerateInfo)
		{
			//今のマップが一致しているとき
			if (generate->mapNumber == thisMapName)
			{
				//生成済みでなければ
				if (!generate->isCreated)
				{
					if (generate->enemyName != "bear")
					{
						//生成済みにして敵を生成する
						generate->isCreated = true;
						CreateEnemy(generate->posX, generate->posY, generate->posZ, generate->enemyName, physics);
					}
					else if (!gameManager->GetEndBoss().sBear)
					{
						//生成済みにしてボスを生成する
						generate->isCreated = true;
						CreateEnemy(generate->posX, generate->posY, generate->posZ, generate->enemyName, physics);
					}
					
				}
			}
		}
	}
	
	//マップが0以外だと動かす
	if (thisMapName != 0)
	{
		//敵の更新する
		for (auto& enemy : m_pEnemys)
		{
			//物理判定更新
			physics->CheckEnemyUpdate();

			enemy->Update(playerPos, shieldPos, isPlayerChase, se, physics);

			m_enemyPos.emplace_back(enemy->GetPos());
			m_enemyTarget.emplace_back(enemy->GetTarget());
			m_damage.emplace_back(enemy->GetAttack());
			m_enemyAttackHit.emplace_back(enemy->GetPlayerHit());
			m_dropCore.emplace_back(enemy->GetDropCore());
			m_bossJudg.emplace_back(enemy->GetBossJudg());
			m_enemyIsDead.emplace_back(enemy->GetIsDead());

			if (enemy->GetIsDead())
			{
				//一回だけ行う
				if (enemy->GetOne())
				{
					cEnemy++;

					//コア取得
					core.Core(enemy->GetDropCore());

					enemy->SetOne(false);
				}
			}
			
		}
	}

}

/// <summary>
/// 描画
/// </summary>
void EnemyManager::Draw(UI& ui)
{
	//敵を描画する
	for (auto& enemy : m_pEnemys)
	{
		enemy->Draw(ui);
	}

	//DrawFormatString(200, 500, 0xffffff, "%d", cEnemy);
}

/// <summary>
/// 終了処理
/// </summary>
void EnemyManager::End()
{
	for (auto& enemy : m_pEnemys)
	{
		enemy->End();
	}
}

const int EnemyManager::GetHP()
{
	for (auto& enemy : m_pEnemys)
	{
		return enemy->GetHp();
	}
}

const int EnemyManager::GetMaxHP()
{
	for (auto& enemy : m_pEnemys)
	{
		return enemy->GetMaxHp();
	}
}

/// <summary>
/// ボスの部屋に入ったか
/// </summary>
/// <param name="set"></param>
/// <returns></returns>
bool EnemyManager::SetBossRoom(bool set)
{
	return bear->SetBossRoom(set);
}

/// <summary>
/// ボスが死んだかの判定
/// </summary>
/// <returns></returns>
bool EnemyManager::GetBossDead()
{
	return bear->GetBossDead();
}

/// <summary>
/// 敵の生成
/// </summary>
/// <param name="posX">X座標</param>
/// <param name="posY">Y座標</param>
/// <param name="posZ">Z座標</param>
/// <param name="name">キャラクター名</param>
/// <param name="physics">物理ポインタ</param>
void EnemyManager::CreateEnemy(float posX, float posY, float posZ, std::string name, std::shared_ptr<MyLibrary::Physics> physics)
{
	if (name == "Immortal")
	{
		immortal = std::make_shared<Immortal>();
		immortal->Init(posX, posY, posZ, physics);
		m_pEnemys.emplace_back(immortal);
	}
	if (name == "bear")
	{
		bear = std::make_shared<Bear>();
		bear->Init(posX, posY, posZ, physics);
		m_pEnemys.emplace_back(bear);
	}
}

/// <summary>
/// 敵のゲームの仕様上での初期化
/// </summary>
/// <param name="posX">X座標</param>
/// <param name="posY">Y座標</param>
/// <param name="posZ">Z座標</param>
/// <param name="name">キャラクター名</param>
/// <param name="physics">物理ポインタ</param>
void EnemyManager::EnemyInit(float posX, float posY, float posZ, std::string name, std::shared_ptr<MyLibrary::Physics> physics)
{
	if (name == "Immortal")
	{
		immortal = std::make_shared<Immortal>();
		immortal->GameInit(posX, posY, posZ, physics);
		m_pEnemys.emplace_back(immortal);

	}
	if (name == "bear")
	{
		bear = std::make_shared<Bear>();
		bear->GameInit(posX, posY, posZ, physics);
		m_pEnemys.emplace_back(bear);
	}
}
