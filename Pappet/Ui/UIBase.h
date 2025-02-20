#pragma once
#include "DxLib.h"
#include "Manager/SEManager.h"
#include<memory>

class ItemManager;

/// <summary>
/// UI関連の画像などを読み込むためのクラス
/// </summary>
class UIBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	UIBase();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~UIBase();

	/// <summary>
	/// 画像のサイズを変えてロードする関数
	/// </summary>
	/// <param name="FileName">画像のFileName</param>
	/// <param name="XSize">画像の横幅を割る数</param>
	/// <param name="YSize">画像の縦幅を割る数</param>
	/// <returns>サイズを変えた画像を返す</returns>
	int MyLoadGraph(const char* FileName, int XSize, int YSize);

protected:
	int m_heelStone;      //回復石の画像格納変数
	int m_eqFist;         //装備の拳画像格納変数
	int m_sword;          //剣の画像格納変数
	int m_eqSword;        //装備の剣画像格納変数
	int m_eqShield;       //装備の盾画像格納変数

	//UI画面
	int m_equipment;      //装備画面格納変数
	int m_itemBox;        //アイテム画面格納変数
	int m_selectEquipment;//装備選択画面格納変数
	int m_rest;           //休息選択画面格納変数
	int m_levelUp;        //レベルアップ画面格納変数
	int m_backRightBar;   //バーの右端の画像格納変数
	int m_backLeftBar;    //バーの左端の画像格納変数
	int m_backCenterBar;  //バーの中央の画像格納変数
	int m_backOutBar;     //バーの外枠画像格納変数   
	int m_hpBar;          //HPバー端の画像格納変数
	int m_staminaBar;     //スタミナバーの画像格納変数
	int m_coreBackBar;    //コアバーの背景画像格納変数
	int m_statusIcon;     //ステータスアイコン画像格納変数
	int m_equipmentFrame; //装備の枠画像格納変数

	//アイテムの画像
	int m_blackSword;     //黒い剣の画像格納変数
	int m_fist;           //拳の画像格納変数
	int m_body;           //裸体の画像格納変数
	int m_uglyShield;     //醜い盾の画像格納変数
	int m_commonArmor;    //平凡な鎧画像格納変数

	//UIの画像
	int m_dead;           //死亡時の画像格納変数
	int m_selectUi;       //選択時の画像格納変数
	int m_actionUI;       //アクションを格納する変数
	int m_itemTaking;     //アイテムを取得した変数
	int m_yButton;        //Yボタン画像格納変数
	int m_bButton;        //Bボタン画像格納変数
	int m_messageUI;      //メッセージ画像格納変数

	//スマートポインタ
	std::shared_ptr<SEManager> pse = std::make_shared<SEManager>();

	ItemManager* m_pItem;
};




