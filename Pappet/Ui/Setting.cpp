#include "Setting.h"
#include "Manager/SelectManager.h"
#include "UI/UIBase.h"
#include "Item/Shield.h"
#include "Item/Weapon.h"
#include "Item/Armor.h"
#include "Character/Player.h"
#include "Manager/CoreManager.h"
#include "Manager/ItemManager.h"

namespace
{
	int selectDecision = 0;     //選択したものを決定する変数
	int brightDecision = 0;     //明るさを決定する変数
	int volumeDecision = 0;     //音量を決定する変数

	//a値をいじる変数
	int cBlenda = 10;  

	//画像の倍率
	float cGraphSize = 1.0f;

	//間違って押さないようにする
	int cWaitTime = 0;
	//連続で押さないようにする
	int cPush = 0;

	//変更する変数
	constexpr int cEquipmentOneX = 630;
	constexpr int cEquipmentOneY = 0;
	constexpr int cEquipmentSecondX = 800;
	constexpr int cEquipmentSecondY = 170;

	constexpr int cItemOneX = 800;
	constexpr int cItemOneY = 0;
	constexpr int cItemSecondX = 970;
	constexpr int cItemSecondY = 170;

	constexpr int cRightEquipmentOneX = 107;
	constexpr int cRightEquipmentOneY = 298;
	constexpr int cRightEquipmentSecondX = 276;
	constexpr int cRightEquipmentSecondY = 467;

	constexpr int cLeftEquipmentOneX = 308;
	constexpr int cLeftEquipmentOneY = 298;
	constexpr int cLeftEquipmentSecondX = 477;
	constexpr int cLeftEquipmentSecondY = 467;

	constexpr int cArmorEquipmentOneX = 509;
	constexpr int cArmorEquipmentOneY = 298;
	constexpr int cArmorEquipmentSecondX = 678;
	constexpr int cArmorEquipmentSecondY = 467;

	constexpr int cOneX = 75;
	constexpr int cOneY = 60;
	constexpr int cSecondX = 218;
	constexpr int cSecondY = 200;
	constexpr int cDifferenceY = 155;

	bool cNo = false;    //力技用
}

/// <summary>
/// コンストラクタ
/// </summary>
Setting::Setting() :
	m_black(0),
	m_white(0),
	m_back(0),
	m_selectX(0),
	m_selectY(0),
	m_blackPal(0),
	m_whitePal(0),
	m_button(0),
	m_thumb(0),
	m_waitTime(0),
	m_brightnessColor(0),
	m_bgmColor(0),
	m_returnColor(0),
	m_one(false),
	m_settingScene(false),
	m_brightness(false),
	m_volume(false),
	m_volumeSize(0),
	m_equipmentMenu(false),
	m_itemMenu(false),
	m_decisionEquipment(false),
	m_blend(false),
	m_returnMenu(true),
	m_titleMenu(false),
	m_statusLevel(false),
	m_up(false),
	m_xpad()
{

	for (int i = 0; i < 5; i++)
	{
		m_brightColor[i] = 0;
		m_volumeColor[i] = 0;
		m_menuSelect[i] = 0;
		m_menuColor[i] = 0;
	}
}

/// <summary>
/// デストラクタ
/// </summary>
Setting::~Setting()
{
	//メモリ解放
	DeleteGraph(m_black);
	DeleteGraph(m_back);
	DeleteGraph(m_white);
	pse->End();
}

/// <summary>
/// 初期化処理
/// </summary>
void Setting::Init()
{
	//画像読み込み
	m_black = LoadGraph("Data/SceneBack/BlackMini.png");         //14.1 KB (14,532 バイト)
	m_back = MyLoadGraph("Data/SceneBack/BlackMini.png", 1, 1);
	m_white = LoadGraph("Data/SceneBack/WhiteMini.png");         //14.1 KB (14,529 バイト)

	selectDecision = 0;

	//明るさ色初期化
	m_brightColor[0] = 0xffffff;
	m_brightColor[1] = 0xffffff;
	m_brightColor[2] = 0xffff00;
	m_brightColor[3] = 0xffffff;
	m_brightColor[4] = 0xffffff;

	//音量色初期化
	m_volumeColor[0] = 0xffffff;
	m_volumeColor[1] = 0xffffff;
	m_volumeColor[2] = 0xffff00;
	m_volumeColor[3] = 0xffffff;
	m_volumeColor[4] = 0xffffff;

	m_menuSelect[0] = 1;
	m_menuSelect[1] = 0;
	m_menuSelect[2] = 0;

	//音量サイズ
	m_volumeSize = 130;

	m_button = 0;
	m_thumb = 0;
	m_one = false;
	m_settingScene = false;
	m_brightness = false;
	m_volume = false;
	m_equipmentMenu = false;
	m_returnMenu = true;
	m_titleMenu = false;

	m_brightnessColor = 0xffffff;
	m_bgmColor = 0xffffff;
	m_returnColor = 0xffffff;

	m_blackPal = 0;
	m_whitePal = 0;

	pse->SceneInit();

	m_equipment = MyLoadGraph("Data/UI/装備画面UI.png", 1, 1);
	m_itemBox = MyLoadGraph("Data/UI/アイテム画面UI.png", 1, 1);
	m_selectEquipment = MyLoadGraph("Data/UI/装備選択画面UI.png", 1, 1);
	m_rest = MyLoadGraph("Data/UI/四角の白い長方形の囲い.png", 1, 1);
	m_selectUi = MyLoadGraph("Data/UI/選択UI.png", 2, 2);
	m_levelUp = MyLoadGraph("Data/UI/レベルステータスUI.png", 1, 1);
}

