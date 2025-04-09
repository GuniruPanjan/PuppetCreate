#include "UI.h"
#include "Item/Weapon.h"
#include "Item/Shield.h"
#include "Item/Armor.h"
#include "Character/Player.h"
#include "Manager/EnemyManager.h"
#include "Manager/ItemManager.h"
#include "Manager/MapManager.h"
#include "Item/Tool.h"
#include "External/Font.h"
#include "Manager/SEManager.h"
#include "Manager/MessageManager.h"

namespace
{
	const int cHpBarWidth = 200;
	const int cBossHpBarWidth = 800;
	const int cStatusDrawX = 180;
	const int cStatusDrawY = 30;
	const int cEquipmentFrameX1 = 150;
	const int cEquipmentFrameY1 = 550;
	const int cEquipmentFrameX2 = 0;
	const int cEquipmentFrameY2 = 665;
	const int cEquipmentFrameX3 = 300;
	const int cEquipmentFrameY3 = 665;
	const int cEquipmentFrameX4 = 150;
	const int cEquipmentFrameY4 = 780;
	const int cCoreBarX = 1050;
	const int cCoreBarY = 750;
	const int cActionUiX = 480;
	const int cActionUiY = 800;
	const int cButtonY = 805;
	const int cItemTakingX = 480;
	const int cItemTakingY = 600;
	const int cOkTextX = 800;
	const int cOkTextY = 815;
	const int cPlayerCoreX = 1400;
	const int cPlayerCoreY = 905;
	const int cDirectionTextX = -150;
	const int cDirectionTextY = -200;
	const int cDirectionBackX1 = 0;
	const int cDirectionBackX2 = 1920;
	const int cDirectionWinBackY1 = 230;
	const int cDirectionWinBackY2 = 450;
	const int cDirectionDeadBackY1 = 250;
	const int cDirectionDeadBackY2 = 450;
	const int cDirectionDivide = 2;
	const int cEnemyHpBarXOffset = -100;
	const int cEnemyHpBarYOffset = -300;
	const int cBossNameX = 450;
	const int cBossNameY = 700;
	const int cBossSubnameY = 670;
	const int cBossHpBarX = 450;
	const int cBossHpBarY = 750;

	const int cHpBarHeight = 45;
	const int cStaminaBarHeight = 50;
	const int cHpBarOffsetX = 5;
	const int cHpBarOffsetY = 8;
	const int cStaminaBarOffsetX = -8;
	const int cStaminaBarOffsetY = 50;
	const int cHpBarSegmentWidth = 50;
	const int cStaminaBarSegmentWidth = 10;
	const int cHpBarInitialWidth = 185;
	const int cStaminaBarInitialWidth = 150;
	const int cHpBarSegmentXOffset = 23;
	const int cStaminaBarSegmentXOffset = 23;
	const int cStaminaBarSegmentXAdjustment = 2;

	const int cFistX = 70;
	const int cFistY = 56;
	const int cBodyX = 120;
	const int cBodyY = 66;

	const int cRightWeaponX = 305;
	const int cRightWeaponY = 700;
	const int cLeftWeaponX = 5;
	const int cLeftWeaponY = 700;
	const int cLeftWeaponOffsetX = 15;
	const int cLeftWeaponOffsetY = 20;
	const int cArmorX = 195;
	const int cArmorY = 600;
	const int cCommonArmorX = 160;
	const int cCommonArmorY = 600;
	const int cHeelStoneX = 115;
	const int cHeelStoneY = 815;
	const int cHeelStoneTextX = 260;
	const int cHeelStoneTextY = 930;

	const int cWeaponX = 110;
	const int cWeaponY = 310;
	const int cShieldX = 310;
	const int cShieldY = 310;
	const int cShieldOffsetX = 25;
	const int cShieldOffsetY = 20;
	const int cArmorBodyX = 565;
	const int cArmorBodyY = 320;
	const int cArmorCommonX = 530;
	const int cArmorCommonY = 320;

