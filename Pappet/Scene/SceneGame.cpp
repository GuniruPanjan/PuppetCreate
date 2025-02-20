#include "SceneGame.h"
#include "SceneTitle.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneGame::SceneGame()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneGame::~SceneGame()
{
}

/// <summary>
/// ����������
/// </summary>
void SceneGame::Init()
{
	game->Init();
}

/// <summary>
/// �X�V����
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

	return shared_from_this();  //���g�̃|�C���^��Ԃ�
}

/// <summary>
/// �`�揈��
/// </summary>
void SceneGame::Draw()
{
	game->Draw();

#if _DEBUG
	MyLibrary::DebugDraw::Draw();
#endif
}

/// <summary>
/// �I������
/// </summary>
void SceneGame::End()
{
	game->End();
}

