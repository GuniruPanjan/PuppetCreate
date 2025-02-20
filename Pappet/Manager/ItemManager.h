#pragma once
#include <memory>
#include <map>
#include <string>
#include <List>
#include "Library/MyLibrary.h"
#include "Item/ItemBase.h"

class GameManager;
class Armor;
class Shield;
class Weapon;
class Tool;

/// <summary>
/// �A�C�e�����Ǘ�����N���X
/// </summary>
class ItemManager
{
public:
	//�A�C�e���̍\����
	struct Item
	{
		int SmallCore = 0;       //���^�R�A
		int MediumCore = 0;      //���^�R�A
		int Rubbish = 0;         //�S�~
		int BlackSword = 0;      //������
		int Distorted = 0;       //�c�񂾏�
		int ArmorNormal = 0;     //���ʂ̊Z
	}m_item;

	//UI�\���p�̍\����
	struct UIItem
	{
		int u_SmallCore = 0;     //���^�R�A
		int u_MediumCore = 0;    //���^�R�A
		int u_Rubbish = 0;       //�S�~
		int u_BlackSword = 0;    //������
		int u_Distorted = 0;     //�c�񂾏�
		int u_ArmorNormal = 0;   //���ʂ̊Z
	}m_uiItem;

	//�A�C�e���������
	struct ItemGenerateInfo
	{
		std::string itemName;      //�A�C�e���̖��O
		int mapNumber;             //�}�b�v�̔ԍ�
		int posX;                  //�A�C�e����X���W
		int posY;                  //�A�C�e����Y���W
		int posZ;                  //�A�C�e����Z���W
		
		//�A�C�e������
		int SmallCore = 0;       //���^�R�A
		int MediumCore = 0;      //���^�R�A
		int Rubbish = 0;         //�S�~
		int BlackSword = 0;      //������
		int Distorted = 0;       //�c�񂾏�
		int ArmorNormal = 0;     //���ʂ̊Z

		bool isCreated = false;    //�����ς݂��ǂ���
		bool isPickUp = false;     //�E��ꂽ���ǂ���
	};

public:
	//�R���X�g���N�^
	ItemManager();
	//�f�X�g���N�^
	virtual ~ItemManager();
	//����������
	void Init(const char* stageName);
	//�Q�[�����ł̏���������
	void GameInit(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager);
	//�X�V����
	void Update(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, bool taking);

	//�`�揈��
	void Draw();

	//�I������
	void End();

	//�A�C�e�����E���邩��Ԃ�
	bool GetItemPick() { return m_itemPick; }
	//�A�C�e��������Ԃ�Ԃ�
	Item GetItem() { return m_item; }
private:
	//�A�C�e���̐���
	void CreateItem(float posX, float posY, float posZ, std::string name, std::shared_ptr<MyLibrary::Physics> physics);
	//���̃A�C�e�����̔��f
	void CheckItem(std::string name, int SmallCore, int MediumCore, int Rubbish, int BlackSword, int Distorted, int ArmorNormal);
private:
	//�A�C�e���̊Ǘ�
	std::list<std::shared_ptr<ItemBase>> m_pItems;
	//�A�C�e���̐������
	std::list<std::shared_ptr<ItemGenerateInfo>> m_pGenerateInfo;

	std::shared_ptr<Armor> m_parmor;
	std::shared_ptr<Shield> m_pshield;
	std::shared_ptr<Weapon> m_pweapon;
	std::shared_ptr<Tool> m_ptool;

	//�X�e�[�W��
	const char* m_stageName;

	//�X�e�[�W���̃A�C�e���̐�����
	std::unordered_map<int, int> m_itemGenerationCountPerOneMap;

	bool m_itemPick;      //�A�C�e�������邩

};

