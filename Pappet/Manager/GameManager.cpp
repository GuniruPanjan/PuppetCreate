#include "GameManager.h"
#include "Character/Player.h"
#include "Character/CollidableNpc.h"
#include "Camera/Camera.h"
#include "Manager/EnemyManager.h"
#include "Ui/Setting.h"
#include "Ui/UI.h"
#include "EffectManager.h"
#include "Manager/SEManager.h"
#include "Item/Tool.h"
#include "External/Font.h"
#include "Manager/FadeManager.h"

namespace
{
	//ワープ時に一回リセットする
	bool cOne = false;
	//ゲームBGMを再生する
	bool cGameBGMOne = false;
	//ボスBGMを再生する
	bool cBossBGMOne = false;
	//休息終了後のフェードイン判定
	bool cFadeIn = false;
	//チュートリアル
	bool cTutorial = false;
	//チュートリアルをクリアした時の判定
	bool cClearTutorial = false;

	//チュートリアルをクリアして跳ぶまでの時間
	float cTutorialTime = 0.0f;
	//一回だけ行う
	bool cOneLoad = false;

	//ロード画面ランダム
	int cLoad = 0;

	//シングルトン
	auto& cEffect = EffectManager::GetInstance();

	//勝利演出リセット
	bool cWinRest[] = {
		false,
	};
}

/// <summary>
/// コンストラクタ
/// </summary>
GameManager::GameManager() :
	m_nowMap(eMapName::TutorialMap),
	m_load(0),
	m_loadNow1(-1),
	m_loadNow2(-1),
	m_deadInit(false),
	m_title(false),
	m_isLoading(false),
	m_restInit(false),
	m_fadeTitle(false),
	m_restMap(false)
{
	m_pUi = std::make_shared<UI>();
	m_pWeapon = std::make_shared<Weapon>();
	m_pShield = std::make_shared<Shield>();
	m_pArmor = std::make_shared<Armor>();
	m_pEnemyWeapon = std::make_shared<EnemyWeapon>();
	m_pCore = std::make_shared<CoreManager>();
	m_pBgm = std::make_shared<BgmManager>();
	m_pMap = std::make_shared<MapManager>();
	m_pSe = std::make_shared<SEManager>();
	m_pFont = std::make_shared<Font>();
}

/// <summary>
/// デストラクタ
/// </summary>
GameManager::~GameManager()
{
	DeleteGraph(m_loadNow1);
	DeleteGraph(m_loadNow2);
}

/// <summary>
/// 初期化
/// </summary>
void GameManager::Init()
{
	cGameBGMOne = false;
	cBossBGMOne = false;

	m_loadNow1 = m_pUi->MyLoadGraph("Data/SceneBack/NowLoading1.png", 1, 1);
	m_loadNow2 = m_pUi->MyLoadGraph("Data/SceneBack/NowLoading2.png", 1, 1);
	
	//0が休息マップデータ
	//1がHARIBOのマップデータ
	//6がチュートリアルマップデータ
	m_pMap->DataInit(6);

	m_pPhysics = std::make_shared<MyLibrary::Physics>(m_pMap->GetCollisionMap());

	m_pMap->Init(m_pPhysics);
	cEffect.Init();

	m_pPlayer = std::make_shared<Player>();
	m_pPlayer->Init(m_pPhysics, this, *m_pWeapon, *m_pShield, *m_pArmor, true);
	m_pPlayer->SetMapNow(GetThisMapName());
	m_pEnemy = std::make_shared<EnemyManager>();
	m_pEnemy->Init(m_nowMap);
	m_pItem = std::make_shared<ItemManager>();
	m_pItem->Init();
	m_pSetting = std::make_shared<Setting>();
	m_pSetting->Init();
	m_pSe->CharaInit();
	m_pSe->BossInit();
	m_pSe->SceneInit();
	m_pUi->Init();
	m_pCore->Init();
	m_pPlayer->ChangeStatus();
	m_pMessage = std::make_shared<MessageManager>();
	m_pMessage->Init();

	//フォントの初期化
	m_pFont->FontInit(32);

	m_pFade = std::make_shared<FadeManager>();
	//フェードアウトイン初期化
	m_pFade->Init();
	
	m_pTool = std::make_shared<Tool>();
	m_pTool->Init();

	m_pBgm->GameOneInit();

	if (m_pMap->GetStageName() == "stageTutorial")
	{
		cTutorial = true;
	}
	else
	{
		cTutorial = false;
	}

	m_load = 30;

	m_isLoading = true;      //必ず各クラスの後につける
}

