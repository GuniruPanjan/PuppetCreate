#pragma once
#include "Manager/SEManager.h"
#include "DxLib.h"
#include "Ui/UIBase.h"
#include<memory>

class SelectManager;
class Shield;
class Weapon;
class Armor;
class Player;
class CoreManager;

/// <summary>
/// �ݒ菈���p�̃N���X
/// </summary>
class Setting : public UIBase
{
public:

	struct SelectXY
	{
		int oneX;
		int oneY;
		int secondX;
		int secondY;
	};

	struct SelectEquipment
	{
		bool right = false;
		bool left = false;
		bool armor = false;
	};

	//���x�����グ�����ʊi�[����\����
	struct LevelUp
	{
		int sl_all;
		int sl_hp;
		int sl_stamina;
		int sl_muscle;
		int sl_skill;
	}ms_levelUP;

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Setting();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Setting();

	/// <summary>
	/// ����������
	/// </summary>
	void Init();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// ���j���[�̍X�V����
	/// </summary>
	void MenuUpdate(Player& player);

	/// <summary>
	/// �X�e�[�^�X��ʂ�ς���X�V����
	/// </summary>
	void MenuChange();

	/// <summary>
	/// �����̍X�V����
	/// </summary>
	void EquipmentUpdate();

	/// <summary>
	/// �x���̍X�V����
	/// </summary>
	void RestUpdate(Player& player, CoreManager& core);

	/// <summary>
	/// ���x���A�b�v����
	/// </summary>
	void LevelUpdate(Player& player, CoreManager& core);

	/// <summary>
	/// ���x���A�b�v����2
	/// </summary>
	void LevelUp(CoreManager& core, int origin, int& level, int now);

	/// <summary>
	/// �A�C�e���{�b�N�X�X�V����
	/// </summary>
	void ItemBoxUpdate();

	/// <summary>
	/// �����I����ʍX�V����
	/// </summary>
	void EquipmentDecisionUpdate(Weapon& weapon, Shield& shield, Armor& armor, ItemManager& item);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// �I�𒆂̐F��ς���
	/// </summary>
	/// <param name="select">�񋓌^</param>
	/// <param name="now">�I�����Ă����</param>
	/// <param name="other1">����ȊO�P</param>
	/// <param name="other2">����ȊO�Q</param>
	/// <param name="other3">����ȊO�R</param>
	/// <param name="other4">����ȊO�S</param>
	/// <param name="black">�����摜�̃u�����h��</param>
	/// <param name="white">�����摜�̃u�����h��</param>
	void BrightColorDraw(int select, int now, int other1, int other2, int other3, int other4, int black, int white);

	// <summary>
	/// �I�𒆂̐F��ς���
	/// </summary>
	/// <param name="select">�񋓌^</param>
	/// <param name="now">�I�����Ă����</param>
	/// <param name="other1">����ȊO�P</param>
	/// <param name="other2">����ȊO�Q</param>
	/// <param name="other3">����ȊO�R</param>
	/// <param name="other4">����ȊO�S</param>
	/// <param name="volume">����</param>
	void VolumeColorDraw(int select, int now, int other1, int other2, int other3, int other4, int volume);

	/// <summary>
	/// �ݒ�̕`��
	/// </summary>
	/// <param name="volume">����</param>
	void SettingDraw(int volume);

	/// <summary>
	/// ���j���[�`��
	/// </summary>
	void MenuDraw();

	/// <summary>
	/// �w�i�`��
	/// </summary>
	void MenuBackDraw();

	/// <summary>
	/// �X�e�[�^�X��ʂ�ς���`�揈��
	/// </summary>
	void MenuChangeDraw();

	/// <summary>
	/// �����̕`�揈��
	/// </summary>
	void EquipmentDraw();

	/// <summary>
	/// �x���̕`�揈��
	/// </summary>
	void RestDraw(bool rest);

	/// <summary>
	/// ���x���A�b�v�`��
	/// </summary>
	void LevelUpDraw(Player& player, CoreManager& core);

	/// <summary>
	/// �A�C�e���{�b�N�X�`�揈��
	/// </summary>
	void ItemBoxDraw();

	/// <summary>
	/// �����I����ʕ`�揈��
	/// </summary>
	void EquipmentDecisionDraw(ItemManager& item);

	/// <summary>
	/// �I������
	/// </summary>
	void End();

	/// <summary>
	/// ���ʂ��Ƃ�
	/// </summary>
	/// <returns>���ʂ�Ԃ�</returns>
	int GetVolume() { return m_volumeSize; }

	/// <summary>
	/// �ݒ�V�[�����o�����߂̔�����Ƃ�
	/// </summary>
	/// <returns>�����Ԃ�</returns>
	bool GetSettingScene() { return m_settingScene; }

