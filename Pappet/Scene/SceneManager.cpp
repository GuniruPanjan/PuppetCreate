#include "SceneManager.h"
#include "SceneTitle.h"
#include "Input/Input.h"

/// <summary>
/// コンストラクタ
/// </summary>
SceneManager::SceneManager() :
	m_pScene(nullptr)
{
}

/// <summary>
/// デストラクタ
/// </summary>
SceneManager::~SceneManager()
{
}

/// <summary>
/// 初期化処理
/// </summary>
void SceneManager::Init()
{
	//最初のシーンのメモリを解放する
	m_pScene = std::make_shared<SceneTitle>();

	m_pScene->Init();
}

/// <summary>
/// 更新処理
/// </summary>
void SceneManager::Update()
{
	Input::GetInstance().Update();

	std::shared_ptr<SceneBase> pNext = m_pScene->Update();
	if (pNext != m_pScene)
	{
		//現在のシーンの終了処理
		m_pScene->End();

		//Updateが返した新しいシーンの開始処理を行う
		m_pScene = pNext;
		m_pScene->Init();
	}
}

/// <summary>
/// 描画処理
/// </summary>
void SceneManager::Draw()
{
	m_pScene->Draw();
}

/// <summary>
/// 終了処理
/// </summary>
void SceneManager::End()
{
	m_pScene->End();
}


