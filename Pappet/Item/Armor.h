#pragma once
#include "ItemBase.h"
#include <string>

/// <summary>
/// 防具のクラス
/// </summary>
class Armor :  public ItemBase
{
public:
	//コンストラクタ
	Armor();
	//デストラクタ
	virtual ~Armor();

	//初期化
	void Init();
	//アイテムの初期化処理
	void ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics);
	//更新処理
	void Update();
	//マップアイテムとしての更新処理
	void ItemUpdate(bool taking);
	//描画処理
	void Draw();
	//終了処理
	void End();

	//装備したかの判定
	bool GetBody() { return m_body.sa_equipment; }
	bool SetBody(bool set) { return m_body.sa_equipment = set; }
	bool GetCommon() { return m_common.sa_equipment; }
	bool SetCommon(bool set) { return m_common.sa_equipment = set; }

	//防御力
	float GetBodyDefence() { return m_body.sa_defense; }
	float GetCommonDefence() { return m_common.sa_defense; }

	//防具のパス
	std::string GetPath() { return m_path; }

private:
	//防具の構造体
	sArmor m_body;      //裸体
	sArmor m_common;    //平凡な鎧

	//防具のパス
	std::string m_path;
};

