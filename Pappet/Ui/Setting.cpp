#include "Setting.h"
#include "Manager/SelectManager.h"
#include "UI/UIBase.h"
#include "Item/Shield.h"
#include "Item/Weapon.h"
#include "Item/Armor.h"
#include "Character/Player.h"
#include "Manager/CoreManager.h"
#include "Manager/ItemManager.h"
#include "Manager/SEManager.h"
#include "External/Font.h"

namespace
{
	int selectDecision = 0;     //選択したものを決定する変数
	int brightDecision = 0;     //明るさを決定する変数
	int volumeDecision = 0;     //音量を決定する変数
	int seDecision = 0;         //SEを決定する変数
	int cameraDecision = 0;     //カメラを決定する変数

	//a値をいじる変数
	int cBlenda = 10;  

	//画像の倍率
	float cGraphSize = 1.0f;

	//間違って押さないようにする
	int cWaitTime = 0;
	//連続で押さないようにする
	int cPush = 0;
	//時間が立つと消えるようにする
	int cTime = 0;
	//文字が消えるまでの時間
	constexpr int cTimeMax = 80;

	//暗さ
	int cBrightnessBlack = 0;
	float cBrightnessBar = 0.5f;
	//明るさ
	int cBrightnessWhite = 0;
	//音量
	int cVolume = 125;
	float cVolumeBar = 0.6f;
	//SE音量
	int cSe = 150;
	float cSeBar = 0.7f;
	//カメラの感度
	float cCamera = 2.0f;
	float cCameraBar = 0.1f;

