#pragma once
#include <memory>
#include <map>
#include <string>
#include <List>
#include "Library/MyLibrary.h"
#include "Ui/Message.h"

class GameManager;
class Player;

/// <summary>
/// ���b�Z�[�W���Ǘ�����N���X
/// </summary>
class MessageManager
{
public:
	//���b�Z�[�W�̐������
	struct MessageGenerateInfo
	{
		int mapNumber;      //�}�b�v�̔ԍ�
		int posx;           //x���W
		int posy;           //y���W
		int posz;           //z���W

		bool official;      //�������b�Z�[�W��

		int one;            //��ڂ̌��t
		int two;            //��ڂ̌��t
		int three;          //�O�ڂ̌��t

		bool isCreated = false;     //�������ꂽ���ǂ���    
	};

public:
	//�R���X�g���N�^
	MessageManager();
	//�f�X�g���N�^
	virtual ~MessageManager();
	//����������
	void Init(const char* stageName);
	//�Q�[�����ł̏���������
	void GameInit(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager);
	//�X�V����
	void Update(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, Player& player);

	//�`�揈��
	void Draw();

	//�I������
	void End();

	//���b�Z�[�W��ǂ߂邩�Ԃ�
	bool GetMessagePick() { return m_messagePick; }
	//���b�Z�[�W��\�������Ԃ�
	bool GetDraw();

private:
	//���b�Z�[�W����
	void CreateMessage(float posx, float posy, float posz, bool official, int one, int two, int three, std::shared_ptr<MyLibrary::Physics> physics);
private:
	//���b�Z�[�W�̊Ǘ�
	std::shared_ptr<Message> m_pMessage;
	//���b�Z�[�W�̐������
	std::list<std::shared_ptr<MessageGenerateInfo>> m_pGenerateInfo;

	//�X�e�[�W��
	const char* m_stageName;

	//�X�e�[�W���̃��b�Z�[�W�̐�����
	std::unordered_map<int, int> m_MessageGenerationCountPerOneMap;

	bool m_messagePick;         //���b�Z�[�W��ǂ�
};