/// <summary>
/// ゲーム中での初期化処理
/// </summary>
void GameManager::GameInit()
{
	cGameBGMOne = false;
	cBossBGMOne = false;

	//マップを変える時の処理
	ChangeStage(m_pMap->GetStageName());

	m_pPhysics = std::make_shared<MyLibrary::Physics>(m_pMap->GetCollisionMap());

	m_pMap->Init(m_pPhysics);
	cEffect.Init();

	m_pPlayer->Init(m_pPhysics, this, *m_pWeapon, *m_pShield, *m_pArmor, false);
	m_pEnemy->Init(m_nowMap);
	m_pItem->GameInit(m_pPhysics, this);
	m_pMessage->Init();
	m_pSetting->Init();
	m_pUi->Init();
	m_pPlayer->ChangeStatus();

	m_pTool->Init();

	m_load = 30;

	//ステージ１だった場合
	if (m_pMap->GetStageName() == "stage1")
	{
		m_pBgm->GameOneInit();
		cTutorial = false;

	}
	//休息マップだった場合
	else if (m_pMap->GetStageName() == "stageRest")
	{
		m_pBgm->GameRestInit();
		cTutorial = false;

	}
	//チュートリアルステージだった場合
	else if (m_pMap->GetStageName() == "stageTutorial")
	{
		cTutorial = true;
	}
}

