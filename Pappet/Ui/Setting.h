#pragma once
#include "Manager/SEManager.h"
#include "DxLib.h"
#include "Ui/UIBase.h"
#include <memory>
#include <map>
#include <string>
#include <List>

class SelectManager;
class Shield;
class Weapon;
class Armor;
class Player;
class CoreManager;
class Font;

/// <summary>
/// 設定処理用のクラス
/// </summary>
class Setting : public UIBase
{
public:

	struct SelectXY
	{
		int oneX;
		int oneY;
		int secondX;
		int secondY;
	};

	struct SelectEquipment
	{
		bool right = false;
		bool left = false;
		bool armor = false;
	};

	//レベルを上げた結果格納する構造体
	struct LevelUp
	{
		int sl_all;
		int sl_hp;
		int sl_stamina;
		int sl_muscle;
		int sl_skill;
	}ms_levelUP;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Setting();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Setting();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(SEManager& se);

	/// <summary>
	/// メニューの更新処理
	/// </summary>
	void MenuUpdate(Player& player, SEManager& se);

	/// <summary>
	/// ステータス画面を変える更新処理
	/// </summary>
	//void MenuChange();

	/// <summary>
	/// 装備の更新処理
	/// </summary>
	void EquipmentUpdate();

	/// <summary>
	/// 休息の更新処理
	/// </summary>
	void RestUpdate(Player& player, CoreManager& core, bool rest, SEManager& se);

	/// <summary>
	/// レベルアップ処理
	/// </summary>
	void LevelUpdate(Player& player, CoreManager& core);

	/// <summary>
	/// レベルアップ処理2
	/// </summary>
	void LevelUp(CoreManager& core, int origin, int& level, int now);

	/// <summary>
	/// アイテムボックス更新処理
	/// </summary>
	void ItemBoxUpdate();

	/// <summary>
	/// 装備選択画面更新処理
	/// </summary>
	void EquipmentDecisionUpdate(Weapon& weapon, Shield& shield, Armor& armor, ItemManager& item);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 設定の描画
	/// </summary>
	/// <param name="volume">音量</param>
	void SettingDraw(SEManager& se);

	/// <summary>
	/// メニュー描画
	/// </summary>
	void MenuDraw(int rb, int lb, int box);

	//背景を暗くする
	void MenuBackDraw();

	/// <summary>
	/// ステータス画面を変える描画処理
	/// </summary>
	//void MenuChangeDraw();

	/// <summary>
	/// 装備の描画処理
	/// </summary>
	void EquipmentDraw(Player& player);

	/// <summary>
	/// 休息の描画処理
	/// </summary>
	void RestDraw(bool rest);

	/// <summary>
	/// レベルアップ描画
	/// </summary>
	void LevelUpDraw(Player& player, CoreManager& core);

	/// <summary>
	/// アイテムボックス描画処理
	/// </summary>
	//void ItemBoxDraw();

	/// <summary>
	/// 装備選択画面描画処理
	/// </summary>
	void EquipmentDecisionDraw(ItemManager& item);

	/// <summary>
	/// ワープできない時の描画処理
	/// </summary>
	void CaveatDraw();

	/// <summary>
	/// 終了処理
	/// </summary>
	void End();

	/// <summary>
	/// 音量をとる
	/// </summary>
	/// <returns>音量を返す</returns>
	int GetVolume() { return m_volumeSize; }

	//SEを取る
	int GetSe() { return m_seSize; }

	//カメラ感度を取る
	float GetCamera() { return m_cameraSize; }


	/// <summary>
	/// 設定シーンを出すための判定をとる
	/// </summary>
	/// <returns>判定を返す</returns>
	bool GetSettingScene() { return m_settingScene; }

	/// <summary>
	/// 設定シーンを出すための判定を決める
	/// </summary>
	/// <param name="scene">判定をとる</param>
	/// <returns>結果を返す</returns>
	bool SetSettingScene(bool scene) { return m_settingScene = scene; }

	/// <summary>
	/// 装備を開く判定をとる
	/// </summary>
	/// <returns>判定を返す</returns>
	bool GetEquipment() { return m_equipmentMenu; }

	/// <summary>
	/// 装備を開く判定を決める
	/// </summary>
	/// <param name="re">判定をとる</param>
	/// <returns>結果を返す</returns>
	bool SetEquipment(bool re) { return m_equipmentMenu = re; }

	/// <summary>
	/// アイテム画面を開く判定をとる
	/// </summary>
	/// <returns>結果を返す</returns>
	bool GetItem() { return m_itemMenu; }

	/// <summary>
	/// アイテムを開く判定を決める
	/// </summary>
	/// <param name="re">判定を取る</param>
	/// <returns>結果を返す</returns>
	bool Setitem(bool re) { return m_itemMenu = re; }

	/// <summary>
	/// ステータスのレベルUp画面を開く判定
	/// </summary>
	bool GetLevel() { return m_statusLevel; }

	/// <summary>
	/// 装備を選択する画面遷移判定を取る
	/// </summary>
	/// <returns></returns>
	bool GetDecision() { return m_decisionEquipment; }

