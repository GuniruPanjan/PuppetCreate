#include "Tool.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Tool::Tool()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Tool::~Tool()
{
}

/// <summary>
/// ����������
/// </summary>
void Tool::Init()
{
	//�񕜃A�C�e���͏���������
	m_heel.sa_number = 3;
}

void Tool::ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
}

void Tool::ItemUpdate(bool taking)
{
}