/// <summary>
/// 更新処理
/// </summary>
void GameManager::Update()
{
	if (m_isLoading)
	{
		m_pEnemy->EnemyGenerate(m_pPhysics, this, *m_pEnemyWeapon, cTutorial);

		m_pCamera->Update(*m_pPlayer);

		//フェードインされる準備
		m_pFade->SetFade(255);
		m_pFade->SetIn(false);

		if (m_load == 0)
		{
			m_isLoading = false;
		}
	}
	else
	{
		//フェードインが完了していない
		if (!m_pFade->GetIn())
		{
			m_pFade->FadeIn(5);
		}
		
		//フェードアウトさせる
		if (m_pUi->GetReset())
		{
			//フェードアウト可能にする
			m_pFade->SetOut(false);
			m_pFade->FadeOut(5);
		}
		//タイトルに戻る際のフェードアウト
		else if (m_fadeTitle)
		{
			//フェードアウト可能にする
			m_pFade->SetOut(false);
			m_pFade->FadeOut(5);
		}
		//ワープ時のフェードアウト
		else if (m_pPlayer->GetWarp() || m_pSetting->GetRestWarp())
		{
			//フェードアウト可能にする
			m_pFade->SetOut(false);
			m_pFade->FadeOut(10);
		}

		//フェードアウト完了でタイトルにもどる
		if (m_pFade->GetOut())
		{
			m_title = m_pSetting->GetTitle();
		}


		//休息初期化
		if (m_restInit)
		{
			m_pEnemy->EnemyGenerate(m_pPhysics, this, *m_pEnemyWeapon, cTutorial);

			m_restInit = false;
		}

		//ワープしてない時
		if (!m_pPlayer->GetWarp() && !cClearTutorial && !m_pSetting->GetRestWarp())
		{
			m_pBgm->Update(m_pSetting->GetVolume());

			//一回再生
			if (!cGameBGMOne && !m_pMap->GetBossRoom())
			{
				m_pBgm->GameBGM();

				cBossBGMOne = false;
				cGameBGMOne = true;
			}

			m_pPlayer->SetCameraAngle(m_pCamera->GetAngle().y);

			//ロックオンしてない時
			m_pCamera->Update(*m_pPlayer);
			//ボス部屋に入ったらボスをロックオンするようにする
			if (m_pMap->GetBossRoom() && m_pPlayer->GetLock() && !m_pEnemy->GetBossDead(GetThisMapName()))
			{
				m_pCamera->LockBossUpdate(*m_pPlayer, *m_pEnemy);
			}
			//ロックオンしてる時
			else if (m_pPlayer->GetLock())
			{
				m_pCamera->LockUpdate(*m_pPlayer, *m_pEnemy);
			}

			m_pItem->Update(m_pPhysics, this, m_pPlayer->GetTaking());
			m_pMessage->Update(m_pPhysics, this, *m_pPlayer);
			m_pEnemy->Update(m_pPhysics, this, *m_pCore, m_pPlayer->GetPos(), m_pCamera->GetDirection(), m_pPlayer->GetShieldPos(), !m_pPlayer->IsGetPlayerDead(), *m_pSe, *m_pEnemyWeapon, cTutorial);

			m_pPlayer->Update(*m_pWeapon, *m_pShield, *m_pArmor, *m_pEnemy, *m_pCore, m_pMap->GetRestPos(), *m_pTool, *m_pSe, m_pMap->GetBossRoom(), m_pEnemy->GetBossDead(GetThisMapName()), m_pPhysics);

			m_pMap->JudgeUpdate();

			//プレイヤーのボス部屋に入り口判定
			m_pPlayer->SetBossStart(m_pMap->GetBossEnter());

			//休息ができるか
			m_pPlayer->SetRest(m_pMap->GetRest());
			//アイテムを拾えるかどうか
			m_pPlayer->SetItemPick(m_pItem->GetItemPick());
			//メッセージを読めるかどうか
			m_pPlayer->SetMessegePick(false);
			//ボス部屋に入ったか
			m_pEnemy->SetBossRoom(m_pMap->GetBossRoom(), GetThisMapName());
			//ボス部屋に入ったら
			if (m_pMap->GetBossRoom())
			{
				if (!cBossBGMOne)
				{
					//ボスのBGM再生
					m_pBgm->BossBGM();

					cGameBGMOne = false;
					cBossBGMOne = true;
				}

			}

			//ボスが死んだ判定
			if (m_pEnemy->GetBossDead(GetThisMapName()))
			{
				m_pMap->CoreUpdate();

				m_pBgm->BossStopBGM();

				//ステージ１だった場合
				if (m_pMap->GetStageName() == "stage1")
				{
					//クマ
					m_bossEnd.sBear = true;
					//勝利演出を一回だけ行う
					if (!cWinRest[0])
					{
						m_bossEnd.sWin = true;

						cWinRest[0] = true;
					}

				}
				//ステージチュートリアルだった場合
				else if (m_pMap->GetStageName() == "stageTutorial")
				{
					//アサシン
					m_bossEnd.sAssassin = true;

					if (cTutorialTime >= 90.0f)
					{
						//フェードアウト可能にする
						m_pFade->SetOut(false);
						//フェードアウトさせる
						m_pFade->FadeOut(10);
						//フェードアウトが完了
						if (m_pFade->GetOut())
						{
							cClearTutorial = true;
						}
					}
					else
					{
						cTutorialTime++;
					}


				}

				//ワープする
				if (m_pMap->GetCore())
				{
					m_pPlayer->WarpMap();
				}
			}

			m_pMap->Update(m_pPhysics, m_pPlayer->GetWarp(), m_pPlayer->GetBossStart(), m_pEnemy->GetBossDead(GetThisMapName()));

			//メニューを開く
			if (m_pPlayer->GetMenu() && !m_pSetting->GetEquipment() && !m_pSetting->GetItem())
			{
				m_pSetting->MenuUpdate(*m_pPlayer);

				//タイトルに戻る際のフェードアウトをさせる
				if (m_pSetting->GetTitle())
				{
					m_fadeTitle = true;

				}

				m_pPlayer->SetMenu(m_pSetting->GetReturn());
			}
			//装備画面を開く
			else if (m_pSetting->GetEquipment() && !m_pSetting->GetDecision())
			{
				m_pSetting->EquipmentUpdate();
			}
			//アイテム画面を開く
			else if (m_pSetting->GetItem() && !m_pSetting->GetDecision())
			{
				m_pSetting->ItemBoxUpdate();
			}
			//装備選択画面更新
			else if (m_pSetting->GetDecision())
			{
				m_pSetting->EquipmentDecisionUpdate(*m_pWeapon, *m_pShield, *m_pArmor, *m_pItem);
			}
			//メニューを開けるようにする
			else if (!m_pPlayer->GetMenu() && !m_pSetting->GetEquipment() && !m_pSetting->GetItem())
			{
				m_pSetting->SetReturn(true);
			}

			//装備画面とアイテム画面の変更更新
			if (m_pSetting->GetEquipment() || m_pSetting->GetItem())
			{
				if (!m_pSetting->GetDecision())
				{
					m_pSetting->MenuChange();
				}

			}

			//死亡した場合
			if (m_pUi->GetReset() && m_pFade->GetOut())
			{
				//一回だけ実行
				if (m_deadInit == true)
				{
					m_pPlayer->GameInit(m_pPhysics);
					m_pEnemy->GameInit(m_pPhysics, this, *m_pEnemyWeapon, m_deadInit, cTutorial);
					m_pMap->TriggerReset();
					m_pUi->Init();
					m_pPlayer->ChangeStatus();
					m_pTool->Init();

					m_pBgm->BossStopBGM();

					m_deadInit = false;

					m_load = 20;

					m_isLoading = true;
				}
			}
			else
			{
				m_deadInit = true;
			}

			//休息した場合
			if (m_pPlayer->GetRest())
			{
				//レベルアップ処理をしていない場合
				if (!m_pSetting->GetLevel())
				{
					m_pSetting->RestUpdate(*m_pPlayer, *m_pCore, m_restMap);
				}
				//レベルアップ処理
				if (m_pSetting->GetLevel())
				{
					m_pSetting->LevelUpdate(*m_pPlayer, *m_pCore);
					m_pPlayer->ChangeStatus();

				}
				//休息処理
				else if (m_pSetting->GetReset())
				{
					//フェードアウト可能にする
					m_pFade->SetOut(false);
					//フェードアウト実行
					m_pFade->FadeOut(20);
					//フェードアウト完了
					if (m_pFade->GetOut())
					{
						//一回だけ実行
						m_pPlayer->GameInit(m_pPhysics);
						m_pPlayer->ChangeStatus();
						m_pTool->Init();

						//休息地点以外だと初期化
						if (m_nowMap != 0)
						{
							m_pEnemy->GameInit(m_pPhysics, this, *m_pEnemyWeapon, m_pSetting->GetReset(), cTutorial);
						}

						m_pMap->TriggerReset();

						m_pSetting->SetReset(false);

						m_restInit = true;

						cFadeIn = true;
					}
				}

				//一回実行
				if (cFadeIn)
				{
					//フェードイン可能にする
					m_pFade->SetIn(false);
					//フェードイン開始
					m_pFade->FadeIn(20);
					//フェードイン完了
					if (m_pFade->GetIn())
					{
						cFadeIn = false;
					}
				}

			}

			cOne = false;

			//物理更新
			m_pPhysics->Update();
		}
		//ワープしたとき
		else if (m_pPlayer->GetWarp() || cClearTutorial || m_pSetting->GetRestWarp())
		{
			//チュートリアルをクリアしたら強制ワープ
			if (cClearTutorial)
			{
				m_pPlayer->SetWarp(true);
			}

			if (m_pFade->GetOut() || cClearTutorial)
			{
				m_pMap->WarpUpdate(m_pPhysics, m_pPlayer->GetWarp(), false, m_pSetting->GetRestWarp());

				//一回だけ実行
				if (!cOne)
				{
					cEffect.End();
					m_pEnemy->End();
					m_pItem->End();
					m_pBgm->GameEnd();
					m_pSetting->End();
					m_pUi->End();
					GameInit();

					m_pPlayer->SetNotRest(false);
					m_pPlayer->SetWarp(false);
					m_pSetting->SetRestWarp(false);

					cOne = true;
				}

				cClearTutorial = false;

				m_isLoading = true;
			}
		}

		cEffect.Update();
	}
	
}

