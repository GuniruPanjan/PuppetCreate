#include "EnemyManager.h"
#include "Character/Immortal.h"
#include "Character/Bear.h"
#include "External/CsvLoad.h"
#include "GameManager.h"
#include "Manager/CoreManager.h"
#include "Manager/SEManager.h"

namespace
{
	int cI = 0;

	int cEnemy = 0;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
EnemyManager::EnemyManager() :
	m_stageName(""),
	m_frontEnemyHp(0),
	m_frontEnemyMaxHp(0)
{
	
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
EnemyManager::~EnemyManager()
{
	//�z��̑S�v�f�폜
	m_pEnemys.clear();
	m_pGenerateInfo.clear();
	m_enemyPos.clear();
	m_enemyTarget.clear();
	m_damage.clear();
	m_enemyAttackHit.clear();
	m_dropCore.clear();
	m_enemyIsDead.clear();
}

/// <summary>
/// ������
/// </summary>
/// <param name="stageName">�X�e�[�W���O</param>
void EnemyManager::Init(const char* stageName)
{
	m_pGenerateInfo.clear();
	m_pEnemys.clear();

	m_stageName = stageName;

	//�G���������擾����
	CsvLoad::GetInstance().EnemyDataLoad(m_stageName, m_pGenerateInfo);

	//csv���Ń}�b�v���w�肵�Ă��邽�߃}�b�v�̕␳������
	for (auto& generate : m_pGenerateInfo)
	{
		//�}�b�v��1�Ȃ�
		if (generate->mapNumber == 1)
		{
			//���̃}�b�v�̏o������G�̐����X�V����
			int num = m_enemyGenerationCountPerOneMap[generate->mapNumber];
			num++;
			m_enemyGenerationCountPerOneMap[generate->mapNumber];
		}
	}
}

/// <summary>
/// �Q�[���̎d�l��ł̏�����
/// </summary>
/// <param name="physics">�����N���X</param>
/// <param name="gameManager">�Q�[���}�l�W���[</param>
/// <param name="init">���������邩�ǂ���</param>
void EnemyManager::GameInit(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, bool init)
{
	//�G�̓����蔻��ƃ��f���폜
	for (auto& enemy : m_pEnemys)
	{
		//�����Ă���z
		if (!enemy->GetIsDead())
		{
			//�����Ă������폜
			enemy->Finalize(physics);
		}

		enemy->End();
	}

	if (init)
	{
		m_pEnemys.clear();
	}


	auto thisMapName = gameManager->GetThisMapName();

	if (thisMapName == 1 || thisMapName == 2 || thisMapName == 3 ||
		thisMapName == 4 || thisMapName == 5)
	{
		//�G���������܂킵��
		for (auto& generate : m_pGenerateInfo)
		{
			//���̃}�b�v����v���Ă���Ƃ�
			if (generate->mapNumber == thisMapName)
			{
				//�����ς݂̃L����������������
				if (generate->isCreated)
				{
					EnemyInit(generate->posX, generate->posY, generate->posZ, generate->enemyName, physics);
				}
			}
		}
	}
}

/// <summary>
/// �X�V
/// </summary>
/// <param name="physics">�����N���X</param>
/// <param name="gameManager">�Q�[���}�i�W���[</param>
/// <param name="playerPos">�v���C���[�|�W�V����</param>
/// <param name="playerDir">�v���C���[�̕���</param>
/// <param name="isPlayerChase">�v���C���[�𔭌��������ǂ���</param>
void EnemyManager::Update(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, CoreManager& core, MyLibrary::LibVec3 playerPos, MyLibrary::LibVec3 playerDir, MyLibrary::LibVec3 shieldPos, bool isPlayerChase, SEManager& se, bool init)
{
	m_enemyPos.clear();
	m_enemyTarget.clear();
	m_damage.clear();
	m_enemyAttackHit.clear();
	m_dropCore.clear();
	m_bossJudg.clear();
	m_enemyIsDead.clear();


	//���̃}�b�v���ǂ̃}�b�v���擾����
	auto thisMapName = gameManager->GetThisMapName();

	if (thisMapName == 0 || thisMapName == 1 || thisMapName == 2 ||
		thisMapName == 3 || thisMapName == 4 || thisMapName == 5)
	{
		//�G���������܂킵��
		for (auto& generate : m_pGenerateInfo)
		{
			//���̃}�b�v����v���Ă���Ƃ�
			if (generate->mapNumber == thisMapName)
			{
				//�����ς݂łȂ����
				if (!generate->isCreated)
				{
					if (generate->enemyName != "bear")
					{
						//�����ς݂ɂ��ēG�𐶐�����
						generate->isCreated = true;
						CreateEnemy(generate->posX, generate->posY, generate->posZ, generate->enemyName, physics);
					}
					else if (!gameManager->GetEndBoss().sBear)
					{
						//�����ς݂ɂ��ă{�X�𐶐�����
						generate->isCreated = true;
						CreateEnemy(generate->posX, generate->posY, generate->posZ, generate->enemyName, physics);
					}
					
				}
			}
		}
	}
	
	//�}�b�v��0�ȊO���Ɠ�����
	if (thisMapName != 0)
	{
		//�G�̍X�V����
		for (auto& enemy : m_pEnemys)
		{
			//��������X�V
			physics->CheckEnemyUpdate();

			enemy->Update(playerPos, shieldPos, isPlayerChase, se, physics);

			m_enemyPos.emplace_back(enemy->GetPos());
			m_enemyTarget.emplace_back(enemy->GetTarget());
			m_damage.emplace_back(enemy->GetAttack());
			m_enemyAttackHit.emplace_back(enemy->GetPlayerHit());
			m_dropCore.emplace_back(enemy->GetDropCore());
			m_bossJudg.emplace_back(enemy->GetBossJudg());
			m_enemyIsDead.emplace_back(enemy->GetIsDead());

			if (enemy->GetIsDead())
			{
				//��񂾂��s��
				if (enemy->GetOne())
				{
					cEnemy++;

					//�R�A�擾
					core.Core(enemy->GetDropCore());

					enemy->SetOne(false);
				}
			}
			
		}
	}

}

/// <summary>
/// �`��
/// </summary>
void EnemyManager::Draw(UI& ui)
{
	//�G��`�悷��
	for (auto& enemy : m_pEnemys)
	{
		enemy->Draw(ui);
	}

	//DrawFormatString(200, 500, 0xffffff, "%d", cEnemy);
}

/// <summary>
/// �I������
/// </summary>
void EnemyManager::End()
{
	for (auto& enemy : m_pEnemys)
	{
		enemy->End();
	}
}

const int EnemyManager::GetHP()
{
	for (auto& enemy : m_pEnemys)
	{
		return enemy->GetHp();
	}
}

const int EnemyManager::GetMaxHP()
{
	for (auto& enemy : m_pEnemys)
	{
		return enemy->GetMaxHp();
	}
}

/// <summary>
/// �{�X�̕����ɓ�������
/// </summary>
/// <param name="set"></param>
/// <returns></returns>
bool EnemyManager::SetBossRoom(bool set)
{
	return bear->SetBossRoom(set);
}

/// <summary>
/// �{�X�����񂾂��̔���
/// </summary>
/// <returns></returns>
bool EnemyManager::GetBossDead()
{
	return bear->GetBossDead();
}

/// <summary>
/// �G�̐���
/// </summary>
/// <param name="posX">X���W</param>
/// <param name="posY">Y���W</param>
/// <param name="posZ">Z���W</param>
/// <param name="name">�L�����N�^�[��</param>
/// <param name="physics">�����|�C���^</param>
void EnemyManager::CreateEnemy(float posX, float posY, float posZ, std::string name, std::shared_ptr<MyLibrary::Physics> physics)
{
	if (name == "Immortal")
	{
		immortal = std::make_shared<Immortal>();
		immortal->Init(posX, posY, posZ, physics);
		m_pEnemys.emplace_back(immortal);
	}
	if (name == "bear")
	{
		bear = std::make_shared<Bear>();
		bear->Init(posX, posY, posZ, physics);
		m_pEnemys.emplace_back(bear);
	}
}

/// <summary>
/// �G�̃Q�[���̎d�l��ł̏�����
/// </summary>
/// <param name="posX">X���W</param>
/// <param name="posY">Y���W</param>
/// <param name="posZ">Z���W</param>
/// <param name="name">�L�����N�^�[��</param>
/// <param name="physics">�����|�C���^</param>
void EnemyManager::EnemyInit(float posX, float posY, float posZ, std::string name, std::shared_ptr<MyLibrary::Physics> physics)
{
	if (name == "Immortal")
	{
		immortal = std::make_shared<Immortal>();
		immortal->GameInit(posX, posY, posZ, physics);
		m_pEnemys.emplace_back(immortal);

	}
	if (name == "bear")
	{
		bear = std::make_shared<Bear>();
		bear->GameInit(posX, posY, posZ, physics);
		m_pEnemys.emplace_back(bear);
	}
}
