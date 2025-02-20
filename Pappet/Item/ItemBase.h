#pragma once
#include "DxLib.h"
#include "Library/MyLibrary.h"
#include "Object/SearchObject.h"
#include "Object/ItemObject.h"
#include <map>
#include <string>

/// <summary>
/// �A�C�e���̐e�N���X
/// </summary>
class ItemBase
{
public:
	//�A�C�e���̎�ލ\����
	struct Item
	{
		int SmallCore = 0;       //���^�R�A
		int MediumCore = 0;      //���^�R�A
		int Rubbish = 0;         //�S�~
		int BlackSword = 0;      //������
		int Distorted = 0;       //�c�񂾏�
		int ArmorNormal = 0;     //���ʂ̊Z
	};

	//����̍\����
	struct sWeapon
	{
		float sw_attack;               //�U����
		float sw_muscle;               //�͕ؗ␳
		float sw_skill;                //�Z�ʕ␳
		float sw_radius;               //����̓����蔻��
		bool sw_equipment = false;     //�����������̔���
	};

	//���̍\����
	struct sShield
	{
		int ss_cut;                    //�J�b�g��
		int ss_strength;               //�����x
		bool ss_equipment = false;     //�����������̔���
	};

	//�h��̍\����
	struct sArmor
	{
		int sa_defense;                //�h���
		bool sa_equipment = false;     //�����������̔���
	};

	//�A�C�e���̍\����
	struct sTool
	{
		int sa_number;                 //�����Ă��鐔
	};

	//�R���X�g���N�^
	ItemBase() :
		m_itemUse(0),
		m_itemHandle(-1),
		m_frameIndex(0),
		m_itemPick(false),
		m_isTaking(false),
		m_itemBox(false),
		m_framePos(VGet(0.0f,0.0f,0.0f)),
		m_pos(VGet(0.0f,0.0f,0.0f)),
		m_transMatrix(),
		m_mixMatrix()
	{

	}
	//�f�X�g���N�^
	virtual ~ItemBase() {};

	//�}�b�v�A�C�e���Ƃ��Ă̏���������
	virtual void ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics) {};
	//�}�b�v�A�C�e���Ƃ��Ă̍폜����
	void ItemFinalize(std::shared_ptr<MyLibrary::Physics> physics);
	//�}�b�v�A�C�e���Ƃ��Ă̔��f����
	void ItemGudgment(int SmallCore, int MediumCore, int Rubbish, int BlackSword, int Distorted, int ArmorNormal);
	//�}�b�v�A�C�e���Ƃ��Ă̍X�V����
	virtual void ItemUpdate(bool taking) {};
	//�}�b�v�A�C�e���Ƃ��Ă̏I������
	void ItemEnd();

	//�A�C�e�����E���邩���̔����Ԃ�
	bool GetItemPick() { return m_itemPick; }
	//�A�C�e�����擾��������Ԃ�
	bool GetItemTaking() { return m_isTaking; }
	//�A�C�e�����擾������
	bool SetItemTaking(bool set) { return m_isTaking = set; }
	//�A�C�e�����擾�������Ƀ{�b�N�X�ɓ���锻��
	bool GetItemBox() { return m_itemBox; }
	//�A�C�e�����擾�������Ƀ{�b�N�X�ɓ���锻��
	bool SetItemBox(bool set) { return m_itemBox = set; }
	//�A�C�e���̎�ނ�Ԃ�
	Item GetItemKinds() { return m_item; }
	//�A�C�e���̒��S���W��Ԃ�
	MyLibrary::LibVec3 GetPos() { return m_centerPos; }

	//�͈͊O�ɏo������
	bool GetIsOut();
protected:
	//�A�C�e������ǂݍ���
	void LoadData(std::string name);
	//�����蔻����쐬
	void InitSearch(MyLibrary::LibVec3 pos, float radius);

protected:
	std::shared_ptr<ItemObject> m_pSearch;     //���G����

	//�A�C�e���̌���
	int m_itemUse;
	//�A�C�e���̃n���h��
	int m_itemHandle;
	//�t���[������
	int m_frameIndex;
	//�A�C�e�������邩�̔���
	bool m_itemPick;
	//���ꂽ���ǂ����̔���
	bool m_isTaking;
	//��������̃A�C�e������
	bool m_itemBox;
	//�t���[���|�W�V����
	VECTOR m_framePos;
	//�|�W�V����
	VECTOR m_pos;
	//���f���̍s��
	MATRIX m_transMatrix;
	//���f���̍����s��
	MATRIX m_mixMatrix;

	Item m_item;

	std::shared_ptr<MyLibrary::Physics> m_pPhysics;

	MyLibrary::LibVec3 m_centerPos;              //���S���W
};