/// <summary>
/// 更新処理
/// </summary>
void Setting::Update()
{
	//パッド入力所得
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	if (m_waitTime > 10)
	{
		//上
		if (m_xpad.ThumbLY > 2000)
		{
			m_button++;
		}
		//下
		else if (m_xpad.ThumbLY < 0)
		{
			m_button--;
		}
		//右
		else if (m_xpad.ThumbLX > 2000)
		{
			m_thumb--;
		}
		//左
		else if (m_xpad.ThumbLX < 0)
		{
			m_thumb++;
		}
		else
		{
			//初期化
			m_button = 0;
			m_thumb = 0;

			m_one = false;
		}

		//選択中
		if (m_brightness == false && m_volume == false)
		{
			pselect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, pselect->Eight);
		}
		//明るさの選択中
		if (m_brightness == true && m_volume == false)
		{
			pselect->Menu_Update(m_thumb, m_one, m_xpad.Buttons[13], brightDecision, pselect->Six);
		}
		//音量の選択中
		if (m_brightness == false && m_volume == true)
		{
			pselect->Menu_Update(m_thumb, m_one, m_xpad.Buttons[13], volumeDecision, pselect->Six);
		}

		//Aボタンを押したら
		if (m_xpad.Buttons[12] == 1)
		{
			PlaySoundMem(pse->GetButtonSE(), DX_PLAYTYPE_BACK, true);

			//明るさ設定
			if (selectDecision == 8)
			{
				m_brightness = true;
			}
			//音量設定
			if (selectDecision == 9)
			{
				m_volume = true;
			}
			//元の画面に戻る
			if (selectDecision == 10)
			{
				m_settingScene = false;
			}
		}

		//明るさ設定を押したら
		if (m_brightness == true)
		{
			//Bボタンを押したら
			if (m_xpad.Buttons[13] == 1)
			{
				PlaySoundMem(pse->GetButtonSE(), DX_PLAYTYPE_BACK, true);

				m_brightness = false;
			}
		}
		//音量設定を押したら
		if (m_volume == true)
		{
			//Bボタンを押したら
			if (m_xpad.Buttons[13] == 1)
			{
				PlaySoundMem(pse->GetButtonSE(), DX_PLAYTYPE_BACK, true);

				m_volume = false;
			}
		}
	}
	else
	{
		m_waitTime++;
	}

	pse->Update(m_volumeSize);

}

/// <summary>
/// メニュー更新処理
/// </summary>
void Setting::MenuUpdate(Player& player)
{
	//パッド入力所得
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	//上
	if (m_xpad.Buttons[0] == 1)
	{
		m_button++;
	}
	//下
	else if (m_xpad.Buttons[1] == 1)
	{
		m_button--;
	}
	else
	{
		//初期化
		m_button = 0;

		m_one = false;
	}

	

	pselect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, pselect->Eight);

	if (cWaitTime >= 10)
	{
		//Aボタンが押されたら
		if (m_xpad.Buttons[12] == 1)
		{
			PlaySoundMem(pse->GetButtonSE(), DX_PLAYTYPE_BACK, true);

			//装備選択
			if (selectDecision == 8)
			{
				m_equipmentMenu = true;

				//リセット
				cWaitTime = 0;
			}
			//元の画面に戻る
			if (selectDecision == 9)
			{
				m_returnMenu = false;

				player.SetRoll(0);
			}
			//タイトルに戻る
			if (selectDecision == 10)
			{
				m_titleMenu = true;
			}

			//リセット
			cWaitTime = 0;
		}
		//Bボタンが押されたら
		else if (m_xpad.Buttons[13] == 1)
		{
			PlaySoundMem(pse->GetButtonSE(), DX_PLAYTYPE_BACK, true);

			//元の画面に戻る
			m_returnMenu = false;
			//リセット
			cWaitTime = 0;

			player.SetRoll(0);

		}
	}
	else
	{
		cWaitTime++;
	}

}

/// <summary>
/// ステータス画面を変える更新処理
/// </summary>
void Setting::MenuChange()
{
	//パッド入力所得
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	//Rボタンが押されたら
	if (m_xpad.Buttons[9] == 1)
	{
		//装備画面から戻る
		m_equipmentMenu = false;
		//アイテム画面に移る
		m_itemMenu = true;
	}
	//Lボタンが押されたら
	else if (m_xpad.Buttons[8] == 1)
	{
		//アイテム画面から戻る
		m_itemMenu = false;
		//装備画面に移る
		m_equipmentMenu = true;
	}
}

