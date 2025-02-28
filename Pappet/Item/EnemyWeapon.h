#pragma once
#include "ItemBase.h"

/// <summary>
/// 敵の武器クラス
/// </summary>
class EnemyWeapon : public ItemBase
{
public:
	//コンストラクタ
	EnemyWeapon();
	//デストラクタ
	virtual ~EnemyWeapon();

	//初期化
	void Init(std::string path, VECTOR pos, float x, float y, float z, float size);
	//更新処理
	void Update(MATRIX mat, float size);
	//ポジションとして使うフレーム
	void WeaponFrame(const char* frame1, const char* frame2);
	//描画処理
	void Draw();
	//終了処理
	void End();

	VECTOR GetFramePos1() { return m_weaponFramePos[0]; }
	VECTOR GetFramePos2() { return m_weaponFramePos[1];}

private:
	float m_weaponMatrixX = 0.0f;
	float m_weaponMatrixY = 0.0f;
	float m_weaponMatrixZ = 0.0f;

	int m_weaponFrame[2];           //武器のフレーム
	VECTOR m_weaponFramePos[2];     //武器のフレームの座標
};