	const int cStatusIconX = -50;
	const int cStatusIconY = 0;
	const int cActionUiXOffset = 20;
	const int cActionTextX = 750;
	const int cActionTextY = 815;
	const int cItemTextX = 700;
	const int cBossTextX = 690;
	const int cWarpTextX = 750;
	const int cMessageTextX = 680;
	const int cTextColor = 0xffffff;
	const int cDirectionColor = 0x000000;
	const int cItemTakingUiX = 500;
	const int cItemTakingUiYBlackSword = 625;
	const int cItemTakingUiYDistorted = 645;
	const int cItemTakingUiYArmorNormal = 635;
	const int cItemTakingUiYBat = 635;
	const int cItemTakingUiYWoodShield = 635;
	const int cItemTakingUiCharX = 800;
	const int cItemTakingUiCharX1 = 650;
	const int cItemTakingUiCharY = 675;

	bool cItemTakingUi = false;
}

/// <summary>
/// コンストラクタ
/// </summary>
UI::UI() :
	m_deadReset(false),
	m_winReset(false),
	m_waitResetTime(0),
	m_youDead(0),
	m_youWin(0),
	m_alphaValue(false),
	m_equipmentReturn(false),
	m_xpad()
{
	m_pFont = std::make_shared<Font>();
	m_pSmallFont = std::make_shared<Font>();
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
	m_youDead = 0;
	m_youWin = 0;
	m_waitResetTime = 0;
	m_deadReset = false;
	m_winReset = true;

	m_heelStone = MyLoadGraph("Data/UI/HeelStoneMini.png", 4, 4);
	m_blackSword = MyLoadGraph("Data/UI/黒い剣UI.png", 3, 3);
	m_fist = MyLoadGraph("Data/UI/拳UI.png", 3, 3);
	m_body = MyLoadGraph("Data/UI/裸体UI.png", 3, 3);
	m_uglyShield = MyLoadGraph("Data/UI/醜い盾UI.png", 4, 4);
	m_commonArmor = MyLoadGraph("Data/UI/平凡な鎧UI.png", 3, 3);
	m_bat = MyLoadGraph("Data/UI/But.png", 3, 3);
	m_woodShield = MyLoadGraph("Data/UI/WoodShield.png", 4, 4);

	m_dead = MyLoadGraph("Data/UI/YOUDIEDGraph.png", 1, 1);
	m_victory = MyLoadGraph("Data/UI/GottheCoreback.png", 1, 1);
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
	m_yButton = MyLoadGraph("Data/UI/YButton.png", 3, 3);
	m_bButton = MyLoadGraph("Data/UI/BButton.png", 3, 3);
	m_tagetLock = MyLoadGraph("Data/UI/TagetLock.png", 1, 1);

	m_pFont->FontInit(40);
	m_pSmallFont->FontInit(30);
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
	StatusDraw(cStatusDrawX, cStatusDrawY, player);

	//プレイヤーの状態アイコン描画
	DrawGraph(cStatusIconX, cStatusIconY, m_statusIcon, true);

	//装備の描画
	DrawGraph(cEquipmentFrameX1, cEquipmentFrameY1, m_equipmentFrame, true);
	DrawGraph(cEquipmentFrameX2, cEquipmentFrameY2, m_equipmentFrame, true);
	DrawGraph(cEquipmentFrameX3, cEquipmentFrameY3, m_equipmentFrame, true);
	DrawGraph(cEquipmentFrameX4, cEquipmentFrameY4, m_equipmentFrame, true);

	//装備している物描画
	EquipmentUIDraw(weapon, shield, armor, tool);

	//コアバーの描画
	DrawGraph(cCoreBarX, cCoreBarY, m_coreBackBar, true);

	//休息するときは　休息する
	//アイテムの時は　アイテムを取る
	//ボス部屋に入るときは　白い光の中に入る
	//ワープするときは　転移する
	if (player.GetItemPick() || player.GetRestTouch() || player.GetBossEnter() || map.GetCore() || player.GetMessagePick())
	{
		DrawGraph(cActionUiX, cActionUiY, m_actionUI, true);
		DrawGraph(cActionUiX + cActionUiXOffset, cButtonY, m_yButton, true);
		//休息
		if (player.GetRestTouch())
		{
			DrawStringToHandle(cActionTextX, cActionTextY, "休息する", cTextColor, m_pFont->GetHandle());
		}
		//アイテム
		else if (player.GetItemPick())
		{
			DrawStringToHandle(cItemTextX, cActionTextY, "アイテムを取る", cTextColor, m_pFont->GetHandle());
		}
		//ボス部屋入り口
		else if (player.GetBossEnter())
		{
			DrawStringToHandle(cBossTextX, cActionTextY, "白い光の中に入る", cTextColor, m_pFont->GetHandle());
		}
		//ワープ
		else if (map.GetCore())
		{
			DrawStringToHandle(cWarpTextX, cActionTextY, "転移する", cTextColor, m_pFont->GetHandle());
		}
		//メッセージ
		else if (player.GetMessagePick())
		{
			DrawStringToHandle(cMessageTextX, cActionTextY, "メッセージを読む", cTextColor, m_pFont->GetHandle());
		}
	}

	//アイテムを取った時
	if (player.GetTaking())
	{
		if (player.GetNowFrame() == 2.0f)
		{
			cItemTakingUi = true;
		}
	}

	if (cItemTakingUi)
	{
		DrawGraph(cItemTakingX, cItemTakingY, m_itemTaking, true);
		DrawGraph(cActionUiX, cActionUiY, m_actionUI, true);
		DrawGraph(cActionUiX + cActionUiXOffset, cButtonY, m_bButton, true);

		DrawStringToHandle(cOkTextX, cOkTextY, "O K", cTextColor, m_pFont->GetHandle());

		ItemTakingUI(item.m_uiItem.u_BlackSword, m_blackSword, cItemTakingUiX, cItemTakingUiYBlackSword, cItemTakingUiCharX, cItemTakingUiCharY, "黒剣");
		ItemTakingUI(item.m_uiItem.u_Distorted, m_uglyShield, cItemTakingUiX, cItemTakingUiYDistorted, cItemTakingUiCharX, cItemTakingUiCharY, "忌盾");
		ItemTakingUI(item.m_uiItem.u_ArmorNormal, m_commonArmor, cItemTakingUiX, cItemTakingUiYArmorNormal, cItemTakingUiCharX1, cItemTakingUiCharY, "ノクターニス兵の鎧");
		ItemTakingUI(item.m_uiItem.u_Bat, m_bat, cItemTakingUiX, cItemTakingUiYBat, cItemTakingUiCharX, cItemTakingUiCharY, "木の棍棒");
		ItemTakingUI(item.m_uiItem.u_WoodShield, m_woodShield, cItemTakingUiX, cItemTakingUiYWoodShield, cItemTakingUiCharX, cItemTakingUiCharY, "木の盾");

		//Bbuttonを押すと閉じる
		if (m_xpad.Buttons[13] == 1)
		{
			item.m_uiItem.u_SmallCore = 0;
			item.m_uiItem.u_MediumCore = 0;
			item.m_uiItem.u_Rubbish = 0;
			item.m_uiItem.u_BlackSword = 0;
			item.m_uiItem.u_Distorted = 0;
			item.m_uiItem.u_ArmorNormal = 0;
			item.m_uiItem.u_Bat = 0;
			item.m_uiItem.u_WoodShield = 0;

			cItemTakingUi = false;
		}
	}

	if (message.GetDraw())
	{
		DrawGraph(cActionUiX, cActionUiY, m_actionUI, true);
		DrawGraph(cActionUiX + cActionUiXOffset, cButtonY, m_bButton, true);

		DrawStringToHandle(cOkTextX, cOkTextY, "O K", cTextColor, m_pFont->GetHandle());
	}

	//コア数描画
	DrawFormatStringToHandle(cPlayerCoreX, cPlayerCoreY, cTextColor, m_pFont->GetHandle(), "%d", player.GetStatus().s_core);
}