	// バーの描画
	constexpr int cBarWidth = 600; //バーの幅
	constexpr int cBarHeight = 20; //バーの高さ

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
	m_seColor(0),
	m_cameraColor(0),
	m_returnColor(0),
	m_right(0),
	m_left(0),
	m_armor(0),
	m_one(false),
	m_settingScene(false),
	m_brightness(false),
	m_volume(false),
	m_se(false),
	m_camera(false),
	m_volumeSize(0),
	m_seSize(0),
	m_cameraSize(0.0f),
	m_brightBar(0.0f),
	m_volumeBar(0.0f),
	m_seBar(0.0f),
	m_cameraBar(0.0f),
	m_equipmentMenu(false),
	m_itemMenu(false),
	m_decisionEquipment(false),
	m_blend(false),
	m_returnMenu(true),
	m_titleMenu(false),
	m_statusLevel(false),
	m_up(false),
	m_menuDecision(false),
	m_restWarp(false),
	m_caveat(false),
	m_reset(false),
	m_change(),
	m_core(0),
	m_equipmentColorPos(),
	m_selectObject(),
	ms_levelUP(),
	m_xpad()
{

	for (int i = 0; i < 5; i++)
	{
		m_menuSelect[i] = 0;
		m_menuColor[i] = 0;
	}

	m_brightBar = cBrightnessBar;
	m_volumeBar = cVolumeBar;
	m_seBar = cSeBar;
	m_cameraBar = cCameraBar;

	m_pFont = std::make_shared<Font>();
	m_pSmallFont = std::make_shared<Font>();
	m_pBigFont = std::make_shared<Font>();
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

	m_blackPal = cBrightnessBlack;
	m_whitePal = cBrightnessWhite;
	m_volumeSize = cVolume;
	m_seSize = cSe;
	m_cameraSize = cCamera;

	m_menuSelect[0] = 1;
	m_menuSelect[1] = 0;
	m_menuSelect[2] = 0;

	m_button = 0;
	m_thumb = 0;
	m_one = false;
	m_settingScene = false;
	m_brightness = false;
	m_volume = false;
	m_equipmentMenu = false;
	m_returnMenu = true;
	m_titleMenu = false;
	m_menuDecision = false;

	m_brightnessColor = 0xffffff;
	m_bgmColor = 0xffffff;
	m_returnColor = 0xffffff;

	m_equipment = MyLoadGraph("Data/UI/装備画面UI.png", 1, 1);
	m_itemBox = MyLoadGraph("Data/UI/アイテム画面UI.png", 1, 1);
	m_selectEquipment = MyLoadGraph("Data/UI/装備選択画面UI.png", 1, 1);
	m_rest = MyLoadGraph("Data/UI/四角の白い長方形の囲い.png", 1, 1);
	m_selectUi = MyLoadGraph("Data/UI/選択UI.png", 2, 2);
	m_levelUp = MyLoadGraph("Data/UI/レベルステータスUI.png", 1, 1);
	m_menu = MyLoadGraph("Data/UI/MenuUI.png", 1, 1);

	m_pFont->FontInit(50);
	m_pSmallFont->FontInit(30);
	m_pBigFont->FontInit(80);
}

/// <summary>
/// 更新処理
/// </summary>
void Setting::Update(SEManager& se)
{
	//パッド入力所得
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	if (m_waitTime > 10)
	{
		//上
		if (m_xpad.ThumbLY > 2000 || m_xpad.Buttons[0] == 1)
		{
			m_button++;
		}
		//下
		else if (m_xpad.ThumbLY < 0 || m_xpad.Buttons[1] == 1)
		{
			m_button--;
		}
		//右
		else if (m_xpad.ThumbLX > 2000 || m_xpad.Buttons[3] == 1)
		{
			m_thumb--;
		}
		//左
		else if (m_xpad.ThumbLX < 0 || m_xpad.Buttons[2] == 1)
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
		if (m_brightness == false && m_volume == false && m_se == false && m_camera == false)
		{
			m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Six);
		}
		//明るさの選択中
		if (m_brightness == true && m_volume == false && m_se == false && m_camera == false)
		{
			m_pSelect->Menu_Update(m_thumb, m_one, m_xpad.Buttons[13], brightDecision, m_pSelect->One);
		}
		//音量の選択中
		if (m_brightness == false && m_volume == true && m_se == false && m_camera == false)
		{
			m_pSelect->Menu_Update(m_thumb, m_one, m_xpad.Buttons[13], volumeDecision, m_pSelect->One);
		}
		//SEの選択中
		if (m_brightness == false && m_volume == false && m_se == true && m_camera == false)
		{
			m_pSelect->Menu_Update(m_thumb, m_one, m_xpad.Buttons[13], seDecision, m_pSelect->One);
		}
		//カメラ感度の選択中
		if (m_brightness == false && m_volume == false && m_se == false && m_camera == true)
		{
			m_pSelect->Menu_Update(m_thumb, m_one, m_xpad.Buttons[13], cameraDecision, m_pSelect->One);
		}

		if (m_brightness)
		{
			//暗さを決める
			SettingChange(m_blackPal, cBrightnessBlack, 125, 100, 75, 50, 0, 0, 0, 0, 0, 0);
			//明るさを決める
			SettingChange(m_whitePal, cBrightnessWhite, 0, 0, 0, 0, 0, 25, 50, 75, 100, 125);
		}
		else if(m_volume)
		{
			//音量を決める
			SettingChange(m_volumeSize, cVolume, 0, 25, 50, 75, 100, 125, 150, 175, 200, 225);
		}
		else if (m_se)
		{
			//SEを決める
			SettingChange(m_seSize, cSe, 0, 25, 50, 75, 100, 125, 150, 175, 200, 225);
		}
		else if (m_camera)
		{
			//カメラ感度を決める
			CameraChange(m_cameraSize, cCamera, 2.0f, 2.3f, 2.5f, 2.7f, 3.0f, 3.2f, 3.4f, 3.6f, 3.8f, 4.0f);
		}


		//Aボタンを押したら
		if (m_xpad.Buttons[12] == 1 && !m_brightness && !m_volume && !m_se && !m_camera)
		{
			PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

			//明るさ設定
			if (selectDecision == 6)
			{
				m_brightness = true;

				//初期化
				m_waitTime = 0;
			}
			//BGM設定
			if (selectDecision == 7)
			{
				m_volume = true;

				//初期化
				m_waitTime = 0;
			}
			//SE設定
			if (selectDecision == 8)
			{
				m_se = true;

				//初期化
				m_waitTime = 0;
			}
			//カメラ感度
			if (selectDecision == 9)
			{
				m_camera = true;

				//初期化
				m_waitTime = 0;
			}
			//元の画面に戻る
			if (selectDecision == 10)
			{
				m_settingScene = false;

				//初期化
				m_waitTime = 0;
			}
		}

		//明るさ設定を押したら
		if (m_brightness == true && m_waitTime != 0)
		{
			//ABボタンを押したら
			if (m_xpad.Buttons[12] == 1 || m_xpad.Buttons[13] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

				m_pSelect->NowSelect = 5;

				//初期化
				m_waitTime = 0;

				m_brightness = false;
			}
		}
		//音量設定を押したら
		else if (m_volume == true && m_waitTime != 0)
		{
			//Bボタンを押したら
			if (m_xpad.Buttons[12] == 1 || m_xpad.Buttons[13] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

				m_pSelect->NowSelect = 6;

				//初期化
				m_waitTime = 0;

				m_volume = false;
			}
		}
		//SE設定を押したら
		else if (m_se == true && m_waitTime != 0)
		{
			//Bボタンを押したら
			if (m_xpad.Buttons[12] == 1 || m_xpad.Buttons[13] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

				m_pSelect->NowSelect = 7;

				//初期化
				m_waitTime = 0;

				m_se = false;
			}
		}
		//カメラ感度を設定
		else if (m_camera == true && m_waitTime != 0)
		{
			//Bボタンを押したら
			if (m_xpad.Buttons[12] == 1 || m_xpad.Buttons[13] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

				m_pSelect->NowSelect = 8;

				//初期化
				m_waitTime = 0;

				m_camera = false;
			}
		}
		//画面に戻る
		else
		{
			//Bボタンを押したら
			if (m_xpad.Buttons[13] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

				m_pSelect->NowSelect = 7;

				//初期化
				m_waitTime = 0;

				m_settingScene = false;
			}
		}
		

		
	}
	else
	{
		m_waitTime++;
	}

	se.Update(m_seSize);
}