/// <summary>
/// 装備画面更新処理
/// </summary>
void Setting::EquipmentUpdate()
{
	//パッド入力所得
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	m_change.oneX = cEquipmentOneX;
	m_change.oneY = cEquipmentOneY;
	m_change.secondX = cEquipmentSecondX;
	m_change.secondY = cEquipmentSecondY;

	m_select.right = false;
	m_select.left = false;
	m_select.armor = false;

	//右
	if (m_xpad.Buttons[3] == 1)
	{
		m_thumb--;
	}
	//左
	else if (m_xpad.Buttons[2] == 1)
	{
		m_thumb++;
	}
	else
	{
		//初期化
		m_thumb = 0;

		m_one = false;
	}

	pselect->Menu_Update(m_thumb, m_one, m_xpad.Buttons[12], selectDecision, pselect->Eight);


	if (cWaitTime >= 10)
	{
		//Bボタンが押されたら
		if (m_xpad.Buttons[13] == 1)
		{
			//装備画面から戻る
			m_equipmentMenu = false;
			//リセット
			cWaitTime = 0;
		}


		//Aボタンを押したら
		if (m_xpad.Buttons[12] == 1)
		{
			//右装備選択
			//右アイテム選択
			if (selectDecision == 8)
			{
				m_select.right = true;
			}
			//左装備選択
			//右指輪選択
			//真ん中アイテム選択
			else if (selectDecision == 9)
			{
				m_select.left = true;
			}
			//防具選択
			//左指輪選択
			//左アイテム選択
			else if (selectDecision == 10)
			{
				m_select.armor = true;
			}

			//装備を開く
			m_decisionEquipment = true;

			//リセット
			cWaitTime = 0;
		}
	}
	else
	{
		cWaitTime++;
	}

}

/// <summary>
/// 休息の更新処理
/// </summary>
void Setting::RestUpdate(Player& player, CoreManager& core)
{
	//パッド入力所得
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	//上
	if (m_xpad.Buttons[0] == 1)
	{
		m_button++;
	}
	//下
	else if (m_xpad.Buttons[1] == 1)
	{
		m_button--;
	}
	else
	{
		//初期化
		m_button = 0;

		m_one = false;
	}


	//普通の休息だった場合
	if (!player.GetBigRest())
	{
		pselect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, pselect->Nine);

		if (cWaitTime >= 10)
		{
			//Aボタンが押されたら
			if (m_xpad.Buttons[12] == 1)
			{
				PlaySoundMem(pse->GetButtonSE(), DX_PLAYTYPE_BACK, true);

				//転送
				if (selectDecision == 9)
				{
					//まだマップがないから実装しない
				}
				//休息をやめる
				if (selectDecision == 10)
				{
					player.SetNotRest(false);

					player.SetRoll(0);
				}

				//リセット
				cWaitTime = 0;
			}

			if (m_xpad.Buttons[13] == 1)
			{
				PlaySoundMem(pse->GetButtonSE(), DX_PLAYTYPE_BACK, true);

				player.SetNotRest(false);
			}
		}
		else
		{
			cWaitTime++;
		}
		
	}
	//レベルを上げられる休息だった場合
	else
	{
		pselect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, pselect->Eight);

		if (cWaitTime >= 10)
		{
			//Aボタンが押されたら
			if (m_xpad.Buttons[12] == 1)
			{
				PlaySoundMem(pse->GetButtonSE(), DX_PLAYTYPE_BACK, true);

				//レベルを上げる
				if (selectDecision == 8)
				{
					m_statusLevel = true;

					ms_levelUP.sl_hp = player.GetHPLevel();
					ms_levelUP.sl_stamina = player.GetStaminaLevel();
					ms_levelUP.sl_muscle = player.GetMuscleLevel();
					ms_levelUP.sl_skill = player.GetSkillLevel();

					m_core = core.GetCore();

				}
				//転送
				if (selectDecision == 9)
				{
					//まだマップがないから実装しない

				}
				//休息をやめる
				if (selectDecision == 10)
				{
					player.SetNotRest(false);
				}

				//リセット
				cWaitTime = 0;
			}
		}
		else
		{
			cWaitTime++;
		}
		
	}

	
}

