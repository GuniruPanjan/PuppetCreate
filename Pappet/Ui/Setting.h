#pragma once
#include "Manager/SEManager.h"
#include "DxLib.h"
#include "Ui/UIBase.h"
#include<memory>

class SelectManager;
class Shield;
class Weapon;
class Armor;
class Player;
class CoreManager;

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
	void Update();

	/// <summary>
	/// メニューの更新処理
	/// </summary>
	void MenuUpdate(Player& player);

	/// <summary>
	/// ステータス画面を変える更新処理
	/// </summary>
	void MenuChange();

	/// <summary>
	/// 装備の更新処理
	/// </summary>
	void EquipmentUpdate();

	/// <summary>
	/// 休息の更新処理
	/// </summary>
	void RestUpdate(Player& player, CoreManager& core);

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
	void BrightColorDraw(int select, int now, int other1, int other2, int other3, int other4, int black, int white);

	// <summary>
	/// 選択中の色を変える
	/// </summary>
	/// <param name="select">列挙型</param>
	/// <param name="now">選択してるもの</param>
	/// <param name="other1">それ以外１</param>
	/// <param name="other2">それ以外２</param>
	/// <param name="other3">それ以外３</param>
	/// <param name="other4">それ以外４</param>
	/// <param name="volume">音量</param>
	void VolumeColorDraw(int select, int now, int other1, int other2, int other3, int other4, int volume);

	/// <summary>
	/// 設定の描画
	/// </summary>
	/// <param name="volume">音量</param>
	void SettingDraw(int volume);

	/// <summary>
	/// メニュー描画
	/// </summary>
	void MenuDraw();

	/// <summary>
	/// 背景描画
	/// </summary>
	void MenuBackDraw();

	/// <summary>
	/// ステータス画面を変える描画処理
	/// </summary>
	void MenuChangeDraw();

	/// <summary>
	/// 装備の描画処理
	/// </summary>
	void EquipmentDraw();

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
	void ItemBoxDraw();

	/// <summary>
	/// 装備選択画面描画処理
	/// </summary>
	void EquipmentDecisionDraw(ItemManager& item);

	/// <summary>
	/// 終了処理
	/// </summary>
	void End();

	/// <summary>
	/// 音量をとる
	/// </summary>
	/// <returns>音量を返す</returns>
	int GetVolume() { return m_volumeSize; }

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

private:
	int m_black;              //黒い画像格納変数
	int m_white;              //白い画像格納変数
	int m_back;               //黒い画像格納変数
	XINPUT_STATE m_xpad;      //パッド入力
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
	int m_returnColor;        //戻るの色
	int m_brightColor[5];     //明るさ選択の色
	int m_volumeColor[5];     //音量選択の色
	int m_menuColor[5];       //メニューの色
	int m_core;               //表記用にコアを取得する
	bool m_one;               //単発入力
	bool m_settingScene;      //設定するための画面を呼ぶ変数
	bool m_brightness;        //明るさ設定
	bool m_volume;            //音量設定
	int m_volumeSize;         //音量
	bool m_equipmentMenu;     //装備メニュー
	bool m_itemMenu;          //アイテムボックスメニュー
	bool m_decisionEquipment; //装備選択画面
	bool m_returnMenu;        //戻るメニュー
	bool m_titleMenu;         //タイトルメニュー
	bool m_statusLevel;       //ステータスを開く判定
	bool m_blend;             //ブレンド判定
	bool m_up;                //レベルが上げられるかの判定

	//選んだ装備
	SelectEquipment m_select;

	//選んでいる色を変える
	SelectXY m_change;
	SelectXY m_equipmentColorPos;
	SelectXY m_selectObject;

	//スマートポインタ
	std::shared_ptr<SEManager> pse = std::make_shared<SEManager>();
	std::shared_ptr<SelectManager> pselect = std::make_shared<SelectManager>();
};