	/// <summary>
	/// �ݒ�V�[�����o�����߂̔�������߂�
	/// </summary>
	/// <param name="scene">������Ƃ�</param>
	/// <returns>���ʂ�Ԃ�</returns>
	bool SetSettingScene(bool scene) { return m_settingScene = scene; }

	/// <summary>
	/// �������J��������Ƃ�
	/// </summary>
	/// <returns>�����Ԃ�</returns>
	bool GetEquipment() { return m_equipmentMenu; }

	/// <summary>
	/// �������J����������߂�
	/// </summary>
	/// <param name="re">������Ƃ�</param>
	/// <returns>���ʂ�Ԃ�</returns>
	bool SetEquipment(bool re) { return m_equipmentMenu = re; }

	/// <summary>
	/// �A�C�e����ʂ��J��������Ƃ�
	/// </summary>
	/// <returns>���ʂ�Ԃ�</returns>
	bool GetItem() { return m_itemMenu; }

	/// <summary>
	/// �A�C�e�����J����������߂�
	/// </summary>
	/// <param name="re">��������</param>
	/// <returns>���ʂ�Ԃ�</returns>
	bool Setitem(bool re) { return m_itemMenu = re; }

	/// <summary>
	/// �X�e�[�^�X�̃��x��Up��ʂ��J������
	/// </summary>
	bool GetLevel() { return m_statusLevel; }

	/// <summary>
	/// ������I�������ʑJ�ڔ�������
	/// </summary>
	/// <returns></returns>
	bool GetDecision() { return m_decisionEquipment; }

	/// <summary>
	/// ���̑������J���Ă��邩
	/// </summary>
	/// <returns></returns>
	SelectEquipment GetSelect() { return m_select; }

	/// <summary>
	/// ��ʂɖ߂锻����Ƃ�
	/// </summary>
	/// <returns>�����Ԃ�</returns>
	bool GetReturn() { return m_returnMenu; }

	/// <summary>
	/// ��ʂɖ߂锻������߂�
	/// </summary>
	/// <param name="re">������Ƃ�</param>
	/// <returns>���ʂ�Ԃ�</returns>
	bool SetReturn(bool re) { return m_returnMenu = re; }

	/// <summary>
	/// �^�C�g���ɖ߂锻����Ƃ�
	/// </summary>
	/// <returns>������Ƃ�</returns>
	bool GetTitle() { return m_titleMenu; }

private:
	int m_black;              //�����摜�i�[�ϐ�
	int m_white;              //�����摜�i�[�ϐ�
	int m_back;               //�����摜�i�[�ϐ�
	XINPUT_STATE m_xpad;      //�p�b�h����
	int m_menuSelect[5];      //���j���[�I�� 
	int m_selectX;            //�I���摜��X���W
	int m_selectY;            //�I���摜��Y���W
	int m_blackPal;           //�����摜�̃A���t�@�l�ۑ�
	int m_whitePal;           //�����摜�̃A���t�@�l�ۑ�
	int m_button;             //���������Ԃ𓾂�
	int m_thumb;              //���������Ԃ𓾂�
	int m_waitTime;           //���͂�҂��߂̎���
	int m_brightnessColor;    //���邳�̐F
	int m_bgmColor;           //���ʂ̐F
	int m_returnColor;        //�߂�̐F
	int m_brightColor[5];     //���邳�I���̐F
	int m_volumeColor[5];     //���ʑI���̐F
	int m_menuColor[5];       //���j���[�̐F
	int m_core;               //�\�L�p�ɃR�A���擾����
	bool m_one;               //�P������
	bool m_settingScene;      //�ݒ肷�邽�߂̉�ʂ��Ăԕϐ�
	bool m_brightness;        //���邳�ݒ�
	bool m_volume;            //���ʐݒ�
	int m_volumeSize;         //����
	bool m_equipmentMenu;     //�������j���[
	bool m_itemMenu;          //�A�C�e���{�b�N�X���j���[
	bool m_decisionEquipment; //�����I�����
	bool m_returnMenu;        //�߂郁�j���[
	bool m_titleMenu;         //�^�C�g�����j���[
	bool m_statusLevel;       //�X�e�[�^�X���J������
	bool m_blend;             //�u�����h����
	bool m_up;                //���x�����グ���邩�̔���

	//�I�񂾑���
	SelectEquipment m_select;

	//�I��ł���F��ς���
	SelectXY m_change;
	SelectXY m_equipmentColorPos;
	SelectXY m_selectObject;

	//�X�}�[�g�|�C���^
	std::shared_ptr<SEManager> pse = std::make_shared<SEManager>();
	std::shared_ptr<SelectManager> pselect = std::make_shared<SelectManager>();
};




