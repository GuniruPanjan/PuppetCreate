#include "UIBase.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
UIBase::UIBase() :
	m_heelStone(0),
	m_eqFist(0),
	m_sword(0),
	m_eqSword(0),
	m_eqShield(0),
	m_equipment(0),
	m_itemBox(0),
	m_selectEquipment(0),
	m_blackSword(0),
	m_fist(0),
	m_body(0),
	m_uglyShield(0),
	m_commonArmor(0),
	m_rest(0),
	m_dead(0),
	m_selectUi(0),
	m_levelUp(0),
	m_backRightBar(0),
	m_backLeftBar(0),
	m_backCenterBar(0),
	m_backOutBar(0),
	m_hpBar(0),
	m_staminaBar(0),
	m_coreBackBar(0),
	m_statusIcon(0),
	m_equipmentFrame(0),
	m_actionUI(0),
	m_itemTaking(0),
	m_yButton(0),
	m_bButton(0),
	m_messageUI(0)
{
	//�摜�ǂݍ���
	//m_heelStone = MyLoadGraph("Data/UI/HeelStoneMini.png", 6, 6);
	//m_eqFist = MyLoadGraph("Data/UI/FistUi.png", 7, 7);
	//m_sword = MyLoadGraph("Data/UI/SwordUI.png", 3, 3);
	//m_eqSword = MyLoadGraph("Data/UI/SwordUI.png", 7, 7);
	//m_eqShield = MyLoadGraph("Data/UI/Shield.png", 7, 7);

	////�����֌W
	//m_equipment = MyLoadGraph("Data/UI/�������UI.png", 1, 1);
	//m_itemBox = MyLoadGraph("Data/UI/�A�C�e�����UI.png", 1, 1);
	//m_selectEquipment = MyLoadGraph("Data/UI/�����I�����UI.png", 1, 1);
	//m_blackSword = MyLoadGraph("Data/UI/������UI.png", 3, 3);
	//m_fist = MyLoadGraph("Data/UI/��UI.png", 3, 3);
	//m_body = MyLoadGraph("Data/UI/����UI.png", 3, 3);
	//m_uglyShield = MyLoadGraph("Data/UI/�X����UI.png", 4, 4);
	//m_commonArmor = MyLoadGraph("Data/UI/���}�ȊZUI.png", 3, 3);

	////UI�֌W
	//m_rest = MyLoadGraph("Data/UI/�l�p�̔��������`�̈͂�.png", 1, 1);
	//m_dead = MyLoadGraph("Data/UI/YOUDIEDGraph.png", 1, 1);
	//m_selectUi = MyLoadGraph("Data/UI/�I��UI.png", 2, 2);
	//m_levelUp = MyLoadGraph("Data/UI/���x���X�e�[�^�XUI.png", 1, 1);
	//m_backRightBar = MyLoadGraph("Data/UI/StatusBar�E�[.png", 2, 2);
	//m_backLeftBar = MyLoadGraph("Data/UI/StatusBar���[.png", 2, 2);
	//m_backCenterBar = MyLoadGraph("Data/UI/StatusBar����.png", 2, 2);
	//m_backOutBar = MyLoadGraph("Data/UI/StatusBar�O�g.png", 2, 2);
	//m_hpBar = MyLoadGraph("Data/UI/HPBarRed.png", 2, 2);
	//m_staminaBar = MyLoadGraph("Data/UI/StaminaBar.png", 2, 2);
	//m_coreBackBar = MyLoadGraph("Data/UI/CoreBar.png", 1, 1);
	//m_statusIcon = MyLoadGraph("Data/UI/StatusIcon.png", 1, 1);
	//m_equipmentFrame = MyLoadGraph("Data/UI/Frame.png", 1, 1);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
UIBase::~UIBase()
{
	//���������
	DeleteGraph(m_heelStone);
	DeleteGraph(m_sword);
	DeleteGraph(m_eqFist);
	DeleteGraph(m_eqSword);
	DeleteGraph(m_eqShield);
	DeleteGraph(m_equipment);
	DeleteGraph(m_itemBox);
	DeleteGraph(m_selectEquipment);
	DeleteGraph(m_blackSword);
	DeleteGraph(m_fist);
	DeleteGraph(m_body);
	DeleteGraph(m_uglyShield);
	DeleteGraph(m_commonArmor);
	DeleteGraph(m_dead);
	DeleteGraph(m_rest);
	DeleteGraph(m_selectUi);
	DeleteGraph(m_levelUp);
	DeleteGraph(m_coreBackBar);
	DeleteGraph(m_statusIcon);
	DeleteGraph(m_equipmentFrame);
	DeleteGraph(m_hpBar);
	DeleteGraph(m_backOutBar);
}

/// <summary>
/// �摜�̃T�C�Y��ς��ă��[�h����֐�
/// </summary>
/// <param name="FileName">�摜��FileName</param>
/// <param name="XSize">�摜�̉��������鐔</param>
/// <param name="YSize">�摜�̏c�������鐔</param>
/// <returns>�T�C�Y��ς����摜��Ԃ�</returns>
int UIBase::MyLoadGraph(const char* FileName, int XSize, int YSize)
{
	int handle = LoadGraph(FileName);  //�摜�̃��[�h
	if (handle != -1)  //�摜�̃��[�h�ɐ��������ꍇ
	{
		int SizeX, SizeY;  //�摜�T�C�Y���i�[���邽�߂̕ϐ���p��

		GetGraphSize(handle, &SizeX, &SizeY);  //���[�h�����摜�̃T�C�Y�擾

		int NowScreen = GetDrawScreen();    //���݂̕`��Ώۉ�ʂ��ꎞ�ۑ�

		SizeX /= XSize;
		SizeY /= YSize;

		int miniHandle = MakeScreen(SizeX, SizeY, TRUE);  //�T�C�Y�ύX��̃O���t�B�b�N�n���h�����쐬
		if (miniHandle == -1)   //�n���h���̍쐬�Ɏ��s�����ꍇ
		{
			DeleteGraph(handle);   //���������[�N���Ȃ��悤�ɁA�{���̃O���t�B�b�N�n���h�����폜
		}
		SetDrawScreen(miniHandle);   //�`��Ώۉ�ʂ��قǍ�����T�C�Y�̃O���t�B�b�N�n���h���ɕύX
		DrawExtendGraph(0, 0, SizeX, SizeY, handle, TRUE);   //���[�h�����摜��`��
		DeleteGraph(handle);   //�s�v�ɂȂ����{���̃T�C�Y�̃O���t�B�b�N�n���h�����폜
		SetDrawScreen(NowScreen);  //���݂̕`��Ώۉ�ʂ����̉�ʂɖ߂�

		return miniHandle;  //�k�߂��摜�̃O���t�B�b�N�n���h����Ԃ�
	}
	else   //�摜�̃��[�h�Ɏ��s�����ꍇ
	{
		return -1;   //�G���[����
	}
}


