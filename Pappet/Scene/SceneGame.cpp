#include "SceneGame.h"
#include "SceneTitle.h"

/// <summary>
/// コンストラクタ
/// </summary>
SceneGame::SceneGame()
{
}

/// <summary>
/// デストラクタ
/// </summary>
SceneGame::~SceneGame()
{
}

/// <summary>
/// 初期化処理
/// </summary>
void SceneGame::Init()
{
	game->Init();
}

/// <summary>
/// 更新処理
/// </summary>
/// <returns></returns>
std::shared_ptr<SceneBase> SceneGame::Update()
{
#if _DEBUG
	MyLibrary::DebugDraw::Clear();
#endif

	game->Update();
	m_pBgm->Update(m_pSetting->GetVolume());

	if (game->GetTitle())
	{
		return std::make_shared<SceneTitle>();
	}

	return shared_from_this();  //自身のポインタを返す
}

/// <summary>
/// 描画処理
/// </summary>
void SceneGame::Draw()
{
	game->Draw();

#if _DEBUG
	MyLibrary::DebugDraw::Draw();
#endif
}

/// <summary>
/// 終了処理
/// </summary>
void SceneGame::End()
{
	game->End();
}