/// <summary>
/// メニュー更新処理
/// </summary>
void Setting::MenuUpdate(Player& player, SEManager& se)
{
	//パッド入力所得
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	//右
	if (m_xpad.Buttons[9] == 1 && !m_menuDecision)
	{
		m_button--;
	}
	//左
	else if (m_xpad.Buttons[8] == 1 && !m_menuDecision)
	{
		m_button++;
	}
	else
	{
		//初期化
		m_button = 0;

		m_one = false;
	}

	

	m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Seven);

	if (cWaitTime >= 10)
	{
		//Aボタンが押されたら
		if (m_xpad.Buttons[12] == 1)
		{
			PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

			//装備選択
			if (selectDecision == 7)
			{
				m_equipmentMenu = true;

				//リセット
				cWaitTime = 0;
			}
			//設定
			if (selectDecision == 8)
			{
				//初期化する
				m_waitTime = 0;

				m_settingScene = true;

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

				m_menuDecision = true;
			}

			//リセット
			cWaitTime = 0;
		}
		//Bボタンが押されたら
		else if (m_xpad.Buttons[13] == 1)
		{
			PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

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

	m_pSelect->Menu_Update(m_thumb, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Eight);


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
void Setting::RestUpdate(Player& player, CoreManager& core, bool rest, SEManager& se)
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
		m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Eight);

		if (cWaitTime >= 10)
		{
			//Aボタンが押されたら
			if (m_xpad.Buttons[12] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

				//休息する
				if (selectDecision == 8)
				{
					m_reset = true;
				}
				//転送
				if (selectDecision == 9)
				{
					//休息地点に転送する
					if (rest)
					{
						m_restWarp = true;
					}
					//転送できない
					else
					{
						m_caveat = true;
					}
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

			//Aボタンが押されたら休息をやめる
			if (m_xpad.Buttons[13] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

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
		m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Seven);

		if (cWaitTime >= 10)
		{
			//Aボタンが押されたら
			if (m_xpad.Buttons[12] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

				//休息する
				if (selectDecision == 7)
				{
					m_reset = true;
				}
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
					//休息地点に転送する
					if (rest)
					{
						m_restWarp = true;
					}
					//転送できない
					else
					{
						m_caveat = true;
					}

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

			//Aボタンが押されたら休息をやめる
			if (m_xpad.Buttons[13] == 1)
			{
				PlaySoundMem(se.GetButtonSE(), DX_PLAYTYPE_BACK, true);

				player.SetNotRest(false);
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

	m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Six);

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
		
		LevelUp(core, player.GetHPLevel(), ms_levelUP.sl_hp, m_pSelect->Six);   //HP
		LevelUp(core, player.GetStaminaLevel(), ms_levelUP.sl_stamina, m_pSelect->Seven); //Stamina
		LevelUp(core, player.GetMuscleLevel(), ms_levelUP.sl_muscle, m_pSelect->Eight); //Muscle
		LevelUp(core, player.GetSkillLevel(), ms_levelUP.sl_skill, m_pSelect->Nine);  //Skill
		
		
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
		if (m_pSelect->NowSelect == now)
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
	int Right = 0;
	int Left = 0;
	int Armor = 0;

	m_weaponList.clear();
	m_shieldList.clear();
	m_armorList.clear();

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
		for (const auto& itemName : item.GetItemOrder())
		{
			if (itemName == "BlackSword")
			{
				Right++;

				m_weaponList.push_back("BlackSword");
			}
			else if (itemName == "Bat")
			{
				Right++;

				m_weaponList.push_back("Bat");
			}

		}

		if (Right == 0)
		{
			//武器が0だった場合
			m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Ten);
		}
		else if (Right == 1)
		{
			//武器が1つだった場合
			m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Nine);
		}
		else if (Right == 2)
		{
			//武器が2つだった場合
			m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Eight);
		}

	}
	//左装備だった場合
	else if (m_select.left)
	{
		for (const auto& itemName : item.GetItemOrder())
		{
			if (itemName == "Distorted")
			{
				Left++;

				m_shieldList.push_back("Distorted");
			}
			else if (itemName == "WoodShield")
			{
				Left++;

				m_shieldList.push_back("WoodShield");

			}

		}

		if (Left == 0)
		{
			//盾が0だった場合
			m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Ten);
		}
		else if (Left == 1)
		{
			//盾が1つだった場合
			m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Nine);
		}
		else if (Left == 2)
		{
			//盾が2つだった場合
			m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Eight);
		}
	}
	//防具だった場合
	else if (m_select.armor)
	{
		for (const auto& itemName : item.GetItemOrder())
		{
			if (itemName == "ArmorNormal")
			{
				Armor++;

				m_armorList.push_back("ArmorNormal");
			}

		}

		if (Armor == 0)
		{
			//防具が0だった場合
			m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Ten);
		}
		else if (Armor == 1)
		{
			//防具が1つだった場合
			m_pSelect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, m_pSelect->Nine);
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
			//右装備だった場合
			if (m_select.right)
			{
				WeaponUpdate(m_weaponList, weapon, m_right);
			}
			//左装備だった場合
			else if (m_select.left)
			{
				ShieldUpdate(m_shieldList, shield, m_left);
			}
			//防具だった場合
			else if (m_select.armor)
			{
				ArmorUpdate(m_armorList, armor, m_armor);
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
	if (m_brightness == false && m_volume == false && m_se == false && m_camera == false)
	{
		if (m_pSelect->NowSelect == m_pSelect->Six)
		{
			m_brightnessColor = 0xffff00;
			m_bgmColor = 0xffffff;
			m_seColor = 0xffffff;
			m_cameraColor = 0xffffff;
			m_returnColor = 0xffffff;
		}
		else if (m_pSelect->NowSelect == m_pSelect->Seven)
		{
			m_brightnessColor = 0xffffff;
			m_bgmColor = 0xffff00;
			m_seColor = 0xffffff;
			m_cameraColor = 0xffffff;
			m_returnColor = 0xffffff;
		}
		else if (m_pSelect->NowSelect == m_pSelect->Eight)
		{
			m_brightnessColor = 0xffffff;
			m_bgmColor = 0xffffff;
			m_seColor = 0xffff00;
			m_cameraColor = 0xffffff;
			m_returnColor = 0xffffff;
		}
		else if (m_pSelect->NowSelect == m_pSelect->Nine)
		{
			m_brightnessColor = 0xffffff;
			m_bgmColor = 0xffffff;
			m_seColor = 0xffffff;
			m_cameraColor = 0xffff00;
			m_returnColor = 0xffffff;
		}
		else if (m_pSelect->NowSelect == m_pSelect->Ten)
		{
			m_brightnessColor = 0xffffff;
			m_bgmColor = 0xffffff;
			m_seColor = 0xffffff;
			m_cameraColor = 0xffffff;
			m_returnColor = 0xffff00;
		}
	}
	//明るさを選択
	if (m_brightness == true)
	{
		SettingBarChange(m_pSelect->NowSelect, m_brightBar, cBrightnessBar);

	}

	//音量を選択
	if (m_volume == true)
	{
		SettingBarChange(m_pSelect->NowSelect, m_volumeBar, cVolumeBar);

	}

	//SEを選択
	if (m_se == true)
	{
		SettingBarChange(m_pSelect->NowSelect, m_seBar, cSeBar);

	}

	//カメラ感度を選択
	if (m_camera == true)
	{
		SettingBarChange(m_pSelect->NowSelect, m_cameraBar, cCameraBar);

	}

	DrawStringToHandle(100, 50, "設定", 0xffffff, m_pBigFont->GetHandle());

	DrawStringToHandle(100, 180, "明るさ設定", m_brightnessColor, m_pFont->GetHandle());
	DrawStringToHandle(100, 360, "BGM設定", m_bgmColor, m_pFont->GetHandle());
	DrawStringToHandle(100, 540, "SE設定", m_seColor, m_pFont->GetHandle());
	DrawStringToHandle(100, 720, "カメラ感度", m_cameraColor, m_pFont->GetHandle());
	DrawStringToHandle(100, 900, "戻る", m_returnColor, m_pFont->GetHandle());

	SettingBarDraw(m_brightBar, 500, 200);
	SettingBarDraw(m_volumeBar, 500, 380);
	SettingBarDraw(m_seBar, 500, 560);
	SettingBarDraw(m_cameraBar, 500, 740);
	
	m_pSelect->Draw();
}