/// <summary>
/// レベルアップ処理
/// </summary>
/// <param name="player"></param>
void Setting::LevelUpdate(Player& player, CoreManager& core)
{

	ms_levelUP.sl_all = ms_levelUP.sl_hp + ms_levelUP.sl_muscle + ms_levelUP.sl_skill + ms_levelUP.sl_stamina;

	//パッド入力所得
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	//上
	if (m_xpad.Buttons[0] == 1)
	{
		m_button++;
	}
	//下
	else if (m_xpad.Buttons[1] == 1)
	{
		m_button--;
	}
	else
	{
		//初期化
		m_button = 0;

		m_one = false;
	}

	pselect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, pselect->Six);

	//必要コアより多かったらレベルが上げられる
	if (core.NeedCore(ms_levelUP.sl_all) <= m_core)
	{
		m_up = true;
	}
	else
	{
		m_up = false;
	}

	if (cWaitTime >= 10)
	{
		//Bボタンが押されたら
		if (m_xpad.Buttons[13] == 1)
		{
			//ステータスレベル画面を閉じる
			m_statusLevel = false;

			cWaitTime = 0;
		}
		
		LevelUp(core, player.GetHPLevel(), ms_levelUP.sl_hp, pselect->Six);   //HP
		LevelUp(core, player.GetStaminaLevel(), ms_levelUP.sl_stamina, pselect->Seven); //Stamina
		LevelUp(core, player.GetMuscleLevel(), ms_levelUP.sl_muscle, pselect->Eight); //Muscle
		LevelUp(core, player.GetSkillLevel(), ms_levelUP.sl_skill, pselect->Nine);  //Skill
		
		
		//Aボタンが押されたら
		if (m_xpad.Buttons[12] == 1)
		{
			//上げたレベルとコアを決定する
			if (selectDecision == 10)
			{
				m_statusLevel = false;

				player.SetHPLevel(ms_levelUP.sl_hp);
				player.SetStaminaLevel(ms_levelUP.sl_stamina);
				player.SetMuscleLevel(ms_levelUP.sl_muscle);
				player.SetSkillLevel(ms_levelUP.sl_skill);

				core.SetCore(m_core);

				cWaitTime = 0;
			}
		}

	}
	else
	{
		cWaitTime++;
	}
}

void Setting::LevelUp(CoreManager& core, int origin, int& level, int now)
{
	if (cPush >= 30)
	{
		if (pselect->NowSelect == now)
		{
			//左(レベルを戻す)
			if (m_xpad.Buttons[2] == 1)
			{
				//レベルが高かったら戻せる
				if (origin < level)
				{
					m_core += core.NeedCore(ms_levelUP.sl_all - 1);

					level--;
				}

				cPush = 0;
			}
			//右(レベルを上げる)
			else if (m_xpad.Buttons[3] == 1)
			{
				//レベルを上げることが可能なら
				if (m_up)
				{
					m_core -= core.NeedCore(ms_levelUP.sl_all);

					level++;
				}

				cPush = 0;
			}
		}
	}
	else
	{
		cPush++;
	}
	
}

/// <summary>
/// アイテムボックス画面更新処理
/// </summary>
void Setting::ItemBoxUpdate()
{
	//パッド入力所得
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	m_change.oneX = cItemOneX;
	m_change.oneY = cItemOneY;
	m_change.secondX = cItemSecondX;
	m_change.secondY = cItemSecondY;

	//Bボタンが押されたら
	if (m_xpad.Buttons[13] == 1)
	{
		//装備画面から戻る
		m_itemMenu = false;
	}
}

/// <summary>
/// 装備選択画面更新処理
/// </summary>
void Setting::EquipmentDecisionUpdate(Weapon& weapon, Shield& shield, Armor& armor, ItemManager& item)
{
	//パッド入力所得
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	//上
	if (m_xpad.Buttons[0] == 1)
	{
		m_button++;
	}
	//下
	else if (m_xpad.Buttons[1] == 1)
	{
		m_button--;
	}
	else
	{
		//初期化
		m_button = 0;

		m_one = false;
	}

	//右装備だった場合
	if (m_select.right)
	{
		if (item.GetItem().BlackSword >= 1)
		{
			pselect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, pselect->Nine);

			cNo = true;
		}
		else if (item.GetItem().BlackSword <= 0)
		{
			pselect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, pselect->Ten);

			cNo = false;
		}
	}
	//左装備だった場合
	else if (m_select.left)
	{
		if (item.GetItem().Distorted >= 1)
		{
			pselect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, pselect->Nine);

			cNo = true;
		}
		else if (item.GetItem().Distorted <= 0)
		{
			pselect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, pselect->Ten);

			cNo = false;
		}
	}
	//防具だった場合
	else if (m_select.armor)
	{
		if (item.GetItem().ArmorNormal >= 1)
		{
			pselect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, pselect->Nine);

			cNo = true;
		}
		else if (item.GetItem().ArmorNormal <= 0)
		{
			pselect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, pselect->Ten);

			cNo = false;
		}
	}


	if (cWaitTime >= 10)
	{
		//Bボタンが押されたら
		if (m_xpad.Buttons[13] == 1)
		{
			//装備画面から戻る
			m_decisionEquipment = false;

			cWaitTime = 0;
		}

		//Aボタンを押したら
		if (m_xpad.Buttons[12] == 1)
		{
			if (selectDecision == 8)
			{
				
			}
			else if (selectDecision == 9)
			{
				//右装備だった場合
				if (m_select.right)
				{
					weapon.End();

					weapon.SetFist(true);
					weapon.SetBlack(false);

					weapon.Init();
				}
				//左装備だった場合
				else if (m_select.left)
				{
					shield.End();

					shield.SetFist(true);
					shield.SetUgly(false);

					shield.Init();
				}
				//防具だった場合
				else if (m_select.armor)
				{
					armor.SetBody(true);
					armor.SetCommon(false);

					armor.Init();
				}
			}
			else if (selectDecision == 10)
			{
				//力技
				if (cNo)
				{
					//右装備だった場合
					if (m_select.right)
					{
						weapon.End();

						weapon.SetBlack(true);
						weapon.SetFist(false);

						weapon.Init();
					}
					//左装備だった場合
					else if (m_select.left)
					{
						shield.End();

						shield.SetUgly(true);
						shield.SetFist(false);

						shield.Init();
					}
					//防具だった場合
					else if (m_select.armor)
					{
						armor.SetCommon(true);
						armor.SetBody(false);

						armor.Init();
					}
				}
				//力技
				else if (!cNo)
				{
					//右装備だった場合
					if (m_select.right)
					{
						weapon.End();

						weapon.SetFist(true);
						weapon.SetBlack(false);

						weapon.Init();
					}
					//左装備だった場合
					else if (m_select.left)
					{
						shield.End();

						shield.SetFist(true);
						shield.SetUgly(false);

						shield.Init();
					}
					//防具だった場合
					else if (m_select.armor)
					{
						armor.SetBody(true);
						armor.SetCommon(false);

						armor.Init();
					}
				}
				
			}

			//装備を開く
			m_decisionEquipment = false;

			//リセット
			cWaitTime = 0;
		}
	}
	else
	{
		cWaitTime++;
	}



}

