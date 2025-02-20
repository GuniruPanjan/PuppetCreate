#pragma once
#include "Character/CharacterBase.h"
#include "Manager/EnemyManager.h"
#include "Manager/ItemManager.h"
#include "Manager/MessageManager.h"
#include <string>
#include <memory>

namespace DataLoad
{
	//�X�e�[�^�X���񋓌^
	enum CsvStatus : int
	{
		name,      //�L�����N�^�[��
		hp,        //�̗�
		stamina,   //�X�^�~�i
		attack,    //�U����
		attack1,   //�U����2
		attack2,   //�U����3
		muscle,    //�ؗ�
		skill,     //�Z��
		defense,   //�h���
		speed,     //���x
		core,      //�h���b�v�R�A
	};

	//�G�̐����ꏊ�̗񋓌^
	enum CsvEnemyStage : int
	{
		ID,         //�GID
		map,        //�G�̏o���}�b�v
		x,          //�G��X���W
		y,          //�G��Y���W
		z,          //�G��Z���W
	};

	//�A�C�e���̐����ꏊ�̗񋓌^
	enum CsvItemStage : int
	{
		ItemID,         //�A�C�e����ID
		Itemmap,        //�A�C�e���̏o���}�b�v
		Itemx,          //�A�C�e����X���W
		Itemy,          //�A�C�e����Y���W
		Itemz,          //�A�C�e����Z���W

		//�A�C�e������
		ItemSmallCore,   //���^�R�A
		ItemMediumCore,  //���^�R�A
		ItemRubbish,     //�S�~
		ItemBlackSword,  //������
		ItemDistorted,   //�c�񂾏�
		ItemArmorNormal, //���ʂ̊Z
	};

	//���b�Z�[�W�̐����ꏊ�̗񋓌^
	enum CsvMessageStage : int
	{
		messageMap,          //�o���}�b�v
		messageX,            //x���W
		messageY,            //y���W
		messageZ,            //z���W
		official,     //�������̔��f
		one,          //�ŏ��̌��t
		two,          //��ڂ̌��t
		three,        //�O�ڂ̌��t
	};
}

/// <summary>
/// Csv��ǂݍ��ރN���X
/// </summary>
class CsvLoad
{
private:
	//�V���O���g���ɂ��邽�߃R���X�g���N�^��private�ɒu��
	CsvLoad() {};

public:
	virtual ~CsvLoad() {};

	//�R�s�[�R���X�g���N�^���֎~����
	CsvLoad(const CsvLoad&) = delete;
	CsvLoad& operator = (const CsvLoad&) = delete;
	CsvLoad(CsvLoad&&) = delete;
	CsvLoad& operator = (const CsvLoad&&) = delete;

	/// <summary>
	/// GetInstance()��ʂ����Q�Ƃ��炵�����p�ł��Ȃ�
	/// </summary>
	/// <returns></returns>
	static CsvLoad& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new CsvLoad;
		}

		return *m_instance;
	}

	//���������[�N��h������
	static void Destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:
	//�X�e�[�^�X��񃍁[�h
	void StatusLoad(CharacterBase::Status& data, const char* charaName);

	//�G������񃍁[�h
	void EnemyDataLoad(const char* stageName, std::list<std::shared_ptr<EnemyManager::EnemyGenerateInfo>>& pGenerateInfo);

	//�A�j���[�V������񃍁[�h
	void AnimDataLoad(std::string charaName, std::map<std::string, int>& anim);

	//�A�C�e��������񃍁[�h
	void ItemDataLoad(const char* stageName, std::list<std::shared_ptr<ItemManager::ItemGenerateInfo>>& pGenerateInfo);

	//���b�Z�[�W������񃍁[�h
	void MessageDataLoad(const char* stageName, std::list<std::shared_ptr<MessageManager::MessageGenerateInfo>>& pGenerateInfo);

private:
	//Singleton�̃|�C���^���v���O�����N�����Ɉ�����悤�ɂ���
	static CsvLoad* m_instance;
};