	/// <summary>
	/// 何の装備を開いているか
	/// </summary>
	/// <returns></returns>
	SelectEquipment GetSelect() { return m_select; }

	/// <summary>
	/// 画面に戻る判定をとる
	/// </summary>
	/// <returns>判定を返す</returns>
	bool GetReturn() { return m_returnMenu; }

	/// <summary>
	/// 画面に戻る判定を決める
	/// </summary>
	/// <param name="re">判定をとる</param>
	/// <returns>結果を返す</returns>
	bool SetReturn(bool re) { return m_returnMenu = re; }

	/// <summary>
	/// タイトルに戻る判定をとる
	/// </summary>
	/// <returns>判定をとる</returns>
	bool GetTitle() { return m_titleMenu; }

	/// <summary>
	/// 休息スタート
	/// </summary>
	/// <returns></returns>
	bool GetReset() { return m_reset; }
	bool SetReset(bool set) { return m_reset = set; }

	/// <summary>
	/// 休息地点にワープするかの判定をとる
	/// </summary>
	/// <returns></returns>
	bool GetRestWarp() { return m_restWarp; }
	bool SetRestWarp(bool set) { return m_restWarp = set; }

	//待ち時間を設定する
	int SetWait(int set) { return m_waitTime = set; }

private:

	void SettingChange(int& setting, int& cSetting, int one, int two, int three, int four, int five, int six, int seven, int eight, int nine, int ten);
	void CameraChange(float& setting, float& cSetting, float one, float two, float three, float four, float five, float six, float seven, float eight, float nine, float ten);
	void SettingBarChange(int Decision, float& bar, float& cBar);
	void SettingBarDraw(float bar, int x, int y);

	void WeaponUpdate(std::list<std::string> list, Weapon& weapon, int right);
	void WeaponDraw(std::list<std::string> list, int right);
	void ShieldUpdate(std::list<std::string> list, Shield& shield, int left);
	void ShieldDraw(std::list<std::string> list, int left);
	void ArmorUpdate(std::list<std::string> list, Armor& armor, int body);
	void ArmorDraw(std::list<std::string> list, int body);

private:

	XINPUT_STATE m_xpad;      //パッド入力

	int m_black;              //黒い画像格納変数
	int m_white;              //白い画像格納変数
	int m_back;               //黒い画像格納変数
	int m_volumeSize;         //音量
	int m_seSize;             //SE音量
	float m_cameraSize;       //カメラ感度
	int m_menuSelect[5];      //メニュー選択 
	int m_selectX;            //選択画像のX座標
	int m_selectY;            //選択画像のY座標
	int m_blackPal;           //黒い画像のアルファ値保存
	int m_whitePal;           //白い画像のアルファ値保存
	int m_button;             //押した時間を得る
	int m_thumb;              //押した時間を得る
	int m_waitTime;           //入力を待つための時間
	int m_brightnessColor;    //明るさの色
	int m_bgmColor;           //音量の色
	int m_seColor;            //SEの色
	int m_cameraColor;        //カメラの色
	int m_returnColor;        //戻るの色
	int m_menuColor[5];       //メニューの色
	int m_core;               //表記用にコアを取得する
	int m_right;              //右装備の選んでるところを格納する変数
	int m_left;               //左装備の選んでるところを格納する変数
	int m_armor;              //鎧装備の選んでるところを格納する変数

	float m_brightBar;        //明るさのバー
	float m_volumeBar;        //音量のバー
	float m_seBar;            //SEのバー
	float m_cameraBar;        //カメラのバー

	bool m_one;               //単発入力
	bool m_settingScene;      //設定するための画面を呼ぶ変数
	bool m_brightness;        //明るさ設定
	bool m_volume;            //BGM設定
	bool m_se;                //SE設定
	bool m_camera;            //カメラ感度
	bool m_equipmentMenu;     //装備メニュー
	bool m_itemMenu;          //アイテムボックスメニュー
	bool m_decisionEquipment; //装備選択画面
	bool m_returnMenu;        //戻るメニュー
	bool m_titleMenu;         //タイトルメニュー
	bool m_statusLevel;       //ステータスを開く判定
	bool m_blend;             //ブレンド判定
	bool m_up;                //レベルが上げられるかの判定
	bool m_menuDecision;      //メニューでの決定判定
	bool m_restWarp;          //休息地点をワープするための判定
	bool m_caveat;            //注意書きの表示
	bool m_reset;             //休息によるリセット


	//選んだ装備
	SelectEquipment m_select;

	//選んでいる色を変える
	SelectXY m_change;
	SelectXY m_equipmentColorPos;
	SelectXY m_selectObject;

	std::list<std::string> m_weaponList;      //武器の入手順に入れるリスト
	std::list<std::string> m_shieldList;      //盾の入手順に入れるリスト
	std::list<std::string> m_armorList;       //鎧の入手順に入れるリスト

	//スマートポインタ
	std::shared_ptr<SelectManager> m_pSelect = std::make_shared<SelectManager>();
	std::shared_ptr<Font> m_pFont;
	std::shared_ptr<Font> m_pSmallFont;
	std::shared_ptr<Font> m_pBigFont;
};




