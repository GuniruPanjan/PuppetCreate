#pragma once
#include <memory>
#include <map>
#include <string>
#include <List>
#include "Library/MyLibrary.h"
#include "Item/ItemBase.h"

class GameManager;
class Armor;
class Shield;
class Weapon;
class Tool;

/// <summary>
/// アイテムを管理するクラス
/// </summary>
class ItemManager
{
public:
	//アイテムの構造体
	struct Item
	{
		int SmallCore = 0;       //小型コア
		int MediumCore = 0;      //中型コア
		int Rubbish = 0;         //ゴミ
		int BlackSword = 0;      //黒い剣
		int Distorted = 0;       //歪んだ盾
		int ArmorNormal = 0;     //普通の鎧
		int Bat = 0;             //バット
		int WoodShield = 0;      //木の盾

	}m_item;

	//UI表示用の構造体
	struct UIItem
	{
		int u_SmallCore = 0;     //小型コア
		int u_MediumCore = 0;    //中型コア
		int u_Rubbish = 0;       //ゴミ
		int u_BlackSword = 0;    //黒い剣
		int u_Distorted = 0;     //歪んだ盾
		int u_ArmorNormal = 0;   //普通の鎧
		int u_Bat = 0;           //バット
		int u_WoodShield = 0;    //木の盾

	}m_uiItem;

	//アイテムを順に取得した用の構造体
	struct ItemOrder
	{
		int o_SmallCore = 0;     //小型コア
		int o_MediumCore = 0;    //中型コア
		int o_Rubbish = 0;       //ゴミ
		int o_BlackSword = 0;    //黒い剣
		int o_Distorted = 0;     //歪んだ盾
		int o_ArmorNormal = 0;   //普通の鎧
		int o_Bat = 0;           //バット
		int o_WoodShield = 0;    //木の盾
	}m_orderItem;

	//アイテム生成情報
	struct ItemGenerateInfo
	{
		std::string itemName;      //アイテムの名前
		int mapNumber;             //マップの番号
		int posX;                  //アイテムのX座標
		int posY;                  //アイテムのY座標
		int posZ;                  //アイテムのZ座標
		
		//アイテム識別
		int SmallCore = 0;       //小型コア
		int MediumCore = 0;      //中型コア
		int Rubbish = 0;         //ゴミ
		int BlackSword = 0;      //黒い剣
		int Distorted = 0;       //歪んだ盾
		int ArmorNormal = 0;     //普通の鎧
		int Bat = 0;             //バット
		int WoodShield = 0;      //木の盾

		bool isCreated = false;    //生成済みかどうか
		bool isPickUp = false;     //拾われたかどうか
	};

public:
	//コンストラクタ
	ItemManager();
	//デストラクタ
	virtual ~ItemManager();
	//初期化処理
	void Init();
	//ゲーム内での初期化処理
	void GameInit(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager);
	//更新処理
	void Update(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, bool taking);

	//描画処理
	void Draw();

	//終了処理
	void End();

	//アイテムを拾えるかを返す
	bool GetItemPick() { return m_itemPick; }
	//アイテム所得状態を返す
	Item GetItem() { return m_item; }
	//アイテム所得順を返す
	std::list<std::string> GetItemOrder() { return m_itemOrder; }
private:
	//アイテムの生成
	void CreateItem(float posX, float posY, float posZ, std::string name, std::shared_ptr<MyLibrary::Physics> physics);
	//何のアイテムかの判断
	void CheckItem(std::string name, int SmallCore, int MediumCore, int Rubbish, int BlackSword, int Distorted, int ArmorNormal, int Bat, int WoodShield);
	//アイテムの順序を入れる
	void AddItem(ItemOrder& item);
private:
	//アイテムの管理
	std::list<std::shared_ptr<ItemBase>> m_pItems;
	//アイテムの生成情報
	std::list<std::shared_ptr<ItemGenerateInfo>> m_pGenerateInfo;

	std::shared_ptr<Armor> m_parmor;
	std::shared_ptr<Shield> m_pshield;
	std::shared_ptr<Weapon> m_pweapon;
	std::shared_ptr<Tool> m_ptool;

	//ステージ毎のアイテムの生成数
	std::unordered_map<int, int> m_itemGenerationCountPerOneMap;

	bool m_itemPick;      //アイテムを取れるか

	std::list<std::string> m_itemOrder;     //アイテム取得順序を保持するリスト

};

