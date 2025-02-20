#include "CoreManager.h"
#include "Character/Player.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
CoreManager::CoreManager()
{
	m_core = 0.0f;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CoreManager::~CoreManager()
{
}

/// <summary>
/// ����������
/// </summary>
void CoreManager::Init()
{
	//m_core = 0.0f;
}

/// <summary>
/// �X�V����
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
/// ���x���グ�ɕK�v�ȃR�A
/// </summary>
/// <param name="player"></param>
/// <returns></returns>
int CoreManager::NeedCore(int level)
{
	//�K�v�ȃ\�E���̌v�Z��100 * (���x����)�@+ 100 * (���x���� - 1) * (���x����) / 2�ōs��
	return 100 * level + 100 * ((level - 1) * (level / 2));
}
