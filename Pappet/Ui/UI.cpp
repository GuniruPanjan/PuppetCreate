#include "UI.h"
#include "Item/Weapon.h"
#include "Item/Shield.h"
#include "Item/Armor.h"
#include "Character/Player.h"
#include "Manager/EnemyManager.h"
#include "Manager/ItemManager.h"
#include "Manager/MapManager.h"
#include "Item/Tool.h"
#include "Manager/MessageManager.h"

namespace
{
	int c_maxHP = 800;
	int c_maxHPWidth = 1000;

	bool c_itemTakingUI = false;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
UI::UI() :
	m_equipmentReturn(false),
	m_xpad()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
UI::~UI()
{
}

/// <summary>
/// ����������
/// </summary>
/// <param name="player"></param>
/// <param name="enemy"></param>
void UI::Init()
{
	m_deadBack = 0;
	m_deadA = 0;
	m_waitResetTime = 0;
	m_deadReset = false;

	m_heelStone = MyLoadGraph("Data/UI/HeelStoneMini.png", 4, 4);
	m_blackSword = MyLoadGraph("Data/UI/������UI.png", 3, 3);
	m_fist = MyLoadGraph("Data/UI/��UI.png", 3, 3);
	m_body = MyLoadGraph("Data/UI/����UI.png", 3, 3);
	m_uglyShield = MyLoadGraph("Data/UI/�X����UI.png", 4, 4);
	m_commonArmor = MyLoadGraph("Data/UI/���}�ȊZUI.png", 3, 3);
	m_dead = MyLoadGraph("Data/UI/YOUDIEDGraph.png", 1, 1);
	m_backRightBar = MyLoadGraph("Data/UI/StatusBar�E�[.png", 2, 2);
	m_backLeftBar = MyLoadGraph("Data/UI/StatusBar���[.png", 2, 2);
	m_backCenterBar = MyLoadGraph("Data/UI/StatusBar����.png", 2, 2);
	m_backOutBar = MyLoadGraph("Data/UI/StatusBar�O�g.png", 2, 2);
	m_hpBar = MyLoadGraph("Data/UI/HPBarRed.png", 2, 2);
	m_staminaBar = MyLoadGraph("Data/UI/StaminaBar.png", 2, 2);
	m_coreBackBar = MyLoadGraph("Data/UI/CoreBar.png", 1, 1);
	m_statusIcon = MyLoadGraph("Data/UI/StatusIcon.png", 1, 1);
	m_equipmentFrame = MyLoadGraph("Data/UI/Frame.png", 1, 1);
	m_actionUI = MyLoadGraph("Data/UI/Action.png", 1, 1);
	m_itemTaking = MyLoadGraph("Data/UI/�A�C�e���擾�͂�.png", 1, 1);
	m_yButton = MyLoadGraph("Data/UI/YButton.png", 1, 1);
	m_bButton = MyLoadGraph("Data/UI/BButton.png", 1, 1);
}

/// <summary>
/// �Q�[����UI�̕`�揈��
/// </summary>
/// <param name="player"></param>
/// <param name="enemy"></param>
/// <param name="eq"></param>
/// <param name="map"></param>
/// <param name="item"></param>
void UI::Draw(Player& player, EnemyManager& enemy, Setting& eq, MapManager& map, ItemManager& item, Weapon& weapon, Shield& shield, Armor& armor, Tool& tool, MessageManager& message)
{
	//�p�b�h���͎擾
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	//�X�e�[�^�X�o�[�̕`��
	StatusDraw(180, 30, player);

	//�v���C���[�̏�ԃA�C�R���`��
	DrawGraph(-50, 0, m_statusIcon, true);

	//�����̕`��
	DrawGraph(150, 550, m_equipmentFrame, true);
	DrawGraph(0, 665, m_equipmentFrame, true);
	DrawGraph(300, 665, m_equipmentFrame, true);
	DrawGraph(150, 780, m_equipmentFrame, true);

	//�������Ă��镨�`��
	EquipmentUIDraw(weapon, shield, armor, tool);

	//�R�A�o�[�̕`��
	DrawGraph(1050, 750, m_coreBackBar, true);

	SetFontSize(40);

	//�x������Ƃ��́@�x������
	//�A�C�e���̎��́@�A�C�e�������
	//�{�X�����ɓ���Ƃ��́@�������̒��ɓ���
	//���[�v����Ƃ��́@�]�ڂ���
	if (player.GetItemPick() || player.GetRestTouch() || player.GetBossEnter() || map.GetCore() || player.GetMessagePick())
	{
		DrawGraph(480, 800, m_actionUI, true);
		DrawGraph(500, 805, m_yButton, true);
		//�x��
		if (player.GetRestTouch())
		{
			DrawFormatString(750, 820, 0xffffff, "�x������");
		}
		//�A�C�e��
		else if (player.GetItemPick())
		{
			DrawFormatString(700, 820, 0xffffff, "�A�C�e�������");
		}
		//�{�X���������
		else if (player.GetBossEnter())
		{
			DrawFormatString(700, 820, 0xffffff, "�������̒��ɓ���");
		}
		//���[�v
		else if (map.GetCore())
		{
			DrawFormatString(750, 820, 0xffffff, "�]�ڂ���");
		}
		//���b�Z�[�W
		else if (player.GetMessagePick())
		{
			DrawFormatString(700, 820, 0xffffff, "���b�Z�[�W��ǂ�");
		}
	}

	//�A�C�e�����������
	if (player.GetTaking())
	{
		if (player.GetNowFrame() == 2.0f)
		{
			c_itemTakingUI = true;
		}
	}

	if (c_itemTakingUI)
	{
		DrawGraph(480, 600, m_itemTaking, true);
		DrawGraph(480, 800, m_actionUI, true);
		DrawGraph(500, 805, m_bButton, true);

		DrawFormatString(800, 820, 0xffffff, "O K");

		ItemTakingUI(item.m_uiItem.u_BlackSword, m_blackSword, 500, 625, 800, 675, "������");
		ItemTakingUI(item.m_uiItem.u_Distorted, m_uglyShield, 500, 645, 800, 675, "�c�񂾏�");
		ItemTakingUI(item.m_uiItem.u_ArmorNormal, m_commonArmor, 500, 635, 800, 675, "���ʂ̊Z");

		//Bbutton�������ƕ���
		if (m_xpad.Buttons[13] == 1)
		{
			item.m_uiItem.u_SmallCore = 0;
			item.m_uiItem.u_MediumCore = 0;
			item.m_uiItem.u_Rubbish = 0;
			item.m_uiItem.u_BlackSword = 0;
			item.m_uiItem.u_Distorted = 0;
			item.m_uiItem.u_ArmorNormal = 0;

			c_itemTakingUI = false;
		}
	}

	if (message.GetDraw())
	{
		DrawGraph(480, 800, m_actionUI, true);
		DrawGraph(500, 805, m_bButton, true);

		DrawFormatString(800, 820, 0xffffff, "O K");
	}

	//�R�A���`��
	DrawFormatString(1400, 905, 0xffffff, "%d", player.GetStatus().s_core);

	SetFontSize(40);
}

/// <summary>
/// �����̕`�揈��
/// </summary>
/// <param name="player"></param>
void UI::EquipmentDraw(Weapon& weapon, Shield& shield, Armor& armor)
{
	//�E����
	if (weapon.GetFist())
	{
		//��
		DrawGraph(110, 310, m_fist, true);
	}
	else if (weapon.GetBlack())
	{
		//������
		DrawGraph(140, 310, m_blackSword, true);
	}
	//������
	if (shield.GetFist())
	{
		//��
		DrawGraph(310, 310, m_fist, true);
	}
	else if (shield.GetUgly())
	{
		//�X����
		DrawGraph(335, 330, m_uglyShield, true);
	}
	//�h��
	if (armor.GetBody())
	{
		//����
		DrawGraph(565, 320, m_body, true);
	}
	else if (armor.GetCommon())
	{
		//���}�Ȗh��
		DrawGraph(530, 320, m_commonArmor, true);
	}
	
}

/// <summary>
/// �E����`�揈��
/// </summary>
void UI::RightDraw(Weapon& weapon, ItemManager& item)
{
	//��
	DrawGraph(70, 56, m_fist, true);

	if (item.GetItem().BlackSword >= 1)
	{
		//������
		DrawGraph(100, 210, m_blackSword, true);
	}
}

/// <summary>
/// ������`�揈��
/// </summary>
void UI::LeftDraw(Shield& shield, ItemManager& item)
{
	//��
	DrawGraph(70, 56, m_fist, true);

	if (item.GetItem().Distorted >= 1)
	{
		//�X����
		DrawGraph(85, 230, m_uglyShield, true);
	}
	
}

/// <summary>
/// �h��`�揈��
/// </summary>
void UI::ArmorDraw(Armor& armor, ItemManager& item)
{
	//��
	DrawGraph(120, 66, m_body, true);

	if (item.GetItem().ArmorNormal >= 1)
	{
		//���}�ȊZ
		DrawGraph(85, 220, m_commonArmor, true);
	}
}

/// <summary>
/// �Q�[����ʂ̑���UI�`�揈��
/// </summary>
/// <param name="weapon"></param>
/// <param name="shield"></param>
/// <param name="armor"></param>
void UI::EquipmentUIDraw(Weapon& weapon, Shield& shield, Armor& armor, Tool& tool)
{
	//�E����
	if (weapon.GetFist())
	{
		//��
		DrawGraph(305, 700, m_fist, true);
	}
	else if (weapon.GetBlack())
	{
		//������
		DrawGraph(317, 700, m_blackSword, true);
	}
	//������
	if (shield.GetFist())
	{
		//��
		DrawGraph(5, 700, m_fist, true);
	}
	else if (shield.GetUgly())
	{
		//�X����
		DrawGraph(20, 720, m_uglyShield, true);
	}
	//�h��
	if (armor.GetBody())
	{
		//����
		DrawGraph(195, 600, m_body, true);
	}
	else if (armor.GetCommon())
	{
		//���}�Ȗh��
		DrawGraph(160, 600, m_commonArmor, true);
	}

	if (tool.GetHeel().sa_number > 0)
	{
		//�A�C�e��
		DrawGraph(115, 815, m_heelStone, true);

		DrawFormatString(260, 930, 0xffffff, "%d", tool.GetHeel().sa_number);
	}
}

/// <summary>
/// �A�C�e������������̕`��
/// </summary>
/// <param name="item"></param>
/// <param name="handle"></param>
void UI::ItemTakingUI(int item, int handle, int x, int y, int charX, int charY, const char* letter)
{
	if (item > 0)
	{
		DrawGraph(x, y, handle, true);

		DrawFormatString(charX, charY, 0xffffff, letter);

		DrawFormatString(1100, 675, 0xffffff, "%d", item);
	}

}

/// <summary>
/// ���S�`�揈��
/// </summary>
void UI::DiedDraw()
{
	//���S���̔w�i�̓���
	if (m_deadBack < 150)
	{
		m_deadBack++;
	}
	//���S�̕����̓���
	if (m_deadA < 255)
	{
		m_deadA++;

		m_waitResetTime = 0;
	}
	else if (m_deadA >= 255)
	{
		if (m_waitResetTime <= 30)
		{
			m_waitResetTime++;
		}
		else
		{
			m_deadReset = true;
		}
	}

	//�w�i�̐F�𔖂�����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_deadBack);
	DrawBox(0, 100, 2000, 400, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//���S���̕������o��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_deadA);
	DrawGraph(-150, -100, m_dead, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

/// <summary>
/// �X�e�[�^�X�o�[�̕`�揈��
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="player"></param>
void UI::StatusDraw(int x, int y, Player& player)
{
	//HP�ő啝���v�Z
	int HPbar = player.GetMaxStatus().sm_hp / 30;
	//HP�ő啝���v�Z
	int HPBarWidth = (int)((float)player.GetStatus().s_hp / player.GetMaxStatus().sm_hp * (185 + (50 * player.GetLevelStatus().sl_hp)));

	//�X�^�~�i�ő啝���v�Z
	int StaminaBar = player.GetMaxStatus().sm_stamina / 10;
	//�X�^�~�i�ő啝���v�Z
	int StaminaBarWidth = (int)((float)player.GetStatus().s_stamina / player.GetMaxStatus().sm_stamina * (150 + (10 * player.GetLevelStatus().sl_stamina)));

	if (HPBarWidth > 0)
	{
		DrawRectGraph(x + 5, y + 8, 0, 0, HPBarWidth, 45, m_hpBar, true);
	}


	for (int i = 0; i < HPbar; i++)
	{
		//�����̃o�[��`��
		if (i > 0 && i != HPbar - 1)
		{
			if (i == 1)
			{
				DrawGraph(x + 23, y, m_backCenterBar, true);
			}
			else
			{
				DrawGraph(x + 23 + (50 * (i - 1)), y, m_backCenterBar, true);
			}
		}
		//���[�̃o�[��`��
		else if (i == 0)
		{
			DrawGraph(x, y, m_backLeftBar, true);
		}
		//�E�[�̃o�[��`��
		else if (i == HPbar - 1)
		{
			DrawGraph(x + 23 + (50 * (i - 1)), y, m_backRightBar, true);
		}

		
	}

	if (StaminaBarWidth > 0)
	{
		DrawRectGraph(x - 8, y + 50, 0, 0, StaminaBarWidth, 50, m_staminaBar, true);
	}

	//���[�̃o�[��`��
	DrawGraph(x, y + 50, m_backLeftBar, true);

	for (int i = 0; i < StaminaBar; i++)
	{
		//�����̃o�[��`��
		if (i > 0 && i != StaminaBar - 1)
		{
			if (i == 1)
			{
				DrawGraph(x + 23, y +50, m_backCenterBar, true);
			}
			else
			{
				DrawGraph(x + 23 + (10 * (i - 1)), y + 50, m_backCenterBar, true);
			}
		}
		//�E�[�̃o�[��`��
		else if (i == StaminaBar - 1)
		{
			DrawGraph(x + 23 + (10 * (i - 1)) + (2 * i), y + 50, m_backRightBar, true);
		}
	}
	
}

/// <summary>
/// �G��HP�`�揈��
/// </summary>
/// <param name="pos"></param>
/// <param name="hp"></param>
/// <param name="maxHP"></param>
void UI::EnemyHPDraw(VECTOR pos, int hp, int maxHP)
{
	//3D���W����2D���W�ɕϊ�
	VECTOR screenPos;
	
	screenPos = ConvWorldPosToScreenPos(pos);

	int screenX = (int)screenPos.x - 100;   //HP�o�[�̏ꏊ�𒲐�
	int screenY = (int)screenPos.y - 300;   //HP�o�[�̍����𒲐�

	//HP�o�[�̍ő啝���v�Z
	int HPBarWidth = (int)((float)hp / maxHP * 200);

	if (HPBarWidth > 0)
	{
		DrawRectGraph(screenX, screenY, 0, 0, HPBarWidth, 30, m_hpBar, true);
	}
}

/// <summary>
/// �{�X��HP�`�揈��
/// </summary>
/// <param name="hp"></param>
/// <param name="maxHP"></param>
/// <param name="name"></param>
void UI::BossHPDraw(int hp, int maxHP, const char* name, const char* subName)
{
	//HP�o�[�̍ő啝���v�Z
	int HPBarWidth = (int)((float)hp / maxHP * 800);

	SetFontSize(30);
	//�{�X�̓��Ď�
	DrawString(450, 670, subName, 0xffffff);

	SetFontSize(40);

	//�{�X�̖��O
	DrawString(450, 700, name, 0xffffff);

	if (HPBarWidth > 0)
	{
		DrawRectGraph(450, 750, 0, 0, HPBarWidth, 50, m_hpBar, true);
	}

}

/// <summary>
/// �I������
/// </summary>
void UI::End()
{
	DeleteGraph(m_blackSword);
	DeleteGraph(m_fist);
	DeleteGraph(m_body);
	DeleteGraph(m_uglyShield);
	DeleteGraph(m_commonArmor);
	DeleteGraph(m_dead);
	DeleteGraph(m_backRightBar);
	DeleteGraph(m_backLeftBar);
	DeleteGraph(m_backCenterBar);
	DeleteGraph(m_backOutBar);
	DeleteGraph(m_hpBar);
	DeleteGraph(m_staminaBar);
	DeleteGraph(m_coreBackBar);
	DeleteGraph(m_statusIcon);
	DeleteGraph(m_equipmentFrame);
	DeleteGraph(m_actionUI);
	DeleteGraph(m_itemTaking);
	DeleteGraph(m_yButton);
	DeleteGraph(m_bButton);
}
