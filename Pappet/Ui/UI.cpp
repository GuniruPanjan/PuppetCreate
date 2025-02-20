#include "UI.h"
#include "Item/Weapon.h"
#include "Item/Shield.h"
#include "Item/Armor.h"
#include "Character/Player.h"
#include "Manager/EnemyManager.h"
#include "Manager/ItemManager.h"
#include "Manager/MapManager.h"
#include "Item/Tool.h"
#include "Manager/MessageManager.h"

namespace
{
	int c_maxHP = 800;
	int c_maxHPWidth = 1000;

	bool c_itemTakingUI = false;
}

/// <summary>
/// コンストラクタ
/// </summary>
UI::UI() :
	m_equipmentReturn(false),
	m_xpad()
{
}

/// <summary>
/// デストラクタ
/// </summary>
UI::~UI()
{
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="player"></param>
/// <param name="enemy"></param>
void UI::Init()
{
	m_deadBack = 0;
	m_deadA = 0;
	m_waitResetTime = 0;
	m_deadReset = false;

	m_heelStone = MyLoadGraph("Data/UI/HeelStoneMini.png", 4, 4);
	m_blackSword = MyLoadGraph("Data/UI/黒い剣UI.png", 3, 3);
	m_fist = MyLoadGraph("Data/UI/拳UI.png", 3, 3);
	m_body = MyLoadGraph("Data/UI/裸体UI.png", 3, 3);
	m_uglyShield = MyLoadGraph("Data/UI/醜い盾UI.png", 4, 4);
	m_commonArmor = MyLoadGraph("Data/UI/平凡な鎧UI.png", 3, 3);
	m_dead = MyLoadGraph("Data/UI/YOUDIEDGraph.png", 1, 1);
	m_backRightBar = MyLoadGraph("Data/UI/StatusBar右端.png", 2, 2);
	m_backLeftBar = MyLoadGraph("Data/UI/StatusBar左端.png", 2, 2);
	m_backCenterBar = MyLoadGraph("Data/UI/StatusBar中央.png", 2, 2);
	m_backOutBar = MyLoadGraph("Data/UI/StatusBar外枠.png", 2, 2);
	m_hpBar = MyLoadGraph("Data/UI/HPBarRed.png", 2, 2);
	m_staminaBar = MyLoadGraph("Data/UI/StaminaBar.png", 2, 2);
	m_coreBackBar = MyLoadGraph("Data/UI/CoreBar.png", 1, 1);
	m_statusIcon = MyLoadGraph("Data/UI/StatusIcon.png", 1, 1);
	m_equipmentFrame = MyLoadGraph("Data/UI/Frame.png", 1, 1);
	m_actionUI = MyLoadGraph("Data/UI/Action.png", 1, 1);
	m_itemTaking = MyLoadGraph("Data/UI/アイテム取得囲い.png", 1, 1);
	m_yButton = MyLoadGraph("Data/UI/YButton.png", 1, 1);
	m_bButton = MyLoadGraph("Data/UI/BButton.png", 1, 1);
}

/// <summary>
/// ゲーム内UIの描画処理
/// </summary>
/// <param name="player"></param>
/// <param name="enemy"></param>
/// <param name="eq"></param>
/// <param name="map"></param>
/// <param name="item"></param>
void UI::Draw(Player& player, EnemyManager& enemy, Setting& eq, MapManager& map, ItemManager& item, Weapon& weapon, Shield& shield, Armor& armor, Tool& tool, MessageManager& message)
{
	//パッド入力取得
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	//ステータスバーの描画
	StatusDraw(180, 30, player);

	//プレイヤーの状態アイコン描画
	DrawGraph(-50, 0, m_statusIcon, true);

	//装備の描画
	DrawGraph(150, 550, m_equipmentFrame, true);
	DrawGraph(0, 665, m_equipmentFrame, true);
	DrawGraph(300, 665, m_equipmentFrame, true);
	DrawGraph(150, 780, m_equipmentFrame, true);

	//装備している物描画
	EquipmentUIDraw(weapon, shield, armor, tool);

	//コアバーの描画
	DrawGraph(1050, 750, m_coreBackBar, true);

	SetFontSize(40);

	//休息するときは　休息する
	//アイテムの時は　アイテムを取る
	//ボス部屋に入るときは　白い光の中に入る
	//ワープするときは　転移する
	if (player.GetItemPick() || player.GetRestTouch() || player.GetBossEnter() || map.GetCore() || player.GetMessagePick())
	{
		DrawGraph(480, 800, m_actionUI, true);
		DrawGraph(500, 805, m_yButton, true);
		//休息
		if (player.GetRestTouch())
		{
			DrawFormatString(750, 820, 0xffffff, "休息する");
		}
		//アイテム
		else if (player.GetItemPick())
		{
			DrawFormatString(700, 820, 0xffffff, "アイテムを取る");
		}
		//ボス部屋入り口
		else if (player.GetBossEnter())
		{
			DrawFormatString(700, 820, 0xffffff, "白い光の中に入る");
		}
		//ワープ
		else if (map.GetCore())
		{
			DrawFormatString(750, 820, 0xffffff, "転移する");
		}
		//メッセージ
		else if (player.GetMessagePick())
		{
			DrawFormatString(700, 820, 0xffffff, "メッセージを読む");
		}
	}

	//アイテムを取った時
	if (player.GetTaking())
	{
		if (player.GetNowFrame() == 2.0f)
		{
			c_itemTakingUI = true;
		}
	}

	if (c_itemTakingUI)
	{
		DrawGraph(480, 600, m_itemTaking, true);
		DrawGraph(480, 800, m_actionUI, true);
		DrawGraph(500, 805, m_bButton, true);

		DrawFormatString(800, 820, 0xffffff, "O K");

		ItemTakingUI(item.m_uiItem.u_BlackSword, m_blackSword, 500, 625, 800, 675, "黒い剣");
		ItemTakingUI(item.m_uiItem.u_Distorted, m_uglyShield, 500, 645, 800, 675, "歪んだ盾");
		ItemTakingUI(item.m_uiItem.u_ArmorNormal, m_commonArmor, 500, 635, 800, 675, "普通の鎧");

		//Bbuttonを押すと閉じる
		if (m_xpad.Buttons[13] == 1)
		{
			item.m_uiItem.u_SmallCore = 0;
			item.m_uiItem.u_MediumCore = 0;
			item.m_uiItem.u_Rubbish = 0;
			item.m_uiItem.u_BlackSword = 0;
			item.m_uiItem.u_Distorted = 0;
			item.m_uiItem.u_ArmorNormal = 0;

			c_itemTakingUI = false;
		}
	}

	if (message.GetDraw())
	{
		DrawGraph(480, 800, m_actionUI, true);
		DrawGraph(500, 805, m_bButton, true);

		DrawFormatString(800, 820, 0xffffff, "O K");
	}

	//コア数描画
	DrawFormatString(1400, 905, 0xffffff, "%d", player.GetStatus().s_core);

	SetFontSize(40);
}

/// <summary>
/// 装備の描画処理
/// </summary>
/// <param name="player"></param>
void UI::EquipmentDraw(Weapon& weapon, Shield& shield, Armor& armor)
{
	//右武器
	if (weapon.GetFist())
	{
		//拳
		DrawGraph(110, 310, m_fist, true);
	}
	else if (weapon.GetBlack())
	{
		//黒い剣
		DrawGraph(140, 310, m_blackSword, true);
	}
	//左武器
	if (shield.GetFist())
	{
		//拳
		DrawGraph(310, 310, m_fist, true);
	}
	else if (shield.GetUgly())
	{
		//醜い盾
		DrawGraph(335, 330, m_uglyShield, true);
	}
	//防具
	if (armor.GetBody())
	{
		//裸体
		DrawGraph(565, 320, m_body, true);
	}
	else if (armor.GetCommon())
	{
		//平凡な防具
		DrawGraph(530, 320, m_commonArmor, true);
	}
	
}

/// <summary>
/// 右武器描画処理
/// </summary>
void UI::RightDraw(Weapon& weapon, ItemManager& item)
{
	//拳
	DrawGraph(70, 56, m_fist, true);

	if (item.GetItem().BlackSword >= 1)
	{
		//黒い剣
		DrawGraph(100, 210, m_blackSword, true);
	}
}

/// <summary>
/// 左武器描画処理
/// </summary>
void UI::LeftDraw(Shield& shield, ItemManager& item)
{
	//拳
	DrawGraph(70, 56, m_fist, true);

	if (item.GetItem().Distorted >= 1)
	{
		//醜い盾
		DrawGraph(85, 230, m_uglyShield, true);
	}
	
}

/// <summary>
/// 防具描画処理
/// </summary>
void UI::ArmorDraw(Armor& armor, ItemManager& item)
{
	//裸
	DrawGraph(120, 66, m_body, true);

	if (item.GetItem().ArmorNormal >= 1)
	{
		//平凡な鎧
		DrawGraph(85, 220, m_commonArmor, true);
	}
}

/// <summary>
/// ゲーム画面の装備UI描画処理
/// </summary>
/// <param name="weapon"></param>
/// <param name="shield"></param>
/// <param name="armor"></param>
void UI::EquipmentUIDraw(Weapon& weapon, Shield& shield, Armor& armor, Tool& tool)
{
	//右武器
	if (weapon.GetFist())
	{
		//拳
		DrawGraph(305, 700, m_fist, true);
	}
	else if (weapon.GetBlack())
	{
		//黒い剣
		DrawGraph(317, 700, m_blackSword, true);
	}
	//左武器
	if (shield.GetFist())
	{
		//拳
		DrawGraph(5, 700, m_fist, true);
	}
	else if (shield.GetUgly())
	{
		//醜い盾
		DrawGraph(20, 720, m_uglyShield, true);
	}
	//防具
	if (armor.GetBody())
	{
		//裸体
		DrawGraph(195, 600, m_body, true);
	}
	else if (armor.GetCommon())
	{
		//平凡な防具
		DrawGraph(160, 600, m_commonArmor, true);
	}

	if (tool.GetHeel().sa_number > 0)
	{
		//アイテム
		DrawGraph(115, 815, m_heelStone, true);

		DrawFormatString(260, 930, 0xffffff, "%d", tool.GetHeel().sa_number);
	}
}

/// <summary>
/// アイテムを取った時の描画
/// </summary>
/// <param name="item"></param>
/// <param name="handle"></param>
void UI::ItemTakingUI(int item, int handle, int x, int y, int charX, int charY, const char* letter)
{
	if (item > 0)
	{
		DrawGraph(x, y, handle, true);

		DrawFormatString(charX, charY, 0xffffff, letter);

		DrawFormatString(1100, 675, 0xffffff, "%d", item);
	}

}

/// <summary>
/// 死亡描画処理
/// </summary>
void UI::DiedDraw()
{
	//死亡時の背景の透過
	if (m_deadBack < 150)
	{
		m_deadBack++;
	}
	//死亡の文字の透過
	if (m_deadA < 255)
	{
		m_deadA++;

		m_waitResetTime = 0;
	}
	else if (m_deadA >= 255)
	{
		if (m_waitResetTime <= 30)
		{
			m_waitResetTime++;
		}
		else
		{
			m_deadReset = true;
		}
	}

	//背景の色を薄くする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_deadBack);
	DrawBox(0, 100, 2000, 400, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//死亡時の文字を出す
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_deadA);
	DrawGraph(-150, -100, m_dead, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

/// <summary>
/// ステータスバーの描画処理
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="player"></param>
void UI::StatusDraw(int x, int y, Player& player)
{
	//HP最大幅を計算
	int HPbar = player.GetMaxStatus().sm_hp / 30;
	//HP最大幅を計算
	int HPBarWidth = (int)((float)player.GetStatus().s_hp / player.GetMaxStatus().sm_hp * (185 + (50 * player.GetLevelStatus().sl_hp)));

	//スタミナ最大幅を計算
	int StaminaBar = player.GetMaxStatus().sm_stamina / 10;
	//スタミナ最大幅を計算
	int StaminaBarWidth = (int)((float)player.GetStatus().s_stamina / player.GetMaxStatus().sm_stamina * (150 + (10 * player.GetLevelStatus().sl_stamina)));

	if (HPBarWidth > 0)
	{
		DrawRectGraph(x + 5, y + 8, 0, 0, HPBarWidth, 45, m_hpBar, true);
	}


	for (int i = 0; i < HPbar; i++)
	{
		//中央のバーを描画
		if (i > 0 && i != HPbar - 1)
		{
			if (i == 1)
			{
				DrawGraph(x + 23, y, m_backCenterBar, true);
			}
			else
			{
				DrawGraph(x + 23 + (50 * (i - 1)), y, m_backCenterBar, true);
			}
		}
		//左端のバーを描画
		else if (i == 0)
		{
			DrawGraph(x, y, m_backLeftBar, true);
		}
		//右端のバーを描画
		else if (i == HPbar - 1)
		{
			DrawGraph(x + 23 + (50 * (i - 1)), y, m_backRightBar, true);
		}

		
	}

	if (StaminaBarWidth > 0)
	{
		DrawRectGraph(x - 8, y + 50, 0, 0, StaminaBarWidth, 50, m_staminaBar, true);
	}

	//左端のバーを描画
	DrawGraph(x, y + 50, m_backLeftBar, true);

	for (int i = 0; i < StaminaBar; i++)
	{
		//中央のバーを描画
		if (i > 0 && i != StaminaBar - 1)
		{
			if (i == 1)
			{
				DrawGraph(x + 23, y +50, m_backCenterBar, true);
			}
			else
			{
				DrawGraph(x + 23 + (10 * (i - 1)), y + 50, m_backCenterBar, true);
			}
		}
		//右端のバーを描画
		else if (i == StaminaBar - 1)
		{
			DrawGraph(x + 23 + (10 * (i - 1)) + (2 * i), y + 50, m_backRightBar, true);
		}
	}
	
}

/// <summary>
/// 敵のHP描画処理
/// </summary>
/// <param name="pos"></param>
/// <param name="hp"></param>
/// <param name="maxHP"></param>
void UI::EnemyHPDraw(VECTOR pos, int hp, int maxHP)
{
	//3D座標から2D座標に変換
	VECTOR screenPos;
	
	screenPos = ConvWorldPosToScreenPos(pos);

	int screenX = (int)screenPos.x - 100;   //HPバーの場所を調整
	int screenY = (int)screenPos.y - 300;   //HPバーの高さを調整

	//HPバーの最大幅を計算
	int HPBarWidth = (int)((float)hp / maxHP * 200);

	if (HPBarWidth > 0)
	{
		DrawRectGraph(screenX, screenY, 0, 0, HPBarWidth, 30, m_hpBar, true);
	}
}

/// <summary>
/// ボスのHP描画処理
/// </summary>
/// <param name="hp"></param>
/// <param name="maxHP"></param>
/// <param name="name"></param>
void UI::BossHPDraw(int hp, int maxHP, const char* name, const char* subName)
{
	//HPバーの最大幅を計算
	int HPBarWidth = (int)((float)hp / maxHP * 800);

	SetFontSize(30);
	//ボスの当て字
	DrawString(450, 670, subName, 0xffffff);

	SetFontSize(40);

	//ボスの名前
	DrawString(450, 700, name, 0xffffff);

	if (HPBarWidth > 0)
	{
		DrawRectGraph(450, 750, 0, 0, HPBarWidth, 50, m_hpBar, true);
	}

}

/// <summary>
/// 終了処理
/// </summary>
void UI::End()
{
	DeleteGraph(m_blackSword);
	DeleteGraph(m_fist);
	DeleteGraph(m_body);
	DeleteGraph(m_uglyShield);
	DeleteGraph(m_commonArmor);
	DeleteGraph(m_dead);
	DeleteGraph(m_backRightBar);
	DeleteGraph(m_backLeftBar);
	DeleteGraph(m_backCenterBar);
	DeleteGraph(m_backOutBar);
	DeleteGraph(m_hpBar);
	DeleteGraph(m_staminaBar);
	DeleteGraph(m_coreBackBar);
	DeleteGraph(m_statusIcon);
	DeleteGraph(m_equipmentFrame);
	DeleteGraph(m_actionUI);
	DeleteGraph(m_itemTaking);
	DeleteGraph(m_yButton);
	DeleteGraph(m_bButton);
}