/// <summary>
/// 装備の描画処理
/// </summary>
/// <param name="player"></param>
void UI::EquipmentDraw(Weapon& weapon, Shield& shield, Armor& armor)
{
	const int OffsetX = 30;
	const int ShieldOffsetX = 25;
	const int OffsetY = 20;

	//右武器
	if (weapon.GetFist())
	{
		//拳
		DrawGraph(cWeaponX, cWeaponY, m_fist, true);
	}
	else if (weapon.GetBlack())
	{
		//黒い剣
		DrawGraph(cWeaponX + OffsetX, cWeaponY, m_blackSword, true);
	}
	else if (weapon.GetBat())
	{
		//木の棒
		DrawGraph(cWeaponX + OffsetX, cWeaponY, m_bat, true);
	}

	//左武器
	if (shield.GetFist())
	{
		//拳
		DrawGraph(cShieldX, cShieldY, m_fist, true);
	}
	else if (shield.GetUgly())
	{
		//醜い盾
		DrawGraph(cShieldX + ShieldOffsetX, cShieldY + OffsetY, m_uglyShield, true);
	}
	else if (shield.GetWood())
	{
		//木の盾
		DrawGraph(cShieldX + ShieldOffsetX, cShieldY + OffsetY, m_woodShield, true);
	}

	//防具
	if (armor.GetBody())
	{
		//裸体
		DrawGraph(cArmorBodyX, cArmorBodyY, m_body, true);
	}
	else if (armor.GetCommon())
	{
		//平凡な防具
		DrawGraph(cArmorCommonX, cArmorCommonY, m_commonArmor, true);
	}
}

