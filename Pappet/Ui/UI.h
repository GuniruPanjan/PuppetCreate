#pragma once
#include "UIBase.h"


class Player;
class EnemyManager;
class Setting;
class MapManager;
class ItemManager;
class Weapon;
class Shield;
class Armor;
class Tool;
class MessageManager;
class Font;
class SEManager;

/// <summary>
/// UI関係のクラス
/// </summary>
class UI : public UIBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	UI();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~UI();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="player">プレイヤーの呼び出し</param>
	/// <param name="enemy">エネミーの呼び出し</param>
	void Init();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="player">プレイヤーの呼び出し</param>
	/// <param name="enemy">エネミーの呼び出し</param>
	/// <param name="eq">装備の呼び出し</param>
	/// <param name="map">マップの呼び出し</param>
	/// <param name="item">アイテムの呼び出し</param>
	void Draw(Player& player, EnemyManager& enemy, Setting& eq, MapManager& map, ItemManager& item, Weapon& weapon, Shield& shield, Armor& armor, Tool& tool, MessageManager& message);

	/// <summary>
	/// 装備の描画処理
	/// </summary>
	void EquipmentDraw(Weapon& weapon, Shield& shield, Armor& armor);

	/// <summary>
	/// 右武器描画処理
	/// </summary>
	void RightDraw(Weapon& weapon, ItemManager& item);

	/// <summary>
	/// 左武器描画処理
	/// </summary>
	void LeftDraw(Shield& shield, ItemManager& item);

	/// <summary>
	/// 防具描画処理
	/// </summary>
	void ArmorDraw(Armor& armor, ItemManager& item);



	//装備の描画
	void EquipmentUIDraw(Weapon& weapon, Shield& shield, Armor& armor, Tool& tool);

	/// <summary>
	/// アイテム取った時の描画
	/// </summary>
	/// <param name="item"></param>
	/// <param name="handle"></param>
	void ItemTakingUI(int item, int handle, int x, int y, int charX, int charY, const char* letter);

	/// <summary>
	/// 死んだときの描画
	/// </summary>
	void DiedDraw(SEManager& se);

	//ボスを倒したとき勝利演出描画
	void GetCoreDraw(SEManager& se);

	/// <summary>
	/// ステータスバーの描画処理
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="player"></param>
	void StatusDraw(int x, int y, Player& player);

	/// <summary>
	/// 敵のHP描画処理
	/// </summary>
	/// <param name="enemy"></param>
	void EnemyHPDraw(VECTOR pos, int hp, int maxHP);

	/// <summary>
	/// ボスのHP描画処理
	/// </summary>
	void BossHPDraw(int hp, int maxHP, const char* name, const char* subName);

	/// <summary>
	/// 終了処理
	/// </summary>
	void End();

	/// <summary>
	/// ロック画面変数
	/// </summary>
	int GetLockUI() { return m_tagetLock; }

	//死亡してリセットする
	bool GetReset() { return m_deadReset; }

	//勝利演出をリセットする
	bool GetWinReset() { return m_winReset; }

private:
	unsigned int m_hpColor = 0xff0000;    //HPカラー
	unsigned int m_staminaColor = 0x00ff00;       //スタミナカラー

	//死亡の画像透過アルファ値
	int m_youDead;
	//勝利時の画像透過アルファ値
	int m_youWin;
	//アルファ値が変化するかの判定
	bool m_alphaValue;

	//演出時のリセットまでの時間
	int m_waitResetTime;
	//死亡時のリセット
	bool m_deadReset;
	//勝利時のリセット
	bool m_winReset;

	//装備画面から戻る
	bool m_equipmentReturn;

	std::shared_ptr<Font> m_pFont;          //フォント
	std::shared_ptr<Font> m_pSmallFont;     //小フォント

	XINPUT_STATE m_xpad;                //パッド入力
};