/// <summary>
/// 描画処理
/// </summary>
void GameManager::Draw()
{
	if (m_isLoading)
	{
		//一回だけ行う
		if (!cOneLoad)
		{
			cLoad = GetRand(1);

			cOneLoad = true;
		}
		//ランダムでロード画面を変える
		if (cLoad == 0)
		{
			DrawGraph(0, 0, m_loadNow1, true);
		}
		else if (cLoad == 1)
		{
			DrawGraph(-100, 0, m_loadNow2, true);
		}

		DrawStringToHandle(0, 0, "NowLoading...", 0xffffff, m_pFont->GetHandle());

		// ロードの進行状況を計算
		int totalLoadTasks = m_load; // 総ロードタスク数を取得する関数（仮定）

		m_load--;

		int remainingLoadTasks = m_load; // 残りのロードタスク数を取得
		float progress = 1.0f - (static_cast<float>(remainingLoadTasks) / static_cast<float>(totalLoadTasks));

		// バーの描画
		int barWidth = 600; // バーの幅
		int barHeight = 20; // バーの高さ
		int barX = 100; // バーのX座標
		int barY = 50; // バーのY座標
		DrawBox(barX, barY, barX + static_cast<int>(barWidth * progress), barY + barHeight, 0x00ff00, TRUE); // プログレスバー
		DrawBox(barX, barY, barX + barWidth, barY + barHeight, 0xffffff, false);
	}
	else
	{
		cOneLoad = false;

		m_pMap->Draw();
		m_pEnemy->Draw(*m_pEnemyWeapon);

		//ロックオンしてる時に描画する
		if (m_pPlayer->GetLock())
		{
			m_pCamera->Draw(*m_pUi);
		}

		m_pPlayer->Draw(*m_pArmor, m_pFont->GetHandle());
		m_pWeapon->Draw();
		m_pShield->Draw();

		//ボスが死んだ判定
		if (m_pEnemy->GetBossDead(GetThisMapName()))
		{
			m_pMap->CoreDraw();
		}

		cEffect.Draw();

		//エネミーのUI描画
		m_pEnemy->DrawUI(*m_pUi);

		m_pUi->Draw(*m_pPlayer, *m_pEnemy, *m_pSetting, *m_pMap, *m_pItem, *m_pWeapon, *m_pShield, *m_pArmor, *m_pTool, *m_pMessage);

		//メニューの背景描画
		if (m_pPlayer->GetMenu())
		{
			m_pSetting->MenuBackDraw();
		}

		//メニュー画面
		if (m_pPlayer->GetMenu() && !m_pSetting->GetEquipment() && !m_pSetting->GetItem())
		{
			m_pSetting->MenuDraw();
		}
		//装備画面
		else if (m_pSetting->GetEquipment() && !m_pSetting->GetDecision())
		{
			m_pSetting->EquipmentDraw(*m_pPlayer);
			m_pUi->EquipmentDraw(*m_pWeapon, *m_pShield, *m_pArmor);
		}
		//アイテム画面
		else if (m_pSetting->GetItem() && !m_pSetting->GetDecision())
		{
			m_pSetting->ItemBoxDraw();
		}
		//装備選択画面描画
		else if (m_pSetting->GetDecision())
		{
			m_pSetting->EquipmentDecisionDraw(*m_pItem);
			EquipmentDraw();
		}

		//アイテム画面と装備画面の変更描画
		if (m_pSetting->GetEquipment() || m_pSetting->GetItem())
		{
			if (!m_pSetting->GetDecision())
			{
				m_pSetting->MenuChangeDraw();
			}
		}

		//休息画面描画
		if (m_pPlayer->GetRest())
		{
			if (m_pSetting->GetLevel())
			{
				m_pSetting->LevelUpDraw(*m_pPlayer, *m_pCore);
			}
			else
			{
				m_pSetting->RestDraw(m_pPlayer->GetBigRest());
			}
		}

		m_pItem->Draw();

		m_pMessage->DrawString();
		m_pMessage->Draw();
		

		//フェードアウトイン描画
		m_pFade->Draw();
		//死亡した時描画する
		if (m_pPlayer->GetDead() && !m_pUi->GetReset())
		{
			m_pUi->DiedDraw(*m_pSe);
		}
		//勝利時の演出を描画する
		if (m_bossEnd.sWin)
		{
			m_pUi->GetCoreDraw(*m_pSe);
			//勝利演出が終わったら終了する
			m_bossEnd.sWin = m_pUi->GetWinReset();
		}

		//ワープできない時の描画
		m_pSetting->CaveatDraw();
	}

}

