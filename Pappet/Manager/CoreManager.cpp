#include "CoreManager.h"
#include "Character/Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
CoreManager::CoreManager()
{
	m_core = 0.0f;
}

/// <summary>
/// デストラクタ
/// </summary>
CoreManager::~CoreManager()
{
}

/// <summary>
/// 初期化処理
/// </summary>
void CoreManager::Init()
{
}

/// <summary>
/// 更新処理
/// </summary>
void CoreManager::Update(int core)
{
	m_core = core;
}

void CoreManager::DeadUpdate()
{
}

void CoreManager::End()
{
}

/// <summary>
/// レベル上げに必要なコア
/// </summary>
/// <param name="player"></param>
/// <returns></returns>
int CoreManager::NeedCore(int level)
{
	//必要なソウルの計算は100 * (レベル数)　+ 100 * (レベル数 - 1) * (レベル数) / 2で行く
	return 100 * level + 100 * ((level - 1) * (level / 2));
}
