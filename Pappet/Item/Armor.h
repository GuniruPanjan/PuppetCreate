#pragma once
#include "ItemBase.h"
#include <string>

/// <summary>
/// �h��̃N���X
/// </summary>
class Armor :  public ItemBase
{
public:
	//�R���X�g���N�^
	Armor();
	//�f�X�g���N�^
	virtual ~Armor();

	//������
	void Init();
	//�A�C�e���̏���������
	void ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics);
	//�X�V����
	void Update();
	//�}�b�v�A�C�e���Ƃ��Ă̍X�V����
	void ItemUpdate(bool taking);
	//�`�揈��
	void Draw();
	//�I������
	void End();

	//�����������̔���
	bool GetBody() { return m_body.sa_equipment; }
	bool SetBody(bool set) { return m_body.sa_equipment = set; }
	bool GetCommon() { return m_common.sa_equipment; }
	bool SetCommon(bool set) { return m_common.sa_equipment = set; }

	//�h���
	float GetBodyDefence() { return m_body.sa_defense; }
	float GetCommonDefence() { return m_common.sa_defense; }

	//�h��̃p�X
	std::string GetPath() { return m_path; }

private:
	//�h��̍\����
	sArmor m_body;      //����
	sArmor m_common;    //���}�ȊZ

	//�h��̃p�X
	std::string m_path;
};