/// <summary>
/// 右武器描画処理
/// </summary>
void UI::RightDraw(Weapon& weapon, ItemManager& item)
{
	int x = 100;
	int y = 210;
	int offset = 150;
	//拳
	DrawGraph(cFistX, cFistY, m_fist, true);

	//アイテムを取得した順に描画する
	for (const auto& itemName : item.GetItemOrder())
	{
		//黒い剣描画
		if (itemName == "BlackSword")
		{
			DrawGraph(x, y, m_blackSword, true);

			y += offset;

		}
		//木のバット描画
		else if (itemName == "Bat")
		{
			DrawGraph(x, y, m_bat, true);

			y += offset;

		}

	}
}

/// <summary>
/// 左武器描画処理
/// </summary>
void UI::LeftDraw(Shield& shield, ItemManager& item)
{
	int x = 85;
	int y = 230;
	int offset = 150;


	//拳
	DrawGraph(cFistX, cFistY, m_fist, true);


	//アイテムを取得した順に描画
	for (const auto& itemName : item.GetItemOrder())
	{
		//醜い盾描画
		if (itemName == "Distorted")
		{	
			DrawGraph(x, y, m_uglyShield, true);

			y += offset;

		}
		//木の盾描画
		else if (itemName == "WoodShield")
		{
			DrawGraph(x, y, m_woodShield, true);

			y += offset;

		}
		
	}
}