/// <summary>
/// 設定の描画処理
/// </summary>
/// <param name="volume">音量</param>
void Setting::SettingDraw(SEManager& se)
{
	se.Update(m_seSize);

	//画面を暗くする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_blackPal);
	DrawGraph(0, 0, m_black, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//画面を明るくする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_whitePal);
	DrawGraph(0, 0, m_white, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//メニュー描画
void Setting::MenuDraw(int rb, int lb, int box)
{
	int x1 = 0;
	int y1 = 45;
	int x2 = 0;
	int y2 = 155;
	const char* menu = "";

	//アルファ値をいじる
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


	if (m_pSelect->NowSelect == m_pSelect->Seven)
	{
		x1 = 970;
		x2 = 1085;

		menu = "装備とアイテム";
	}
	if (m_pSelect->NowSelect == m_pSelect->Eight)
	{
		x1 = 1100;
		x2 = 1215;

		menu = "設定変更";
	}
	if (m_pSelect->NowSelect == m_pSelect->Nine)
	{
		x1 = 1235;
		x2 = 1350;

		menu = "戻る";
	}
	if (m_pSelect->NowSelect == m_pSelect->Ten)
	{
		x1 = 1365;
		x2 = 1480;

		menu = "タイトルに戻る";
	}
	
	
	DrawRotaGraph(900, 100, 0.5f, 0.0f, lb, true);
	DrawGraph(935, 0, m_menu, true);
	DrawRotaGraph(1550, 100, 0.5f, 0.0f, rb, true);
	DrawRotaGraph(1225, 200, 0.8f, 0.0f, box, true);

	DrawStringToHandle(1130, 190, menu, 0xffffff, m_pSmallFont->GetHandle());

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cBlenda);
	DrawBox(x1, y1, x2, y2, 0x000fff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//背景を暗くする
void Setting::MenuBackDraw()
{
	//画面を暗くする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawGraph(0, 0, m_black, false);
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
void Setting::EquipmentDraw(Player& player)
{
	DrawGraph(0, 0, m_equipment, true);

	if (m_pSelect->NowSelect == m_pSelect->Eight)
	{
		m_equipmentColorPos.oneX = cRightEquipmentOneX;
		m_equipmentColorPos.oneY = cRightEquipmentOneY;
		m_equipmentColorPos.secondX = cRightEquipmentSecondX;
		m_equipmentColorPos.secondY = cRightEquipmentSecondY;
	}
	else if (m_pSelect->NowSelect == m_pSelect->Nine)
	{
		m_equipmentColorPos.oneX = cLeftEquipmentOneX;
		m_equipmentColorPos.oneY = cLeftEquipmentOneY;
		m_equipmentColorPos.secondX = cLeftEquipmentSecondX;
		m_equipmentColorPos.secondY = cLeftEquipmentSecondY;
	}
	else if (m_pSelect->NowSelect == m_pSelect->Ten)
	{
		m_equipmentColorPos.oneX = cArmorEquipmentOneX;
		m_equipmentColorPos.oneY = cArmorEquipmentOneY;
		m_equipmentColorPos.secondX = cArmorEquipmentSecondX;
		m_equipmentColorPos.secondY = cArmorEquipmentSecondY;
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawBox(m_equipmentColorPos.oneX, m_equipmentColorPos.oneY, m_equipmentColorPos.secondX, m_equipmentColorPos.secondY, 0x000fff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	ms_levelUP.sl_all = player.GetLevel();

	ms_levelUP.sl_hp = player.GetHPLevel();
	ms_levelUP.sl_stamina = player.GetStaminaLevel();
	ms_levelUP.sl_muscle = player.GetMuscleLevel();
	ms_levelUP.sl_skill = player.GetSkillLevel();

	DrawFormatStringToHandle(1100, 400, 0xffffff, m_pFont->GetHandle(), "レベル　　　%d", ms_levelUP.sl_all);

	DrawFormatStringToHandle(1150, 515, 0xffffff, m_pFont->GetHandle(), "%d", ms_levelUP.sl_hp);
	DrawFormatStringToHandle(1150, 600, 0xffffff, m_pFont->GetHandle(), "%d", ms_levelUP.sl_stamina);
	DrawFormatStringToHandle(1150, 680, 0xffffff, m_pFont->GetHandle(), "%d", ms_levelUP.sl_muscle);
	DrawFormatStringToHandle(1150, 760, 0xffffff, m_pFont->GetHandle(), "%d", ms_levelUP.sl_skill);


	DrawStringToHandle(1300, 515, "生命力", 0xffffff, m_pFont->GetHandle());
	DrawFormatStringToHandle(1300, 565, 0xffffff, m_pFont->GetHandle(), "%d", static_cast<int>(player.GetStatus().s_hp));
	DrawStringToHandle(1300, 615, "体力", 0xffffff, m_pFont->GetHandle());
	DrawFormatStringToHandle(1300, 665, 0xffffff, m_pFont->GetHandle(), "%d", static_cast<int>(player.GetStatus().s_stamina));
	DrawStringToHandle(1300, 715, "攻撃力", 0xffffff, m_pFont->GetHandle());
	DrawFormatStringToHandle(1300, 765, 0xffffff, m_pFont->GetHandle(), "%d", static_cast<int>(player.GetAttackDamage() + ((ms_levelUP.sl_muscle - 1) * player.GetAttackMuscle()) + ((ms_levelUP.sl_skill - 1) * player.GetAttackSkill())));

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
		if (m_pSelect->NowSelect == m_pSelect->Eight)
		{
			m_selectY = 255;

			m_menuColor[0] = 0x000000;
			m_menuColor[1] = 0xffffff;
			m_menuColor[2] = 0xffffff;
		}
		else if (m_pSelect->NowSelect == m_pSelect->Nine)
		{
			m_selectY = 355;

			m_menuColor[0] = 0xffffff;
			m_menuColor[1] = 0x000000;
			m_menuColor[2] = 0xffffff;
		}
		else if (m_pSelect->NowSelect == m_pSelect->Ten)
		{
			m_selectY = 455;

			m_menuColor[0] = 0xffffff;
			m_menuColor[1] = 0xffffff;
			m_menuColor[2] = 0x000000;
		}

		//フォントのサイズ変更
		SetFontSize(60);

		DrawString(150, 50, "棺桶", 0xffffff);

		//フォントのサイズ変更
		SetFontSize(40);

		DrawString(150, 300, "休息", m_menuColor[0]);
		DrawString(150, 400, "魂器へ転送", m_menuColor[1]);
		DrawString(150, 500, "立ち去る", m_menuColor[2]);
	}
	//レベル上げられる休息
	else
	{
		if (m_pSelect->NowSelect == m_pSelect->Seven)
		{
			m_selectY = 255;

			m_menuColor[0] = 0x000000;
			m_menuColor[1] = 0xffffff;
			m_menuColor[2] = 0xffffff;
			m_menuColor[3] = 0xffffff;
		}
		else if (m_pSelect->NowSelect == m_pSelect->Eight)
		{
			m_selectY = 355;

			m_menuColor[0] = 0xffffff;
			m_menuColor[1] = 0x000000;
			m_menuColor[2] = 0xffffff;
			m_menuColor[3] = 0xffffff;
		}
		else if (m_pSelect->NowSelect == m_pSelect->Nine)
		{
			m_selectY = 455;

			m_menuColor[0] = 0xffffff;
			m_menuColor[1] = 0xffffff;
			m_menuColor[2] = 0x000000;
			m_menuColor[3] = 0xffffff;

		}
		else if (m_pSelect->NowSelect == m_pSelect->Ten)
		{
			m_selectY = 555;

			m_menuColor[0] = 0xffffff;
			m_menuColor[1] = 0xffffff;
			m_menuColor[2] = 0xffffff;
			m_menuColor[3] = 0x000000;
		}

		//フォントのサイズ変更
		SetFontSize(60);

		DrawString(150, 50, "魂器", 0xffffff);

		//フォントのサイズ変更
		SetFontSize(40);

		DrawString(150, 300, "休息", m_menuColor[0]);
		DrawString(150, 400, "レベル上げ", m_menuColor[1]);
		DrawString(150, 500, "魂器へ転送", m_menuColor[2]);
		DrawString(150, 600, "立ち去る", m_menuColor[3]);
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

	if (m_pSelect->NowSelect == m_pSelect->Six)
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
	else if (m_pSelect->NowSelect == m_pSelect->Seven)
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
	else if (m_pSelect->NowSelect == m_pSelect->Eight)
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
	else if (m_pSelect->NowSelect == m_pSelect->Nine)
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
	else if (m_pSelect->NowSelect == m_pSelect->Ten)
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
	int Right = 0;
	int Left = 0;
	int Armor = 0;

	DrawGraph(0, 0, m_selectEquipment, true);

	//右装備だった場合
	if (m_select.right)
	{
		//アイテム取得順
		for (const auto& itemName : item.GetItemOrder())
		{
			if (itemName == "BlackSword")
			{
				Right++;
			}
			else if (itemName == "Bat")
			{
				Right++;
			}
		}

		if (Right == 0)
		{
			if (m_pSelect->NowSelect == m_pSelect->Ten)
			{
				m_selectObject.oneX = cOneX;
				m_selectObject.oneY = cOneY;
				m_selectObject.secondX = cSecondX;
				m_selectObject.secondY = cSecondY;

				//何を選択しているかの変数
				m_right = 0;
			}
		}
		else if (Right == 1)
		{
			if (m_pSelect->NowSelect == m_pSelect->Nine)
			{
				m_selectObject.oneX = cOneX;
				m_selectObject.oneY = cOneY;
				m_selectObject.secondX = cSecondX;
				m_selectObject.secondY = cSecondY;

				//何を選択しているかの変数
				m_right = 0;
			}
			else if (m_pSelect->NowSelect == m_pSelect->Ten)
			{
				m_selectObject.oneY = cOneY + cDifferenceY;
				m_selectObject.secondY = cSecondY + cDifferenceY;

				//何を選択しているかの変数
				m_right = 1;
			}
		}
		else if (Right == 2)
		{
			if (m_pSelect->NowSelect == m_pSelect->Eight)
			{
				m_selectObject.oneX = cOneX;
				m_selectObject.oneY = cOneY;
				m_selectObject.secondX = cSecondX;
				m_selectObject.secondY = cSecondY;

				//何を選択しているかの変数
				m_right = 0;
			}
			else if (m_pSelect->NowSelect == m_pSelect->Nine)
			{
				m_selectObject.oneY = cOneY + cDifferenceY;
				m_selectObject.secondY = cSecondY + cDifferenceY;

				//何を選択しているかの変数
				m_right = 1;
			}
			else if (m_pSelect->NowSelect == m_pSelect->Ten)
			{
				m_selectObject.oneY = cOneY + (cDifferenceY * 2);
				m_selectObject.secondY = cSecondY + (cDifferenceY * 2);

				//何を選択しているかの変数
				m_right = 2;
			}
		}

		//武器の説明描画
		WeaponDraw(m_weaponList, m_right);
	}
	//左装備だった場合
	else if (m_select.left)
	{
		//アイテム取得順
		for (const auto& itemName : item.GetItemOrder())
		{
			if (itemName == "Distorted")
			{
				Left++;
			}
			else if (itemName == "WoodShield")
			{
				Left++;
			}
		}

		if (Left == 0)
		{
			if (m_pSelect->NowSelect == m_pSelect->Ten)
			{
				m_selectObject.oneX = cOneX;
				m_selectObject.oneY = cOneY;
				m_selectObject.secondX = cSecondX;
				m_selectObject.secondY = cSecondY;

				//何を選択しているかの変数
				m_left = 0;
			}
		}
		else if (Left == 1)
		{
			if (m_pSelect->NowSelect == m_pSelect->Nine)
			{
				m_selectObject.oneX = cOneX;
				m_selectObject.oneY = cOneY;
				m_selectObject.secondX = cSecondX;
				m_selectObject.secondY = cSecondY;

				//何を選択しているかの変数
				m_left = 0;
			}
			else if (m_pSelect->NowSelect == m_pSelect->Ten)
			{
				m_selectObject.oneY = cOneY + cDifferenceY;
				m_selectObject.secondY = cSecondY + cDifferenceY;

				//何を選択しているかの変数
				m_left = 1;
			}
		}
		else if (Left == 2)
		{
			if (m_pSelect->NowSelect == m_pSelect->Eight)
			{
				m_selectObject.oneX = cOneX;
				m_selectObject.oneY = cOneY;
				m_selectObject.secondX = cSecondX;
				m_selectObject.secondY = cSecondY;

				//何を選択しているかの変数
				m_left = 0;
			}
			else if (m_pSelect->NowSelect == m_pSelect->Nine)
			{
				m_selectObject.oneY = cOneY + cDifferenceY;
				m_selectObject.secondY = cSecondY + cDifferenceY;

				//何を選択しているかの変数
				m_left = 1;
			}
			else if (m_pSelect->NowSelect == m_pSelect->Ten)
			{
				m_selectObject.oneY = cOneY + (cDifferenceY * 2);
				m_selectObject.secondY = cSecondY + (cDifferenceY * 2);

				//何を選択しているかの変数
				m_left = 2;
			}
		}

		//盾の説明描画
		ShieldDraw(m_shieldList, m_left);
	}
	//防具だった場合
	else if (m_select.armor)
	{
		//アイテム取得順
		for (const auto& itemName : item.GetItemOrder())
		{
			if (itemName == "ArmorNormal")
			{
				Armor++;
			}
		}

		if (Armor == 0)
		{
			if (m_pSelect->NowSelect == m_pSelect->Ten)
			{
				m_selectObject.oneX = cOneX;
				m_selectObject.oneY = cOneY;
				m_selectObject.secondX = cSecondX;
				m_selectObject.secondY = cSecondY;

				//何を選択しているか判断するための変数
				m_armor = 0;
			}
		}
		else if (Armor == 1)
		{
			if (m_pSelect->NowSelect == m_pSelect->Nine)
			{
				m_selectObject.oneX = cOneX;
				m_selectObject.oneY = cOneY;
				m_selectObject.secondX = cSecondX;
				m_selectObject.secondY = cSecondY;

				//何を選択しているか判断するための変数
				m_armor = 0;
			}
			else if (m_pSelect->NowSelect == m_pSelect->Ten)
			{
				m_selectObject.oneY = cOneY + cDifferenceY;
				m_selectObject.secondY = cSecondY + cDifferenceY;

				//何を選択しているか判断するための変数
				m_armor = 1;
			}
		}

		//防具の説明描画
		ArmorDraw(m_armorList, m_armor);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawBox(m_selectObject.oneX, m_selectObject.oneY, m_selectObject.secondX, m_selectObject.secondY, 0x000fff, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// ワープできない時の描画処理
/// </summary>
void Setting::CaveatDraw()
{
	if (m_caveat)
	{
		cTime++;
		//時間中注意書きを表示する
		if (cTime <= cTimeMax)
		{
			DrawGraph(600, 200, m_rest, true);

			DrawStringToHandle(720, 350, "転送できない", 0xffffff, m_pFont->GetHandle());
		}
		else
		{
			m_caveat = false;
		}
	}
	else
	{
		//初期化する
		cTime = 0;
	}
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
}

//設定関数
void Setting::SettingChange(int& setting, int& cSetting, int one, int two, int three, int four, int five, int six, int seven, int eight, int nine, int ten)
{
	if (m_pSelect->NowSelect == 0)
	{
		setting = one;
		cSetting = one;
	}
	else if (m_pSelect->NowSelect == 1)
	{
		setting = two;
		cSetting = two;
	}
	else if (m_pSelect->NowSelect == 2)
	{
		setting = three;
		cSetting = three;
	}
	else if (m_pSelect->NowSelect == 3)
	{
		setting = four;
		cSetting = four;
	}
	else if (m_pSelect->NowSelect == 4)
	{
		setting = five;
		cSetting = five;
	}
	else if (m_pSelect->NowSelect == 5)
	{
		setting = six;
		cSetting = six;
	}
	else if (m_pSelect->NowSelect == 6)
	{
		setting = seven;
		cSetting = seven;
	}
	else if (m_pSelect->NowSelect == 7)
	{
		setting = eight;
		cSetting = eight;
	}
	else if (m_pSelect->NowSelect == 8)
	{
		setting = nine;
		cSetting = nine;
	}
	else if (m_pSelect->NowSelect == 9)
	{
		setting = ten;
		cSetting = ten;
	}
}

//カメラ感度の設定
void Setting::CameraChange(float& setting, float& cSetting, float one, float two, float three, float four, float five, float six, float seven, float eight, float nine, float ten)
{
	if (m_pSelect->NowSelect == 0)
	{
		setting = one;
		cSetting = one;
	}
	else if (m_pSelect->NowSelect == 1)
	{
		setting = two;
		cSetting = two;
	}
	else if (m_pSelect->NowSelect == 2)
	{
		setting = three;
		cSetting = three;
	}
	else if (m_pSelect->NowSelect == 3)
	{
		setting = four;
		cSetting = four;
	}
	else if (m_pSelect->NowSelect == 4)
	{
		setting = five;
		cSetting = five;
	}
	else if (m_pSelect->NowSelect == 5)
	{
		setting = six;
		cSetting = six;
	}
	else if (m_pSelect->NowSelect == 6)
	{
		setting = seven;
		cSetting = seven;
	}
	else if (m_pSelect->NowSelect == 7)
	{
		setting = eight;
		cSetting = eight;
	}
	else if (m_pSelect->NowSelect == 8)
	{
		setting = nine;
		cSetting = nine;
	}
	else if (m_pSelect->NowSelect == 9)
	{
		setting = ten;
		cSetting = ten;
	}
}

//設定のバーの描画処理
void Setting::SettingBarChange(int Decision, float& bar, float& cBar)
{
	if (select)
	{
		if (Decision == 0)
		{
			bar = 0.1f;
			cBar = 0.1f;
		}
		else if (Decision == 1)
		{
			bar = 0.2f;
			cBar = 0.2f;

		}
		else if (Decision == 2)
		{
			bar = 0.3f;
			cBar = 0.3f;

		}
		else if (Decision == 3)
		{
			bar = 0.4f;
			cBar = 0.4f;

		}
		else if (Decision == 4)
		{
			bar = 0.5f;
			cBar = 0.5f;

		}
		else if (Decision == 5)
		{
			bar = 0.6f;
			cBar = 0.6f;

		}
		else if (Decision == 6)
		{
			bar = 0.7f;
			cBar = 0.7f;

		}
		else if (Decision == 7)
		{
			bar = 0.8f;
			cBar = 0.8f;

		}
		else if (Decision == 8)
		{
			bar = 0.9f;
			cBar = 0.9f;

		}
		else if (Decision == 9)
		{
			bar = 1.0f;
			cBar = 1.0f;

		}
	}

	
}

void Setting::SettingBarDraw(float bar, int x, int y)
{
	DrawBox(x, y, x + static_cast<int>(cBarWidth * bar), y + cBarHeight, 0x00ff00, TRUE); // プログレスバー
	DrawBox(x, y, x + cBarWidth, y + cBarHeight, 0xffffff, false);
}

void Setting::WeaponUpdate(std::list<std::string> list, Weapon& weapon, int right)
{
	int select = 1;

	//素手選択
	if (right == 0)
	{
		weapon.End();

		weapon.SetFist(true);
		weapon.SetBlack(false);
		weapon.SetBat(false);

		weapon.Init();

		m_pSelect->NowSelect = 7;
	}
	//武器選択
	else if (right >= 1)
	{
		for (const auto& item : list)
		{
			if (right == select)
			{
				//黒い剣選択
				if (item == "BlackSword")
				{
					weapon.End();

					weapon.SetBlack(true);
					weapon.SetFist(false);
					weapon.SetBat(false);

					weapon.Init();

					m_pSelect->NowSelect = 7;
				}
				//木のバット選択
				else if (item == "Bat")
				{
					weapon.End();

					weapon.SetBat(true);
					weapon.SetBlack(false);
					weapon.SetFist(false);

					weapon.Init();

					m_pSelect->NowSelect = 7;
				}
			}

			select++;
		}
	}
}

/// <summary>
/// 武器の説明分描画
/// </summary>
/// <param name="list"></param>
/// <param name="right"></param>
void Setting::WeaponDraw(std::list<std::string> list, int right)
{
	int select = 1;

	//素手選択
	if (right == 0)
	{
		DrawStringToHandle(400, 100, "素手", 0xffffff, m_pBigFont->GetHandle());

		DrawStringToHandle(400, 300, "信じるべきは己の力のみ", 0xffffff, m_pFont->GetHandle());
	}
	//武器選択
	else if (right >= 1)
	{
		for (const auto& item : list)
		{
			if (right == select)
			{
				//黒い剣選択
				if (item == "BlackSword")
				{
					DrawStringToHandle(400, 100, "黒剣", 0xffffff, m_pBigFont->GetHandle());

					DrawStringToHandle(400, 300, "その漆黒の刃は、まるで夜そのものを\n鍛え上げたかのように光を吸い込み、\n沈黙の中に冷たい輝きを宿す", 0xffffff, m_pFont->GetHandle());
					DrawStringToHandle(400, 450, "この剣を振るう者は、\n刃に秘められた深淵を覗き込むこととなる\n迷いなき者には忠実な刃となり、\n心揺らぐ者には破滅の影を落とす", 0xffffff, m_pFont->GetHandle());
				}
				//木のバット選択
				if (item == "Bat")
				{
					DrawStringToHandle(400, 100, "木の棍棒", 0xffffff, m_pBigFont->GetHandle());

					DrawStringToHandle(400, 300, "人形の一部を削り出した棍棒\n幾千もの戦いを耐え抜いた木肌は\n鋼にも劣らぬ強度を誇る", 0xffffff, m_pFont->GetHandle());
					DrawStringToHandle(400, 450, "かつての人形の記憶を宿し、\n持ち主の手に馴染むその感触は、\n不思議な安心感すら与える", 0xffffff, m_pFont->GetHandle());

				}

			}

			select++;
		}
	}
}

void Setting::ShieldUpdate(std::list<std::string> list, Shield& shield, int left)
{
	int select = 1;

	//素手選択
	if (left == 0)
	{
		shield.End();

		shield.SetFist(true);
		shield.SetUgly(false);
		shield.SetWood(false);

		shield.Init();

		m_pSelect->NowSelect = 8;
	}
	//盾選択
	else if (left >= 1)
	{
		for (const auto& item : list)
		{
			if (left == select)
			{
				//醜い盾選択
				if (item == "Distorted")
				{
					shield.End();

					shield.SetUgly(true);
					shield.SetFist(false);
					shield.SetWood(false);

					shield.Init();

					m_pSelect->NowSelect = 8;

				}
				//木の盾選択
				else if (item == "WoodShield")
				{
					shield.End();

					shield.SetWood(true);
					shield.SetUgly(false);
					shield.SetFist(false);

					shield.Init();

					m_pSelect->NowSelect = 8;

				}
			}

			select++;
		}
	}
}

/// <summary>
/// 盾の説明描画
/// </summary>
/// <param name="list"></param>
/// <param name="left"></param>
void Setting::ShieldDraw(std::list<std::string> list, int left)
{
	int select = 1;

	//素手選択
	if (left == 0)
	{
		DrawStringToHandle(400, 100, "素手", 0xffffff, m_pBigFont->GetHandle());

		DrawStringToHandle(400, 300, "信じるべきは己の力のみ", 0xffffff, m_pFont->GetHandle());
	}
	//盾選択
	else if (left >= 1)
	{
		for (const auto& item : list)
		{
			if (left == select)
			{
				//歪んだ盾選択
				if (item == "Distorted")
				{
					DrawStringToHandle(400, 100, "忌盾", 0xffffff, m_pBigFont->GetHandle());

					DrawStringToHandle(400, 300, "神への反逆を試みた者が捧げた、呪われし盾\n触れる者の心に微かな囁きを落とすという\n刃も魔も弾くが、持ち主の魂を蝕む", 0xffffff, m_pFont->GetHandle());
					DrawStringToHandle(400, 450, "選ぶのなら覚悟せよこの盾は、\n持つ者を守るのではなく、試すのだから", 0xffffff, m_pFont->GetHandle());

				}
				//木の盾選択
				if (item == "WoodShield")
				{
					DrawStringToHandle(400, 100, "木の盾", 0xffffff, m_pBigFont->GetHandle());

					DrawStringToHandle(400, 300, "人形の一部を削り出した盾\nその木目にはかつての嵐と戦火を\n耐え抜いた誇りが刻まれている", 0xffffff, m_pFont->GetHandle());
					DrawStringToHandle(400, 450, "剣をも弾く堅牢さと、\nしなやかに衝撃を受け流す柔軟さを兼ね備え、\n持ち主を静かに、しかし確かに守り続ける", 0xffffff, m_pFont->GetHandle());

				}

			}

			select++;
		}
	}
}

void Setting::ArmorUpdate(std::list<std::string> list, Armor& armor, int body)
{
	int select = 1;

	//裸体選択
	if (body == 0)
	{
		armor.SetBody(true);
		armor.SetCommon(false);

		armor.Init();

		m_pSelect->NowSelect = 9;
	}
	//防具選択
	else if (body >= 1)
	{
		for (const auto& item : list)
		{
			if (body == select)
			{
				//普通の鎧選択
				if (item == "ArmorNormal")
				{
					armor.SetCommon(true);
					armor.SetBody(false);

					armor.Init();

					m_pSelect->NowSelect = 9;

				}
			}

			select++;
		}
	}
}

/// <summary>
/// 防具の説明描画
/// </summary>
/// <param name="list"></param>
/// <param name="body"></param>
void Setting::ArmorDraw(std::list<std::string> list, int body)
{
	int select = 1;

	//裸体選択
	if (body == 0)
	{
		DrawStringToHandle(400, 100, "裸体", 0xffffff, m_pBigFont->GetHandle());

		DrawStringToHandle(400, 300, "信じるべきは己の力のみ", 0xffffff, m_pFont->GetHandle());
	}
	//防具選択
	else if (body >= 1)
	{
		for (const auto& item : list)
		{
			if (body == select)
			{
				//歪んだ盾選択
				if (item == "ArmorNormal")
				{
					DrawStringToHandle(400, 100, "ノクターニス兵の鎧", 0xffffff, m_pBigFont->GetHandle());

					DrawStringToHandle(400, 300, "ノクターニス城を守護する兵たちの鎧\n絶望を吸い込み、静寂を纏う", 0xffffff, m_pFont->GetHandle());
					DrawStringToHandle(400, 400, "鍛え上げられた鋼は剣を弾き、魔を拒む\n夜が終わるまで戦い続ける\nそれがノクターニス兵の宿命である", 0xffffff, m_pFont->GetHandle());

				}

			}

			select++;
		}
	}
}
