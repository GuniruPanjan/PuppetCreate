#include "CsvLoad.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <cassert>

CsvLoad* CsvLoad::m_instance = nullptr;

namespace
{
	//csv�f�[�^��,�ŕ�����𕪊�����
	std::vector<std::string> Split(std::string& input, char delimiter)
	{
		std::istringstream stream(input);
		std::string field;
		std::vector<std::string> result;
		while (getline(stream, field, delimiter))
		{
			result.push_back(field);
		}
		return result;
	}

	const std::string cFrontPathName = "Data/Csv/";
	const std::string cBackPathName = ".csv";
}

/// <summary>
/// �X�e�[�^�X��񃍁[�h
/// </summary>
/// <param name="data">�X�e�[�^�X���</param>
/// <param name="charaName">�L�����N�^�[�̖��O</param>
void CsvLoad::StatusLoad(CharacterBase::Status& data, const char* charaName)
{
	//�ꎞ�ۑ��pstring
	std::string strBuf;
	//�J���}�����ꎞ�ۑ��pstring
	std::vector<std::string> strConmaBuf;

	//�t�@�C���ǂݍ���
	std::ifstream cfs("Data/Csv/status.csv");
	if (!cfs)
	{
		assert(false);
		return;
	}

	//�����擾�ł������ǂ����̃t���O
	bool isGet = false;

	//�Ή���񂪓����Ă��邽�ߖ�������
	std::getline(cfs, strBuf);

	while (getline(cfs, strBuf))
	{
		//�擾������������J���}��؂�̔z��(���Q)�ɂ���
		strConmaBuf = Split(strBuf, ',');

		//0 : �L�����N�^�[��
		//1 : �̗�
		//2 : �X�^�~�i
		//3 : �U����
		//4 : �U����1
		//5 : �U����2
		//4 : �ؗ�
		//5 : �Z��
		//6 : �h���
		//7 : ���x
		//8 : �R�A

		//�w�肵���L�����N�^�[���ƈ�v����f�[�^������Ώ����擾����
		if (strConmaBuf[DataLoad::CsvStatus::name] == charaName)
		{
			isGet = true;
			data.s_hp = std::stoi(strConmaBuf[DataLoad::CsvStatus::hp]);
			data.s_stamina = std::stoi(strConmaBuf[DataLoad::CsvStatus::stamina]);
			data.s_attack = std::stoi(strConmaBuf[DataLoad::CsvStatus::attack]);
			data.s_attack1 = std::stoi(strConmaBuf[DataLoad::CsvStatus::attack1]);
			data.s_attack2 = std::stoi(strConmaBuf[DataLoad::CsvStatus::attack2]);
			data.s_muscle = std::stoi(strConmaBuf[DataLoad::CsvStatus::muscle]);
			data.s_skill = std::stoi(strConmaBuf[DataLoad::CsvStatus::skill]);
			data.s_defense = std::stoi(strConmaBuf[DataLoad::CsvStatus::defense]);
			data.s_speed = std::stoi(strConmaBuf[DataLoad::CsvStatus::speed]);
			data.s_core = std::stoi(strConmaBuf[DataLoad::CsvStatus::core]);
		}
	}

	if (!isGet)
	{
		assert(0 && "�w�肵���L�����N�^�[���̏����擾�ł��Ȃ�����");
	}

	return;
}