/// <summary>
/// 防具描画処理
/// </summary>
void UI::ArmorDraw(Armor& armor, ItemManager& item)
{
	int x = 85;
	int y = 220;
	int offset = 180;

	//裸
	DrawGraph(cBodyX, cBodyY, m_body, true);

	//アイテムを取得した順に描画
	for (const auto& itemName : item.GetItemOrder())
	{
		if (itemName == "ArmorNormal")
		{
			//平凡な鎧
			DrawGraph(x, y, m_commonArmor, true);

			y += offset;

		}
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
	int rightX = 12;

	

	//右武器
	if (weapon.GetFist())
	{
		//拳
		DrawGraph(cRightWeaponX, cRightWeaponY, m_fist, true);
	}
	else if (weapon.GetBlack())
	{
		//黒い剣
		DrawGraph(cRightWeaponX + rightX, cRightWeaponY, m_blackSword, true);
	}
	else if (weapon.GetBat())
	{
		//木の棒
		DrawGraph(cRightWeaponX + rightX, cRightWeaponY, m_bat, true);
	}

	//左武器
	if (shield.GetFist())
	{
		//拳
		DrawGraph(cLeftWeaponX, cLeftWeaponY, m_fist, true);
	}
	else if (shield.GetUgly())
	{
		//醜い盾
		DrawGraph(cLeftWeaponX + cLeftWeaponOffsetX, cLeftWeaponY + cLeftWeaponOffsetY, m_uglyShield, true);
	}
	else if (shield.GetWood())
	{
		//木の盾
		DrawGraph(cLeftWeaponX + cLeftWeaponOffsetX, cLeftWeaponY + cLeftWeaponOffsetY, m_woodShield, true);
	}

	//防具
	if (armor.GetBody())
	{
		//裸体
		DrawGraph(cArmorX, cArmorY, m_body, true);
	}
	else if (armor.GetCommon())
	{
		//平凡な防具
		DrawGraph(cCommonArmorX, cCommonArmorY, m_commonArmor, true);
	}

	if (tool.GetHeel().sa_number > 0)
	{
		//アイテム
		DrawGraph(cHeelStoneX, cHeelStoneY, m_heelStone, true);

		DrawFormatStringToHandle(cHeelStoneTextX, cHeelStoneTextY, cTextColor, m_pFont->GetHandle(), "%d", tool.GetHeel().sa_number);
	}
}

/// <summary>
/// アイテムを取った時の描画
/// </summary>
/// <param name="item"></param>
/// <param name="handle"></param>
void UI::ItemTakingUI(int item, int handle, int x, int y, int charX, int charY, const char* letter)
{
	int X = 1100;
	int Y = 675;

	if (item > 0)
	{
		DrawGraph(x, y, handle, true);

		DrawStringToHandle(charX, charY, letter, cTextColor, m_pFont->GetHandle());

		DrawFormatStringToHandle(X, Y, cTextColor, m_pFont->GetHandle(), "%d", item);
	}

}

/// <summary>
/// 死亡描画処理
/// </summary>
void UI::DiedDraw(SEManager& se)
{
	int Max = 255;
	int WaitTime = 30;
	int Value = 3;
	int End = 10;

	//死亡の文字の透過
	if (m_youDead == 0)
	{
		//敗北SE再生
		PlaySoundMem(se.GetLoserSE(), DX_PLAYTYPE_BACK, true);

		m_alphaValue = true;

		m_waitResetTime = 0;
	}
	else if (m_youDead >= Max)
	{
		if (m_waitResetTime <= WaitTime)
		{
			m_waitResetTime++;
		}
		else
		{
			m_alphaValue = false;
			//初期化
			m_waitResetTime = 0;
		}
	}
	//アルファ値を変化させる
	if (m_alphaValue)
	{
		if (m_youDead < Max)
		{
			m_youDead++;
		}
	}
	else
	{
		if (m_youDead > End)
		{
			m_youDead -= Value;
		}
		else if (m_youDead <= End)
		{
			m_deadReset = true;

			m_youDead = 0;
		}
	}

	//背景の黒い画像を透過する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_youDead / cDirectionDivide);
	DrawBox(cDirectionBackX1, cDirectionDeadBackY1, cDirectionBackX2, cDirectionDeadBackY2, cDirectionColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//死亡時の文字を出す
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_youDead);
	DrawGraph(cDirectionTextX, 0, m_dead, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
/// <summary>
/// ボスを倒したときの勝利演出描画
/// </summary>
void UI::GetCoreDraw(SEManager& se)
{
	int Max = 255;
	int WaitTime = 30;
	int Value = 3;
	int End = 10;

	//勝利の文字の透過
	if (m_youWin == 0)
	{
		//勝利SE再生
		PlaySoundMem(se.GetVictorySE(), DX_PLAYTYPE_BACK, true);

		m_alphaValue = true;

		m_waitResetTime = 0;
	}
	else if(m_youWin >= Max)
	{
		if (m_waitResetTime <= WaitTime)
		{
			m_waitResetTime++;
		}
		else
		{
			m_alphaValue = false;
			//初期化
			m_waitResetTime = 0;
		}
	}
	//アルファ値を変化させる
	if (m_alphaValue)
	{
		if (m_youWin < Max)
		{
			m_youWin += Value;
		}
	}
	else
	{
		if (m_youWin > End)
		{
			m_youWin -= Value;
		}
		else if(m_youWin <= End)
		{
			m_winReset = false;

			m_youWin = 0;
		}
	}

	//背景の黒い画像を透過する
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_youWin / cDirectionDivide);
	DrawBox(cDirectionBackX1, cDirectionWinBackY1, cDirectionBackX2, cDirectionWinBackY2, cDirectionColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//勝利時の文字を出す
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_youWin);
	DrawGraph(cDirectionTextX, cDirectionTextY, m_victory, true);
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
	int HPBarWidth = (int)((float)player.GetStatus().s_hp / player.GetMaxStatus().sm_hp * (cHpBarInitialWidth + (cHpBarSegmentWidth * player.GetLevelStatus().sl_hp)));

	//スタミナ最大幅を計算
	int StaminaBar = player.GetMaxStatus().sm_stamina / 10;
	//スタミナ最大幅を計算
	int StaminaBarWidth = (int)((float)player.GetStatus().s_stamina / player.GetMaxStatus().sm_stamina * (cStaminaBarInitialWidth + (cStaminaBarSegmentWidth * player.GetLevelStatus().sl_stamina)));

	if (HPBarWidth > 0)
	{
		DrawRectGraph(x + cHpBarOffsetX, y + cHpBarOffsetY, 0, 0, HPBarWidth, cHpBarHeight, m_hpBar, true);
	}

	for (int i = 0; i < HPbar; i++)
	{
		//中央のバーを描画
		if (i > 0 && i != HPbar - 1)
		{
			if (i == 1)
			{
				DrawGraph(x + cHpBarSegmentXOffset, y, m_backCenterBar, true);
			}
			else
			{
				DrawGraph(x + cHpBarSegmentXOffset + (cHpBarSegmentWidth * (i - 1)), y, m_backCenterBar, true);
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
			DrawGraph(x + cHpBarSegmentXOffset + (cHpBarSegmentWidth * (i - 1)), y, m_backRightBar, true);
		}
	}

	if (StaminaBarWidth > 0)
	{
		DrawRectGraph(x + cStaminaBarOffsetX, y + cStaminaBarOffsetY, 0, 0, StaminaBarWidth, cStaminaBarHeight, m_staminaBar, true);
	}

	//左端のバーを描画
	DrawGraph(x, y + cStaminaBarOffsetY, m_backLeftBar, true);

	for (int i = 0; i < StaminaBar; i++)
	{
		//中央のバーを描画
		if (i > 0 && i != StaminaBar - 1)
		{
			if (i == 1)
			{
				DrawGraph(x + cStaminaBarSegmentXOffset, y + cStaminaBarOffsetY, m_backCenterBar, true);
			}
			else
			{
				DrawGraph(x + cStaminaBarSegmentXOffset + (cStaminaBarSegmentWidth * (i - 1)), y + cStaminaBarOffsetY, m_backCenterBar, true);
			}
		}
		//右端のバーを描画
		else if (i == StaminaBar - 1)
		{
			DrawGraph(x + cStaminaBarSegmentXOffset + (cStaminaBarSegmentWidth * (i - 1)) + (cStaminaBarSegmentXAdjustment * i), y + cStaminaBarOffsetY, m_backRightBar, true);
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
	int Height = 30;

	//3D座標から2D座標に変換
	VECTOR screenPos;

	screenPos = ConvWorldPosToScreenPos(pos);

	int screenX = (int)screenPos.x + cEnemyHpBarXOffset;   //HPバーの場所を調整
	int screenY = (int)screenPos.y + cEnemyHpBarYOffset;   //HPバーの高さを調整

	//HPバーの最大幅を計算
	int HPBarWidth = (int)((float)hp / maxHP * cHpBarWidth);

	if (HPBarWidth > 0)
	{
		DrawRectGraph(screenX, screenY, 0, 0, HPBarWidth, Height, m_hpBar, true);
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
	const int Height = 50;

	//HPバーの最大幅を計算
	int HPBarWidth = (int)((float)hp / maxHP * cBossHpBarWidth);

	//ボスの当て字
	DrawStringToHandle(cBossNameX, cBossSubnameY, subName, cTextColor, m_pSmallFont->GetHandle());

	//ボスの名前
	DrawStringToHandle(cBossNameX, cBossNameY, name, cTextColor, m_pFont->GetHandle());

	if (HPBarWidth > 0)
	{
		DrawRectGraph(cBossHpBarX, cBossHpBarY, 0, 0, HPBarWidth, Height, m_hpBar, true);
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
	DeleteGraph(m_bat);
	DeleteGraph(m_woodShield);
	DeleteGraph(m_dead);
	DeleteGraph(m_victory);
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
