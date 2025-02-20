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
	void DiedDraw();

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

	//死亡してリセットする
	bool GetReset() { return m_deadReset; }

private:
	unsigned int m_hpColor = 0xff0000;    //HPカラー
	unsigned int m_staminaColor = 0x00ff00;       //スタミナカラー
	//プレイヤーのUI関係変数
	//float m_hpCover;
	//float m_staminaCover;
	//float m_hpExpressionDivide1;
	//float m_hpExpressionDivide2;
	//float m_staminaExpressionDivide1;
	//float m_staminaExpressionDivide2;
	//float m_hpPosX1;
	//float m_hpPosY1;
	//float m_hpPosX2;
	//float m_hpPosY2;
	//float m_hpPosY3;
	//float m_hpPosY4;
	//float m_staminaPosX1;
	//float m_staminaPosY1;
	//float m_staminaPosX2;
	//float m_staminaPosY2;
	//float m_staminaPosY3;
	//float m_staminaPosY4;
	//int m_equipmentUI;
	//int m_youDied;

	//敵のUI変数
	//float m_enemyBossHpCover;
	//float m_enemyBossHpExpressionDivide1;
	//float m_enemyBossHpExpressionDivide2;
	//float m_enemyBossHpPosX1;
	//float m_enemyBossHpPosY1;
	//float m_enemyBossHpPosX2;
	//float m_enemyBossHpPosY2;
	//float m_enemyBossHpPosY3;
	//float m_enemyBossHpPosY4;
	//const char* m_enemyBossName;

	//死亡の画像透過アルファ値
	int m_deadBack;
	int m_deadA;

	//死亡時のゲームリセット
	int m_waitResetTime;
	bool m_deadReset;

	//装備画面から戻る
	bool m_equipmentReturn;

	XINPUT_STATE m_xpad;                //パッド入力
};


