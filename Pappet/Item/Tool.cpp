#include "Tool.h"

/// <summary>
/// コンストラクタ
/// </summary>
Tool::Tool()
{

}

/// <summary>
/// デストラクタ
/// </summary>
Tool::~Tool()
{
}

/// <summary>
/// 初期化処理
/// </summary>
void Tool::Init()
{
	//回復アイテムは初期化する
	m_heel.sa_number = 3;
}

void Tool::ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
}

void Tool::ItemUpdate(bool taking)
{
}