/// <summary>
/// �X�e�[�W�̏�񃍁[�h
/// </summary>
/// <param name="stageName">�X�e�[�W�̖��O</param>
/// <param name="pGenerateInfo">�L�����N�^�[�̖��O</param>
void CsvLoad::EnemyDataLoad(const char* stageName, std::list<std::shared_ptr<EnemyManager::EnemyGenerateInfo>>& pGenerateInfo)
{
	//�ꎞ�ۑ��pstring
	std::string strBuf;
	//�J���}�����ꎞ�ۑ��pstring
	std::vector<std::string> strConmaBuf;

	std::string csvName = cFrontPathName + stageName + cBackPathName;

	//�t�@�C���ǂݍ���
	std::ifstream ifs(csvName);
	if (!ifs)
	{
		assert(false);
		return;
	}

	//�ŏ��͑Ή��\��񂪓����Ă��邾���Ȃ̂Ŗ�������
	std::getline(ifs, strBuf);

	while (getline(ifs, strBuf))
	{
		//����������������J���}��؂�̔z��(���Q)�ɂ���
		strConmaBuf = Split(strBuf, ',');

		//0 : ��������G��ID
		//1 : ���������}�b�v
		//2 : X���W
		//3 : Y���W
		//4 : Z���W

		std::shared_ptr<EnemyManager::EnemyGenerateInfo> add = std::make_shared<EnemyManager::EnemyGenerateInfo>();
		add->enemyName = strConmaBuf[DataLoad::CsvEnemyStage::ID];
		add->mapNumber = std::stoi(strConmaBuf[DataLoad::CsvEnemyStage::map]);
		add->posX = std::stoi(strConmaBuf[DataLoad::CsvEnemyStage::x]);
		add->posY = std::stoi(strConmaBuf[DataLoad::CsvEnemyStage::y]);
		add->posZ = std::stoi(strConmaBuf[DataLoad::CsvEnemyStage::z]);

		pGenerateInfo.emplace_back(add);
	}

	return;
}

/// <summary>
/// �A�j���[�V������񃍁[�h
/// </summary>
/// <param name="charaName">�ǂݍ��݂����L�����N�^�[��</param>
/// <param name="anim">�i�[����A�j���[�V�������</param>
void CsvLoad::AnimDataLoad(std::string charaName, std::map<std::string, int>& anim)
{
	//�ꎞ�ۑ��pstring
	std::string strBuf;
	//�J���}�����ꎞ�ۑ��pstring
	std::vector<std::string> strConmaBuf;

	//�t�@�C���ǂݍ���
	std::ifstream ifs("Data/Csv/animation.csv");
	if (!ifs)
	{
		assert(false);
		return;
	}

	//�����擾�ł������ǂ����̃t���O
	bool isGet = false;

	//�A�j���[�V�������ۑ��pstring
	std::vector<std::string> animationNameBuf;

	//�A�j���[�V�����̖��O��ۑ�����
	std::getline(ifs, strBuf);
	animationNameBuf = Split(strBuf, ',');

	while (std::getline(ifs, strBuf))
	{
		//�O�̃��[�v�ł��łɎ擾�ł��Ă����烋�[�v���I���
		if (isGet)
		{
			break;
		}

		//�擾������������J���}��؂�̔z��(���Q)�ɂ���
		strConmaBuf = Split(strBuf, ',');

		//�ǂݍ��݂����L�����N�^�[���Ɠǂݍ��񂾃L�����N�^�[���������Ȃ�
		if (strConmaBuf[0] == charaName)
		{
			//�A�j���[�V���������i�[���Ă���
			isGet = true;
			for (int i = 1; i < strConmaBuf.size(); i++)
			{
				//�A�j���[�V����Idx��-1�ȉ��������ꍇ���݂��Ȃ����ߒǉ����Ȃ�
				if (std::stoi(strConmaBuf[i]) != -1)
				{
					//�������o�O
					anim[animationNameBuf[i]] = std::stoi(strConmaBuf[i]);
				}
			}
		}
	}

	if (!isGet)
	{
		assert(0 && "�w�肵���L�����N�^�[���̃A�j���[�V���������擾�ł��Ȃ�");
	}

	return;
}