/// <summary>
/// 描画
/// </summary>
void Setting::Draw()
{
	//背景の色を薄くする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawGraph(0, 0, m_back, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//選択中の色を変える
	if (m_brightness == false && m_volume == false)
	{
		if (pselect->NowSelect == pselect->Eight)
		{
			m_brightnessColor = 0xffff00;
			m_bgmColor = 0xffffff;
			m_returnColor = 0xffffff;
		}
		if (pselect->NowSelect == pselect->Nine)
		{
			m_brightnessColor = 0xffffff;
			m_bgmColor = 0xffff00;
			m_returnColor = 0xffffff;
		}
		if (pselect->NowSelect == pselect->Ten)
		{
			m_brightnessColor = 0xffffff;
			m_bgmColor = 0xffffff;
			m_returnColor = 0xffff00;
		}
	}
	//明るさを選択
	if (m_brightness == true)
	{
		BrightColorDraw(5, 0, 1, 2, 3, 4, 125, 0);
		BrightColorDraw(6, 1, 0, 2, 3, 4, 125 / 2, 0);
		BrightColorDraw(7, 2, 1, 0, 3, 4, 0, 0);
		BrightColorDraw(8, 3, 1, 2, 0, 4, 0, 125 / 2);
		BrightColorDraw(9, 4, 1, 2, 3, 0, 0, 125);
	}

	//音量を選択
	if (m_volume == true)
	{
		VolumeColorDraw(5, 0, 1, 2, 3, 4, 0);
		VolumeColorDraw(6, 1, 0, 2, 3, 4, 60);
		VolumeColorDraw(7, 2, 1, 0, 3, 4, 130);
		VolumeColorDraw(8, 3, 1, 2, 0, 4, 190);
		VolumeColorDraw(9, 4, 1, 2, 3, 0, 255);
	}

	//フォントのサイズ変更
	SetFontSize(150);

	DrawString(100, 70, "設定", 0xffffff);

	SetFontSize(100);

	DrawString(100, 340, "明るさ", m_brightnessColor);
	DrawString(100, 500, "音量", m_bgmColor);
	DrawString(100, 660, "戻る", m_returnColor);

	DrawBox(500, 340, 620, 460, m_brightColor[0], true);
	DrawBox(700, 340, 820, 460, m_brightColor[1], true);
	DrawBox(900, 340, 1020, 460, m_brightColor[2], true);
	DrawBox(1100, 340, 1220, 460, m_brightColor[3], true);
	DrawBox(1300, 340, 1420, 460, m_brightColor[4], true);

	DrawBox(500, 500, 620, 620, m_volumeColor[0], true);
	DrawBox(700, 500, 820, 620, m_volumeColor[1], true);
	DrawBox(900, 500, 1020, 620, m_volumeColor[2], true);
	DrawBox(1100, 500, 1220, 620, m_volumeColor[3], true);
	DrawBox(1300, 500, 1420, 620, m_volumeColor[4], true);

	pselect->Draw();

	//フォントのサイズを戻す
	SetFontSize(40);
}

/// <summary>
/// 選択中の色を変える
/// </summary>
/// <param name="select">列挙型</param>
/// <param name="now">選択してるもの</param>
/// <param name="other1">それ以外１</param>
/// <param name="other2">それ以外２</param>
/// <param name="other3">それ以外３</param>
/// <param name="other4">それ以外４</param>
/// <param name="black">黒い画像のブレンド率</param>
/// <param name="white">白い画像のブレンド率</param>
void Setting::BrightColorDraw(int select, int now, int other1, int other2, int other3, int other4, int black, int white)
{
	if (pselect->NowSelect == select)
	{
		m_brightColor[now] = 0xffff00;
		m_brightColor[other1] = 0xffffff;
		m_brightColor[other2] = 0xffffff;
		m_brightColor[other3] = 0xffffff;
		m_brightColor[other4] = 0xffffff;

		m_blackPal = black;
		m_whitePal = white;
	}
}

