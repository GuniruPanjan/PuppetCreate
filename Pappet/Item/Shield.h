#pragma once
#include "ItemBase.h"

/// <summary>
/// 盾のクラス
/// </summary>
class Shield : public ItemBase
{
public:
	//コンストラクタ
	Shield();
	//デストラクタ
	virtual ~Shield();

	//初期化
	void Init();
	//アイテムの初期化処理
	void ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics);
	//更新処理
	void Update(MATRIX mat);
	//マップアイテムとしての更新処理
	void ItemUpdate(bool taking);
	//描画処理
	void Draw();
	//マップアイテムとしての描画処理
	void ItemDraw();
	//描画処理
	void End();

	//装備したかの判定
	bool GetFist() { return m_fist.ss_equipment; }
	bool SetFist(bool set) { return m_fist.ss_equipment = set; }

	bool GetUgly() { return m_ugly.ss_equipment; }
	bool SetUgly(bool set) { return m_ugly.ss_equipment = set; }

	bool GetWood() { return m_wood.ss_equipment; }
	bool SetWood(bool set) { return m_wood.ss_equipment = set; }

	//盾の強度
	float GetStrengthUgly() { return m_ugly.ss_strength; }
	float GetStrengthWood() { return m_wood.ss_strength; }
	
private:
	//盾の構造体
	sShield m_fist;    //素手
	sShield m_ugly;    //醜い盾
	sShield m_wood;    //木の盾

	float m_shieldMatrixX = 0.0f;
	float m_shieldMatrixY = 0.0f;
	float m_shieldMatrixZ = 0.0f;
};

