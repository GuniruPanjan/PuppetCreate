#pragma once
#include "ItemBase.h"
#include <memory>
#include <map>
#include <string>

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
	//武器の当たり判定用リグ
	VECTOR WeaponPattern();
	VECTOR WeaponTip();
	//マップアイテムとしての更新処理
	void ItemUpdate(bool taking);
	//描画処理
	void Draw();
	//マップアイテムとしての描画処理
	void ItemDraw();
	//終了処理
	void End();

	//ステータスの判定
	float GetFistAttack() { return m_fist.sw_attack; }

	float GetBlackAttack() { return m_black.sw_attack; }
	float GetBlackMuscle() { return m_black.sw_muscle; }
	float GetBlackSkill() { return m_black.sw_skill; }

	float GetBatAttack() { return m_bat.sw_attack; }
	float GetBatMuscle() { return m_bat.sw_muscle; }
	float GetBatSkill() { return m_bat.sw_skill; }

	//装備したかの判定
	bool GetFist() { return m_fist.sw_equipment; }
	bool SetFist(bool set) { return m_fist.sw_equipment = set; }

	bool GetBlack() { return m_black.sw_equipment; }
	bool SetBlack(bool set) { return m_black.sw_equipment = set; }

	bool GetBat() { return m_bat.sw_equipment; }
	bool SetBat(bool set) { return m_bat.sw_equipment = set; }

private:
	//剣の構造体
	sWeapon m_fist;      //素手
	sWeapon m_black;     //黒い剣
	sWeapon m_bat;       //木のバット

	int m_weaponPatternFrame;
	int m_weaponTipFrame;

	const char* m_weaponPatternName;
	const char* m_weaponTipName;

	float m_size;

	float m_weaponMatrixX = 0.0f;
	float m_weaponMatrixY = 0.0f;
	float m_weaponMatrixZ = 0.0f;
};

