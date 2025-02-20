#pragma once
#include "ItemBase.h"

/// <summary>
/// 武器のクラス
/// </summary>
class Weapon : public ItemBase
{
public:
	//コンストラクタ
	Weapon();
	//デストラクタ
	virtual ~Weapon();

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
	//終了処理
	void End();

	//ステータスの判定
	float GetFistAttack() { return m_fist.sw_attack; }
	float GetBlackAttack() { return m_black.sw_attack; }
	float GetBlackMuscle() { return m_black.sw_muscle; }
	float GetBlackSkill() { return m_black.sw_skill; }

	//装備したかの判定
	bool GetFist() { return m_fist.sw_equipment; }
	bool SetFist(bool set) { return m_fist.sw_equipment = set; }
	bool GetBlack() { return m_black.sw_equipment; }
	bool SetBlack(bool set) { return m_black.sw_equipment = set; }

private:
	//剣の構造体
	sWeapon m_fist;      //素手
	sWeapon m_black;     //黒い剣
};