/// <summary>
/// �A�C�e����񃍁[�h
/// </summary>
/// <param name="stageName">�X�e�[�W�̖��O</param>
/// <param name="pItemInfo"></param>
void CsvLoad::ItemDataLoad(const char* stageName, std::list<std::shared_ptr<ItemManager::ItemGenerateInfo>>& pGenerateInfo)
{
	//�ꎞ�ۑ��pstring
	std::string strBuf;
	//�J���}�����ꎞ�ۑ��pstring
	std::vector<std::string> strConmaBuf;

	std::string csvName = cFrontPathName + stageName + "Item" + cBackPathName;

	//�t�@�C���ǂݍ���
	std::ifstream ifs(csvName);
	if (!ifs)
	{
		assert(false);
		return;
	}

	//�ŏ��͑Ή��\��񂪓����Ă��邾���Ȃ̂Ŗ�������
	std::getline(ifs, strBuf);

	while (getline(ifs, strBuf))
	{
		//����������������J���}��؂�̔z��(���Q)�ɂ���
		strConmaBuf = Split(strBuf, ',');

		//0 : ��������A�C�e����ID
		//1 : ���������}�b�v
		//2 : X���W
		//3 : Y���W
		//4 : Z���W
		//5 : ���^�R�A
		//6 : ���^�R�A
		//7 : �S�~
		//8 : ������
		//9 : �c�񂾏�
		//10 : ���ʂ̊Z

		std::shared_ptr<ItemManager::ItemGenerateInfo> add = std::make_shared<ItemManager::ItemGenerateInfo>();
		add->itemName = strConmaBuf[DataLoad::CsvItemStage::ItemID];
		add->mapNumber = std::stoi(strConmaBuf[DataLoad::CsvItemStage::Itemmap]);
		add->posX = std::stoi(strConmaBuf[DataLoad::CsvItemStage::Itemx]);
		add->posY = std::stoi(strConmaBuf[DataLoad::CsvItemStage::Itemy]);
		add->posZ = std::stoi(strConmaBuf[DataLoad::CsvItemStage::Itemz]);
		add->SmallCore = std::stoi(strConmaBuf[DataLoad::CsvItemStage::ItemSmallCore]);
		add->MediumCore = std::stoi(strConmaBuf[DataLoad::CsvItemStage::ItemMediumCore]);
		add->Rubbish = std::stoi(strConmaBuf[DataLoad::CsvItemStage::ItemRubbish]);
		add->BlackSword = std::stoi(strConmaBuf[DataLoad::CsvItemStage::ItemBlackSword]);
		add->Distorted = std::stoi(strConmaBuf[DataLoad::CsvItemStage::ItemDistorted]);
		add->ArmorNormal = std::stoi(strConmaBuf[DataLoad::CsvItemStage::ItemArmorNormal]);

		pGenerateInfo.emplace_back(add);
	}

	return;
}

/// <summary>
/// ���b�Z�[�W�̏�񃍁[�h
/// </summary>
/// <param name="stageName"></param>
/// <param name="pGenerateInfo"></param>
void CsvLoad::MessageDataLoad(const char* stageName, std::list<std::shared_ptr<MessageManager::MessageGenerateInfo>>& pGenerateInfo)
{
	//�ꎞ�ۑ��pstring
	std::string strBuf;
	//�J���}�����ꎞ�ۑ��pstring
	std::vector<std::string> strConmaBuf;

	std::string csvName = cFrontPathName + "Message" + cBackPathName;

	//�t�@�C���ǂݍ���
	std::ifstream ifs(csvName);
	if (!ifs)
	{
		assert(false);
		return;
	}

	//�ŏ��͑Ή��\��񂪓����Ă��邾���Ȃ̂Ŗ�������
	std::getline(ifs, strBuf);

	while (getline(ifs, strBuf))
	{
		//����������������J���}��؂�̔z��(���Q)�ɂ���
		strConmaBuf = Split(strBuf, ',');

		//0 : ���������}�b�v
		//1 : X���W
		//2 : Y���W
		//3 : Z���W
		//4 : �������̔��f
		//5 : ��ڂ̌��t
		//6 : ��ڂ̌��t
		//7 : �O�ڂ̌��t

		std::shared_ptr<MessageManager::MessageGenerateInfo> add = std::make_shared<MessageManager::MessageGenerateInfo>();
		add->mapNumber = std::stoi(strConmaBuf[DataLoad::CsvMessageStage::messageMap]);
		add->posx = std::stoi(strConmaBuf[DataLoad::CsvMessageStage::messageX]);
		add->posy = std::stoi(strConmaBuf[DataLoad::CsvMessageStage::messageY]);
		add->posz = std::stoi(strConmaBuf[DataLoad::CsvMessageStage::messageZ]);
		add->official = std::stoi(strConmaBuf[DataLoad::CsvMessageStage::official]);
		add->one = std::stoi(strConmaBuf[DataLoad::CsvMessageStage::one]);
		add->two = std::stoi(strConmaBuf[DataLoad::CsvMessageStage::two]);
		add->three = std::stoi(strConmaBuf[DataLoad::CsvMessageStage::three]);

		pGenerateInfo.emplace_back(add);
	}

	return;
}