/// <summary>
/// ステージを変える時の処理
/// </summary>
/// <param name="stageName">ステージ名</param>
void GameManager::ChangeStage(const char* stageName)
{
	//休息地点だった場合
	if (stageName == "stageRest")
	{
		m_nowMap = eMapName::RestMap;
		m_pPlayer->SetMapNow(RestMap);

		m_restMap = true;
	}
	//マップ1だった場合
	if (stageName == "stage1")
	{
		m_nowMap = eMapName::FirstMap;
		m_pPlayer->SetMapNow(FirstMap);
	}
	//チュートリアルだった場合
	if (stageName == "stageTutorial")
	{
		m_nowMap = eMapName::TutorialMap;
		m_pPlayer->SetMapNow(TutorialMap);
	}
}

/// <summary>
/// 終了処理
/// </summary>
void GameManager::End()
{
	m_pPlayer->End();
	m_pCamera->End();
	m_pMap->End(m_pPhysics, false);
	m_pSetting->End();
	m_pEnemy->End();
	m_pMessage->End();

	DeleteGraph(m_loadNow1);
	DeleteGraph(m_loadNow2);
}

/// <summary>
/// 装備の描画処理
/// </summary>
void GameManager::EquipmentDraw()
{
	//右装備
	if (m_pSetting->GetSelect().right)
	{
		m_pUi->RightDraw(*m_pWeapon, *m_pItem);
	}
	//左装備
	else if (m_pSetting->GetSelect().left)
	{
		m_pUi->LeftDraw(*m_pShield, *m_pItem);
	}
	//防具
	else if (m_pSetting->GetSelect().armor)
	{
		m_pUi->ArmorDraw(*m_pArmor, *m_pItem);
	}
}

const MyLibrary::LibVec3 GameManager::GetPlayerPos() const
{
	return MyLibrary::LibVec3();
}