/// <summary>
/// 選択中の色を変える
/// </summary>
/// <param name="select">列挙型</param>
/// <param name="now">選択してるもの</param>
/// <param name="other1">それ以外１</param>
/// <param name="other2">それ以外２</param>
/// <param name="other3">それ以外３</param>
/// <param name="other4">それ以外４</param>
/// <param name="volume">音量</param>
void Setting::VolumeColorDraw(int select, int now, int other1, int other2, int other3, int other4, int volume)
{
	if (pselect->NowSelect == select)
	{
		m_volumeColor[now] = 0xffff00;
		m_volumeColor[other1] = 0xffffff;
		m_volumeColor[other2] = 0xffffff;
		m_volumeColor[other3] = 0xffffff;
		m_volumeColor[other4] = 0xffffff;

		m_volumeSize = volume;
	}
}

/// <summary>
/// 設定の描画処理
/// </summary>
/// <param name="volume">音量</param>
void Setting::SettingDraw(int volume)
{
	pse->Update(volume);

	//明るさ
	if (brightDecision == 6)
	{
		m_blackPal = 125;
		m_whitePal = 0;
	}
	if (brightDecision == 7)
	{
		m_blackPal = 125 / 2;
		m_whitePal = 0;
	}
	if (brightDecision == 8)
	{
		m_blackPal = 0;
		m_whitePal = 0;
	}
	if (brightDecision == 9)
	{
		m_blackPal = 0;
		m_whitePal = 125 / 2;
	}
	if (brightDecision == 10)
	{
		m_blackPal = 0;
		m_whitePal = 125;
	}

	//音量
	if (volumeDecision == 6)
	{
		m_volumeSize = 0;
	}
	if (volumeDecision == 7)
	{
		m_volumeSize = 60;
	}
	if (volumeDecision == 8)
	{
		m_volumeSize = 130;
	}
	if (volumeDecision == 9)
	{
		m_volumeSize = 190;
	}
	if (volumeDecision == 10)
	{
		m_volumeSize = 255;
	}

	//画面を暗くする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_blackPal);
	DrawGraph(0, 0, m_black, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//画面を明るくする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_whitePal);
	DrawGraph(0, 0, m_white, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Setting::MenuDraw()
{

	if (pselect->NowSelect == pselect->Eight)
	{
		m_menuColor[0] = 0xffff00;
		m_menuColor[1] = 0xffffff;
		m_menuColor[2] = 0xffffff;
	}
	if (pselect->NowSelect == pselect->Nine)
	{
		m_menuColor[0] = 0xffffff;
		m_menuColor[1] = 0xffff00;
		m_menuColor[2] = 0xffffff;
	}
	if (pselect->NowSelect == pselect->Ten)
	{
		m_menuColor[0] = 0xffffff;
		m_menuColor[1] = 0xffffff;
		m_menuColor[2] = 0xffff00;
	}

	//フォントのサイズ変更
	SetFontSize(150);

	DrawString(100, 70, "メニュー", 0xffffff);

	SetFontSize(100);

	DrawString(100, 340, "装備", m_menuColor[0]);
	DrawString(100, 500, "戻る", m_menuColor[1]);
	DrawString(100, 660, "タイトルへ", m_menuColor[2]);

	//フォントのサイズを戻す
	SetFontSize(40);
}

/// <summary>
/// メニュー背景描画
/// </summary>
void Setting::MenuBackDraw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawGraph(0, 0, m_back, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// メニューを変更する所の描画処理
/// </summary>
void Setting::MenuChangeDraw()
{
	if (!m_blend)
	{
		if (cBlenda < 100)
		{
			cBlenda++;
		}
		else
		{
			m_blend = true;
		}
	}
	else if (m_blend)
	{
		if (cBlenda > 10)
		{
			cBlenda--;
		}
		else
		{
			m_blend = false;
		}
	}


	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cBlenda);
	DrawBox(m_change.oneX, m_change.oneY, m_change.secondX, m_change.secondY, 0x000fff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// 装備画面
/// </summary>
void Setting::EquipmentDraw()
{
	DrawGraph(0, 0, m_equipment, true);

	if (pselect->NowSelect == pselect->Eight)
	{
		m_equipmentColorPos.oneX = cRightEquipmentOneX;
		m_equipmentColorPos.oneY = cRightEquipmentOneY;
		m_equipmentColorPos.secondX = cRightEquipmentSecondX;
		m_equipmentColorPos.secondY = cRightEquipmentSecondY;
	}
	else if (pselect->NowSelect == pselect->Nine)
	{
		m_equipmentColorPos.oneX = cLeftEquipmentOneX;
		m_equipmentColorPos.oneY = cLeftEquipmentOneY;
		m_equipmentColorPos.secondX = cLeftEquipmentSecondX;
		m_equipmentColorPos.secondY = cLeftEquipmentSecondY;
	}
	else if (pselect->NowSelect == pselect->Ten)
	{
		m_equipmentColorPos.oneX = cArmorEquipmentOneX;
		m_equipmentColorPos.oneY = cArmorEquipmentOneY;
		m_equipmentColorPos.secondX = cArmorEquipmentSecondX;
		m_equipmentColorPos.secondY = cArmorEquipmentSecondY;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawBox(m_equipmentColorPos.oneX, m_equipmentColorPos.oneY, m_equipmentColorPos.secondX, m_equipmentColorPos.secondY, 0x000fff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// 休息画面
/// </summary>
void Setting::RestDraw(bool rest)
{
	DrawRotaGraph(300, 450, 1.5f, DX_PI_F / 2, m_rest, true);

	m_selectX = 130;

	DrawGraph(m_selectX, m_selectY, m_selectUi, true);

	//普通の休息
	if (!rest)
	{
		if (pselect->NowSelect == pselect->Nine)
		{
			m_selectY = 255;

			m_menuColor[0] = 0x000000;
			m_menuColor[1] = 0xffffff;
		}
		else if (pselect->NowSelect == pselect->Ten)
		{
			m_selectY = 355;

			m_menuColor[0] = 0xffffff;
			m_menuColor[1] = 0x000000;
		}

		//フォントのサイズ変更
		SetFontSize(60);

		DrawString(150, 50, "普通の休息", 0xffffff);

		//フォントのサイズ変更
		SetFontSize(40);

		DrawString(150, 300, "転送", m_menuColor[0]);
		DrawString(150, 400, "立ち去る", m_menuColor[1]);
	}
	//レベル上げられる休息
	else
	{
		if (pselect->NowSelect == pselect->Eight)
		{
			m_selectY = 255;

			m_menuColor[0] = 0x000000;
			m_menuColor[1] = 0xffffff;
			m_menuColor[2] = 0xffffff;
		}
		else if (pselect->NowSelect == pselect->Nine)
		{
			m_selectY = 355;

			m_menuColor[0] = 0xffffff;
			m_menuColor[1] = 0x000000;
			m_menuColor[2] = 0xffffff;
		}
		else if (pselect->NowSelect == pselect->Ten)
		{
			m_selectY = 455;

			m_menuColor[0] = 0xffffff;
			m_menuColor[1] = 0xffffff;
			m_menuColor[2] = 0x000000;
		}

		//フォントのサイズ変更
		SetFontSize(60);

		DrawString(150, 50, "魂器", 0xffffff);

		//フォントのサイズ変更
		SetFontSize(40);

		DrawString(150, 300, "レベル上げ", m_menuColor[0]);
		DrawString(150, 400, "転送", m_menuColor[1]);
		DrawString(150, 500, "立ち去る", m_menuColor[2]);
	}
}

/// <summary>
/// レベルアップ描画
/// </summary>
/// <param name="player"></param>
void Setting::LevelUpDraw(Player& player, CoreManager& core)
{

	DrawGraph(-50, 0, m_levelUp, true);
	DrawRotaGraph(m_selectX, m_selectY, cGraphSize, DX_PI_F, m_selectUi, true);

	if (pselect->NowSelect == pselect->Six)
	{
		m_selectX = 300;
		m_selectY = 540;
		cGraphSize = 1.0f;

		m_menuColor[0] = 0x000000;
		m_menuColor[1] = 0xffffff;
		m_menuColor[2] = 0xffffff;
		m_menuColor[3] = 0xffffff;
		m_menuColor[4] = 0xffffff;
	}
	else if (pselect->NowSelect == pselect->Seven)
	{
		m_selectX = 300;
		m_selectY = 635;
		cGraphSize = 1.0f;

		m_menuColor[0] = 0xffffff;
		m_menuColor[1] = 0x000000;
		m_menuColor[2] = 0xffffff;
		m_menuColor[3] = 0xffffff;
		m_menuColor[4] = 0xffffff;
	}
	else if (pselect->NowSelect == pselect->Eight)
	{
		m_selectX = 300;
		m_selectY = 730;
		cGraphSize = 1.0f;

		m_menuColor[0] = 0xffffff;
		m_menuColor[1] = 0xffffff;
		m_menuColor[2] = 0x000000;
		m_menuColor[3] = 0xffffff;
		m_menuColor[4] = 0xffffff;
	}
	else if (pselect->NowSelect == pselect->Nine)
	{
		m_selectX = 300;
		m_selectY = 825;
		cGraphSize = 1.0f;

		m_menuColor[0] = 0xffffff;
		m_menuColor[1] = 0xffffff;
		m_menuColor[2] = 0xffffff;
		m_menuColor[3] = 0x000000;
		m_menuColor[4] = 0xffffff;
	}
	else if (pselect->NowSelect == pselect->Ten)
	{
		m_selectX = 815;
		m_selectY = 930;
		cGraphSize = 1.3f;

		m_menuColor[0] = 0xffffff;
		m_menuColor[1] = 0xffffff;
		m_menuColor[2] = 0xffffff;
		m_menuColor[3] = 0xffffff;
		m_menuColor[4] = 0x000000;
	}


	//フォントのサイズ変更
	SetFontSize(40);

	DrawFormatString(90, 150, 0xffffff, "レベル      %d", ms_levelUP.sl_all);
	DrawFormatString(90, 250, 0xffffff, "所持コア  %d", m_core);
	DrawFormatString(90, 300, 0xffffff, "必要コア  %d", core.NeedCore(ms_levelUP.sl_all));

	//左の変数がレベル上げる前のレベルで右の変数がレベルを上げた後のレベル
	DrawFormatString(290, 520, m_menuColor[0], "%d", ms_levelUP.sl_hp);
	DrawFormatString(290, 615, m_menuColor[1], "%d", ms_levelUP.sl_stamina);
	DrawFormatString(290, 710, m_menuColor[2], "%d", ms_levelUP.sl_muscle);
	DrawFormatString(290, 805, m_menuColor[3], "%d", ms_levelUP.sl_skill);

	//レベルを上げた時のステータス変動
	DrawFormatString(600, 200, 0xffffff, "最大生命力 :   %d", static_cast<int>(player.GetStatus().s_hp + ((ms_levelUP.sl_hp - 1) * 30.0f)));
	DrawFormatString(600, 400, 0xffffff, "最大体力   :   %d", static_cast<int>(player.GetStatus().s_stamina + ((ms_levelUP.sl_stamina - 1) * 10.0f)));
	DrawFormatString(600, 600, 0xffffff, "攻撃力     :   %d", static_cast<int>(player.GetAttackDamage() + ((ms_levelUP.sl_muscle - 1) * player.GetAttackMuscle()) + ((ms_levelUP.sl_skill - 1) * player.GetAttackSkill())));

	//フォントのサイズ変更
	SetFontSize(60);

	DrawString(750, 900, "決定", m_menuColor[4]);

	//フォントのサイズ変更
	SetFontSize(40);
}

/// <summary>
/// アイテム画面
/// </summary>
void Setting::ItemBoxDraw()
{
	DrawGraph(0, 0, m_itemBox, true);
}

/// <summary>
/// 装備選択画面
/// </summary>
void Setting::EquipmentDecisionDraw(ItemManager& item)
{
	DrawGraph(0, 0, m_selectEquipment, true);

	//右装備だった場合
	if (m_select.right)
	{
		if (item.GetItem().BlackSword >= 1)
		{
			if (pselect->NowSelect == pselect->Nine)
			{
				m_selectObject.oneX = cOneX;
				m_selectObject.oneY = cOneY;
				m_selectObject.secondX = cSecondX;
				m_selectObject.secondY = cSecondY;
			}
			else if (pselect->NowSelect == pselect->Ten)
			{
				m_selectObject.oneY = cOneY + cDifferenceY;
				m_selectObject.secondY = cSecondY + cDifferenceY;
			}
		}
		else if (item.GetItem().BlackSword <= 0)
		{
			if (pselect->NowSelect == pselect->Ten)
			{
				m_selectObject.oneX = cOneX;
				m_selectObject.oneY = cOneY;
				m_selectObject.secondX = cSecondX;
				m_selectObject.secondY = cSecondY;
			}
		}
	}
	//左装備だった場合
	else if (m_select.left)
	{
		if (item.GetItem().Distorted >= 1)
		{
			if (pselect->NowSelect == pselect->Nine)
			{
				m_selectObject.oneX = cOneX;
				m_selectObject.oneY = cOneY;
				m_selectObject.secondX = cSecondX;
				m_selectObject.secondY = cSecondY;
			}
			else if (pselect->NowSelect == pselect->Ten)
			{
				m_selectObject.oneY = cOneY + cDifferenceY;
				m_selectObject.secondY = cSecondY + cDifferenceY;
			}
		}
		else if (item.GetItem().Distorted <= 0)
		{
			if (pselect->NowSelect == pselect->Ten)
			{
				m_selectObject.oneX = cOneX;
				m_selectObject.oneY = cOneY;
				m_selectObject.secondX = cSecondX;
				m_selectObject.secondY = cSecondY;
			}
		}
	}
	//防具だった場合
	else if (m_select.armor)
	{
		if (item.GetItem().ArmorNormal >= 1)
		{
			if (pselect->NowSelect == pselect->Nine)
			{
				m_selectObject.oneX = cOneX;
				m_selectObject.oneY = cOneY;
				m_selectObject.secondX = cSecondX;
				m_selectObject.secondY = cSecondY;
			}
			else if (pselect->NowSelect == pselect->Ten)
			{
				m_selectObject.oneY = cOneY + cDifferenceY;
				m_selectObject.secondY = cSecondY + cDifferenceY;
			}
		}
		else if (item.GetItem().ArmorNormal <= 0)
		{
			if (pselect->NowSelect == pselect->Ten)
			{
				m_selectObject.oneX = cOneX;
				m_selectObject.oneY = cOneY;
				m_selectObject.secondX = cSecondX;
				m_selectObject.secondY = cSecondY;
			}
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawBox(m_selectObject.oneX, m_selectObject.oneY, m_selectObject.secondX, m_selectObject.secondY, 0x000fff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// 終了処理
/// </summary>
void Setting::End()
{
	//メモリ解放
	DeleteGraph(m_black);
	DeleteGraph(m_back);
	DeleteGraph(m_white);
	DeleteGraph(m_equipment);
	DeleteGraph(m_itemBox);
	DeleteGraph(m_selectEquipment);
	DeleteGraph(m_rest);
	DeleteGraph(m_selectUi);
	DeleteGraph(m_levelUp);
	pse->End();
}
