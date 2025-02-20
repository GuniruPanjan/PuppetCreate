#pragma once
#include "ItemBase.h"

/// <summary>
/// 道具のクラス
/// </summary>
class Tool : public ItemBase
{
public:
	//コンストラクタ
	Tool();
	//デストラクタ
	virtual ~Tool();

	//初期化処理
	void Init();
	//アイテムの初期化処理
	void ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics);
	//マップアイテムとしての更新処理
	void ItemUpdate(bool taking);

	//回復アイテム関係を返す
	sTool GetHeel() { return m_heel; }
	int SetHeel(int item) { return m_heel.sa_number -= item; }

private:
	sTool m_heel;        //回復アイテムとしての構造体
};

