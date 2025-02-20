#include "MapManager.h"
#include "Map/MapFirst.h"
#include "Map/MapRest.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
MapManager::MapManager() :
	m_pMap(nullptr)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
MapManager::~MapManager()
{
}

/// <summary>
/// �f�[�^�̏�����
/// </summary>
void MapManager::DataInit(int map)
{
	if (map == 0)
	{
		//�ŏ��̃V�[���̃��������������
		m_pMap = std::make_shared<MapRest>();

		m_pMap->DataInit();
	}
	else if (map == 1)
	{
		//�ŏ��̃V�[���̃��������������
		m_pMap = std::make_shared<MapFirst>();

		m_pMap->DataInit();
	}
	
}

/// <summary>
/// ����������
/// </summary>
void MapManager::Init(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pMap->Init(physics);
}

/// <summary>
/// �X�V����
/// </summary>
void MapManager::Update(std::shared_ptr<MyLibrary::Physics> physics, bool warp, bool enter, bool Dead)
{
	std::shared_ptr<MapBase> pNext = m_pMap->Update(warp, enter, Dead);
	//if (pNext != m_pMap)
	//{
	//	//���݂̃}�b�v�̏I������
	//	m_pMap->End(physics);

	//	//Update���Ԃ����V�����V�[���̊J�n�������s��
	//	m_pMap = pNext;

	//	//�����͂��������
	//	//m_pMap->DataInit();
	//	//m_pMap->Init(physics);
	//}
}

/// <summary>
/// ���[�v�ł̍X�V����
/// </summary>
/// <param name="physics"></param>
/// <param name="warp"></param>
void MapManager::WarpUpdate(std::shared_ptr<MyLibrary::Physics> physics, bool warp, bool title)
{
	std::shared_ptr<MapBase> pNext = m_pMap->WarpUpdate(warp);
	if (pNext != m_pMap)
	{
		//���݂̃}�b�v�̏I������
		m_pMap->End(physics, title);
		m_pMap->MapEnd(physics);

		m_pMap = pNext;

		//�f�[�^�_�E�����[�h
		m_pMap->DataInit();
	}
}

/// <summary>
/// �����������̔���X�V����
/// </summary>
void MapManager::JudgeUpdate()
{
	m_pMap->JudgeUpdate();
}

/// <summary>
/// �R�A�X�V
/// </summary>
void MapManager::CoreUpdate()
{
	m_pMap->CoreUpdate();
}

/// <summary>
/// ���������������
/// </summary>
void MapManager::TriggerReset()
{
	m_pMap->TriggerReset();
}

/// <summary>
/// �`�揈��
/// </summary>
void MapManager::Draw()
{
	m_pMap->Draw();
}

/// <summary>
/// �R�A�`��
/// </summary>
void MapManager::CoreDraw()
{
	m_pMap->CoreDraw();
}

/// <summary>
/// �I������
/// </summary>
void MapManager::End(std::shared_ptr<MyLibrary::Physics> physics, bool title)
{
	m_pMap->End(physics, title);
}
