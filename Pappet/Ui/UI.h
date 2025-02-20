#pragma once
#include "UIBase.h"


class Player;
class EnemyManager;
class Setting;
class MapManager;
class ItemManager;
class Weapon;
class Shield;
class Armor;
class Tool;
class MessageManager;

/// <summary>
/// UI�֌W�̃N���X
/// </summary>
class UI : public UIBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	UI();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~UI();

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="player">�v���C���[�̌Ăяo��</param>
	/// <param name="enemy">�G�l�~�[�̌Ăяo��</param>
	void Init();

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="player">�v���C���[�̌Ăяo��</param>
	/// <param name="enemy">�G�l�~�[�̌Ăяo��</param>
	/// <param name="eq">�����̌Ăяo��</param>
	/// <param name="map">�}�b�v�̌Ăяo��</param>
	/// <param name="item">�A�C�e���̌Ăяo��</param>
	void Draw(Player& player, EnemyManager& enemy, Setting& eq, MapManager& map, ItemManager& item, Weapon& weapon, Shield& shield, Armor& armor, Tool& tool, MessageManager& message);

	/// <summary>
	/// �����̕`�揈��
	/// </summary>
	void EquipmentDraw(Weapon& weapon, Shield& shield, Armor& armor);

	/// <summary>
	/// �E����`�揈��
	/// </summary>
	void RightDraw(Weapon& weapon, ItemManager& item);

	/// <summary>
	/// ������`�揈��
	/// </summary>
	void LeftDraw(Shield& shield, ItemManager& item);

	/// <summary>
	/// �h��`�揈��
	/// </summary>
	void ArmorDraw(Armor& armor, ItemManager& item);

	//�����̕`��
	void EquipmentUIDraw(Weapon& weapon, Shield& shield, Armor& armor, Tool& tool);

	/// <summary>
	/// �A�C�e����������̕`��
	/// </summary>
	/// <param name="item"></param>
	/// <param name="handle"></param>
	void ItemTakingUI(int item, int handle, int x, int y, int charX, int charY, const char* letter);

	/// <summary>
	/// ���񂾂Ƃ��̕`��
	/// </summary>
	void DiedDraw();

	/// <summary>
	/// �X�e�[�^�X�o�[�̕`�揈��
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="player"></param>
	void StatusDraw(int x, int y, Player& player);

	/// <summary>
	/// �G��HP�`�揈��
	/// </summary>
	/// <param name="enemy"></param>
	void EnemyHPDraw(VECTOR pos, int hp, int maxHP);

	/// <summary>
	/// �{�X��HP�`�揈��
	/// </summary>
	void BossHPDraw(int hp, int maxHP, const char* name, const char* subName);

	/// <summary>
	/// �I������
	/// </summary>
	void End();

	//���S���ă��Z�b�g����
	bool GetReset() { return m_deadReset; }

private:
	unsigned int m_hpColor = 0xff0000;    //HP�J���[
	unsigned int m_staminaColor = 0x00ff00;       //�X�^�~�i�J���[
	//�v���C���[��UI�֌W�ϐ�
	//float m_hpCover;
	//float m_staminaCover;
	//float m_hpExpressionDivide1;
	//float m_hpExpressionDivide2;
	//float m_staminaExpressionDivide1;
	//float m_staminaExpressionDivide2;
	//float m_hpPosX1;
	//float m_hpPosY1;
	//float m_hpPosX2;
	//float m_hpPosY2;
	//float m_hpPosY3;
	//float m_hpPosY4;
	//float m_staminaPosX1;
	//float m_staminaPosY1;
	//float m_staminaPosX2;
	//float m_staminaPosY2;
	//float m_staminaPosY3;
	//float m_staminaPosY4;
	//int m_equipmentUI;
	//int m_youDied;

	//�G��UI�ϐ�
	//float m_enemyBossHpCover;
	//float m_enemyBossHpExpressionDivide1;
	//float m_enemyBossHpExpressionDivide2;
	//float m_enemyBossHpPosX1;
	//float m_enemyBossHpPosY1;
	//float m_enemyBossHpPosX2;
	//float m_enemyBossHpPosY2;
	//float m_enemyBossHpPosY3;
	//float m_enemyBossHpPosY4;
	//const char* m_enemyBossName;

	//���S�̉摜���߃A���t�@�l
	int m_deadBack;
	int m_deadA;

	//���S���̃Q�[�����Z�b�g
	int m_waitResetTime;
	bool m_deadReset;

	//������ʂ���߂�
	bool m_equipmentReturn;

	XINPUT_STATE m_xpad;                //�p�b�h����
};


