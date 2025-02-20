#pragma once
#include "DxLib.h"
#include "Library/MyLibrary.h"
#include "Object/SearchObject.h"
#include "Object/ItemObject.h"
#include <map>
#include <string>

/// <summary>
/// アイテムの親クラス
/// </summary>
class ItemBase
{
public:
	//アイテムの種類構造体
	struct Item
	{
		int SmallCore = 0;       //小型コア
		int MediumCore = 0;      //中型コア
		int Rubbish = 0;         //ゴミ
		int BlackSword = 0;      //黒い剣
		int Distorted = 0;       //歪んだ盾
		int ArmorNormal = 0;     //普通の鎧
	};

	//武器の構造体
	struct sWeapon
	{
		float sw_attack;               //攻撃力
		float sw_muscle;               //筋力補正
		float sw_skill;                //技量補正
		float sw_radius;               //武器の当たり判定
		bool sw_equipment = false;     //装備したかの判定
	};

	//盾の構造体
	struct sShield
	{
		int ss_cut;                    //カット率
		int ss_strength;               //盾強度
		bool ss_equipment = false;     //装備したかの判定
	};

	//防具の構造体
	struct sArmor
	{
		int sa_defense;                //防御力
		bool sa_equipment = false;     //装備したかの判定
	};

	//アイテムの構造体
	struct sTool
	{
		int sa_number;                 //持っている数
	};

	//コンストラクタ
	ItemBase() :
		m_itemUse(0),
		m_itemHandle(-1),
		m_frameIndex(0),
		m_itemPick(false),
		m_isTaking(false),
		m_itemBox(false),
		m_framePos(VGet(0.0f,0.0f,0.0f)),
		m_pos(VGet(0.0f,0.0f,0.0f)),
		m_transMatrix(),
		m_mixMatrix()
	{

	}
	//デストラクタ
	virtual ~ItemBase() {};

	//マップアイテムとしての初期化処理
	virtual void ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics) {};
	//マップアイテムとしての削除処理
	void ItemFinalize(std::shared_ptr<MyLibrary::Physics> physics);
	//マップアイテムとしての判断処理
	void ItemGudgment(int SmallCore, int MediumCore, int Rubbish, int BlackSword, int Distorted, int ArmorNormal);
	//マップアイテムとしての更新処理
	virtual void ItemUpdate(bool taking) {};
	//マップアイテムとしての終了処理
	void ItemEnd();

	//アイテムを拾えるかをの判定を返す
	bool GetItemPick() { return m_itemPick; }
	//アイテムを取得したかを返す
	bool GetItemTaking() { return m_isTaking; }
	//アイテムを取得した時
	bool SetItemTaking(bool set) { return m_isTaking = set; }
	//アイテムを取得した時にボックスに入れる判定
	bool GetItemBox() { return m_itemBox; }
	//アイテムを取得した時にボックスに入れる判定
	bool SetItemBox(bool set) { return m_itemBox = set; }
	//アイテムの種類を返す
	Item GetItemKinds() { return m_item; }
	//アイテムの中心座標を返す
	MyLibrary::LibVec3 GetPos() { return m_centerPos; }

	//範囲外に出た判定
	bool GetIsOut();
protected:
	//アイテム情報を読み込む
	void LoadData(std::string name);
	//当たり判定を作成
	void InitSearch(MyLibrary::LibVec3 pos, float radius);

protected:
	std::shared_ptr<ItemObject> m_pSearch;     //索敵判定

	//アイテムの効果
	int m_itemUse;
	//アイテムのハンドル
	int m_itemHandle;
	//フレーム検索
	int m_frameIndex;
	//アイテムを取れるかの判定
	bool m_itemPick;
	//取られたかどうかの判定
	bool m_isTaking;
	//取った時のアイテム判定
	bool m_itemBox;
	//フレームポジション
	VECTOR m_framePos;
	//ポジション
	VECTOR m_pos;
	//モデルの行列
	MATRIX m_transMatrix;
	//モデルの合成行列
	MATRIX m_mixMatrix;

	Item m_item;

	std::shared_ptr<MyLibrary::Physics> m_pPhysics;

	MyLibrary::LibVec3 m_centerPos;              //中心座標
};
