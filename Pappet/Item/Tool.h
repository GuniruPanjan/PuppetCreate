#pragma once
#include "ItemBase.h"

/// <summary>
/// ����̃N���X
/// </summary>
class Tool : public ItemBase
{
public:
	//�R���X�g���N�^
	Tool();
	//�f�X�g���N�^
	virtual ~Tool();

	//����������
	void Init();
	//�A�C�e���̏���������
	void ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics);
	//�}�b�v�A�C�e���Ƃ��Ă̍X�V����
	void ItemUpdate(bool taking);

	//�񕜃A�C�e���֌W��Ԃ�
	sTool GetHeel() { return m_heel; }
	int SetHeel(int item) { return m_heel.sa_number -= item; }

private:
	sTool m_heel;        //�񕜃A�C�e���Ƃ��Ă̍\����
};

