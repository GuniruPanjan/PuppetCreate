#pragma once
#include "DxLib.h"
#include<memory>

class Player;
class Effect;

/// <summary>
/// �}�b�v���Ǘ�����N���X
/// </summary>
class Map
{
public:
	//�R���X�g���N�^
	Map();
	//�f�X�g���N�^
	virtual ~Map();

	//����������
	void Init();
	//�X�V����
	void Update();

	/// <summary>
	/// �A�C�e���̃|�W�V�����ݒ�
	/// </summary>
	/// <param name="number">�A�C�e���̃i���o�[</param>
	/// <param name="x">X���W</param>
	/// <param name="y">Y���W</param>
	/// <param name="z">Z���W</param>
	void ItemPos(int number, float x, float y, float z);

	//�`�揈��
	void Draw();

	//�I������
	void End();

	//�}�b�v�̃R���W�����n���h����Ԃ�
	int GetCollisionMap() { return m_collisionHandle; }

	//�}�b�v�̃{�X������ɓ��������Ԃ�
	bool GetRoomEntered() { return m_bossRoomEntered; }

	//�}�b�v�̋x���ꏊ��Ԃ�
	bool GetSavePossible() { return m_saveSpot; }

	//���݂̃A�C�e���̏�Ԃ�Ԃ�
	//bool GetItem(int max) { return item->GetItem(max); }

	//�A�C�e�������邩�Ԃ�
	//bool SetItemSpot(int max, bool set) { return m_itemSpot[max] = set; }

	//�}�b�v�̃|�W�V������Ԃ�
	VECTOR GetVectorMapPos() { return m_MapPosition; }

	//�}�b�v�̋x���|�C���g��Ԃ�
	VECTOR GetRestPos() { return m_restPos; }

private:
	int m_handle;   //�}�b�v�̃f�[�^������ϐ�
	int m_collisionHandle;   //�}�b�v�̃R���W�����̃f�[�^������ϐ�
	int m_light;    //���C�g�����ϐ�
	//int m_itemModel[ITEM_NUMBER];         //�}�b�v�ɒu���A�C�e���̃��f���ϐ�
	float m_size;   //���f���̃T�C�Y�ϐ�
	float m_Xposition;    //X�|�W�V����
	float m_Yposition;    //Y�|�W�V����
	float m_Zposition;    //Z�|�W�V����
	float m_XCollisionposition;    //X�|�W�V����
	float m_YCollisionposition;    //Y�|�W�V����
	float m_ZCollisionposition;    //Z�|�W�V����
	bool m_bossRoomEntered;    //�{�X�����ɓ���������
	bool m_saveSpot;        //�񕜃X�|�b�g���g���锻��
	bool m_oneInit;         //��񂾂�����������
	//bool m_itemSpot[ITEM_NUMBER];        //�A�C�e���������ł���ꏊ
	VECTOR m_MapPosition;   //�}�b�v�̃|�W�V����
	VECTOR m_collisionMapPosition;   //�R���W�����̃|�W�V�����ݒ�
	VECTOR m_restPos;        //�x������|�W�V����

};




