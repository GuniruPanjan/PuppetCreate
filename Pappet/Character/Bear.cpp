#include "Bear.h"
#include "Ui/UI.h"
#include "Manager/EffectManager.h"
#include "Manager/SEManager.h"

namespace
{
	//�L�����N�^�[��
	constexpr const char* cCharacterName = "bear";
	//���f���p�X
	constexpr const char* cModelPath = "Data/Enemy/BearModel.mv1";
	//���f���̃T�C�Y�̊g�嗦
	constexpr float cModelSize = 0.7f;
	//���f���̒���
	constexpr float cCapsuleLen = 40.0f;
	//���f���̔��a
	constexpr float cCapsuleRadius = 25.0f;
	//���f���̍��W�����킹��
	constexpr float cModelPosY = 24.0f;
	//���S�I��
	bool cDead = false;
	//���S�����Ƃ��̃A�j���[�V����
	constexpr float cDeadFrame = 137.0f;
	//���G�͈�
	constexpr float cSearchRadius = 120.0f;
	//�U���͈�1
	constexpr float cAttackRadius1 = 40.0f;
	//�U���͈�2
	constexpr float cAttackRadius2 = 60.0f;
	//�U���͈�3
	constexpr float cAttackRadius3 = 130.0f;

	int a = 0;
	int b = 0;
	int c = 0;

	//�G����]�����Ȃ�
	bool cTurn = false;

	//�V���O���g��
	EffectManager& cEffect = EffectManager::GetInstance();

	bool cOne = false;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Bear::Bear() :
	EnemyBase(Collidable::Priority::High),
	m_attackPos(),
	m_walk(0)
{
	//�����蔻��̐ݒ�
	InitCollision(MyLibrary::LibVec3(0.0f, 2.0f, 0.0f), cCapsuleLen, cCapsuleRadius);
	//���f���̓ǂݍ���
	LoadModel(cModelPath);
	//�A�j���[�V������X�e�[�^�X���擾
	LoadData(cCharacterName);
	//���G�͈͂̐ݒ�
	m_searchRadius = cSearchRadius;

	//�A�j���[�V�����̔��菉����
	m_anim.s_attack = false;
	m_anim.s_moveflag = false;
	m_anim.s_hit = false;
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Bear::~Bear()
{
	//���������
	MV1DeleteModel(m_modelHandle);
}

/// <summary>
/// ������
/// </summary>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="posZ"></param>
/// <param name="physics"></param>
void Bear::Init(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
	//���
	m_pPhysics = physics;

	m_isExist = true;

	Collidable::Init(m_pPhysics);

	//�����N���X�̏�����
	InitRigidbody(posX, posY + cCapsuleRadius, posZ);

	//���S���W�̐ݒ�
	CalculationCenterPos(1.0f, cModelSize);

	//���f���̍��W��ݒ�
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	//���G��������铖���蔻����쐬
	InitSearch(cSearchRadius, 35.0f);

	//���f���̃T�C�Y�ݒ�
	MV1SetScale(m_modelHandle, VGet(cModelSize, cModelSize, cModelSize));

	//�A�j���[�V�����ݒ�
	m_nowAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Idle"]);
	m_nowAnimIdx = m_animIdx["Idle"];

	float totalAnimFrame = MV1GetAttachAnimTotalTime(m_modelHandle, m_nowAnimNo);

	m_anim.s_isDead = false;
	cDead = false;

	//�ő�HP���擾
	m_maxHP = m_status.s_hp;

	m_bossName = "�F�̘��S�l�`";
	m_subName = "H A R I B O";

	cOne = false;
	m_deadOne = false;

	m_walk = 0;

	m_bossjudg = true;
}

/// <summary>
/// �Q�[���̎d�l��ł̏���������
/// </summary>
/// <param name="posX">X���W</param>
/// <param name="posY">Y���W</param>
/// <param name="posZ">Z���W</param>
/// <param name="physics">�����N���X</param>
void Bear::GameInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pPhysics = physics;

	Collidable::Init(m_pPhysics);

	//���G��������铖���蔻����쐬
	InitSearch(cSearchRadius, 35.0f);

	//���S���W�̐ݒ�
	CalculationCenterPos(1.0f, cModelSize);
	
	//�����N���X�̏�����
	InitRigidbody(posX, posY + cCapsuleRadius, posZ);

	//���f���̍��W��ݒ�
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	//���f���̃T�C�Y�ݒ�
	MV1SetScale(m_modelHandle, VGet(cModelSize, cModelSize, cModelSize));

	//�A�j���[�V�����ݒ�
	m_nowAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Idle"]);
	m_nowAnimIdx = m_animIdx["Idle"];

	m_anim.s_isDead = false;
	cDead = false;

	//�ő�HP���擾
	m_maxHP = m_status.s_hp;

	m_bossName = "�F�̘��S�l�`";
	m_subName = "H  A  R  I  B  O";

	cOne = false;
	m_deadOne = false;

	m_walk = 0;

	m_bossjudg = true;
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="playerPos"></param>
/// <param name="isChase"></param>
void Bear::Update(MyLibrary::LibVec3 playerPos, MyLibrary::LibVec3 shieldPos, bool isChase, SEManager& se, std::shared_ptr<MyLibrary::Physics> physics)
{
	//����̊p�x�����߂�
	m_viewAngle = 30.0f;
	//����̋��������߂�
	m_viewDistance = 500.0f;

	//�A�j���[�V�����̍X�V
	if (!cDead)
	{
		m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);
	}
	//���S�����Ƃ��̃A�j���[�V�����X�V
	else if (cDead && m_nowFrame <= cDeadFrame)
	{
		m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);
	}

	UpdateAnimationBlend();

	DistanceUpdate(playerPos, shieldPos);
	AttackDistance();

	//�ړ�����
	MoveUpdate();

	//�^�[�Q�b�g���
	TargetNow();

	//�U�����Ă��Ȃ��Ƃ�
	if (!m_anim.s_attack)
	{
		if (!cOne)
		{
			InitAttack(0.0f);
			InitAttackDamage(0.0f);

			cOne = true;
		}

	}

	//�����Ă��鎞
	if (m_anim.s_moveflag)
	{
		m_walk++;

		if (m_walk >= 30)
		{
			//����SE�Đ�
			PlaySoundMem(se.GetBossWalkSE(), DX_PLAYTYPE_BACK, true);

			m_walk = 0;
		}
	}
	else
	{
		m_walk = 0;
	}


	//�U�����󂯂���
	if (m_isEnterHit)
	{
		m_status.s_hp -= m_col->GetAttack() - m_status.s_defense;
		//Hit�G�t�F�N�g
		cEffect.EffectCreate("Hit", VGet(rigidbody.GetPos().x, rigidbody.GetPos().y + 30.0f, rigidbody.GetPos().z));
		//HitSE�Đ�
		PlaySoundMem(se.GetHitSE(), DX_PLAYTYPE_BACK, true);
	}
	//�v���C���[���{�X�����ɓ�������
	if (m_isBossDiscovery && !cDead)
	{
		Action(playerPos, isChase, se);
	}

	TriggerUpdate();
	HitTriggerUpdate();

	//����̍X�V
	MyLibrary::LibVec3 centerPos = rigidbody.GetPos();
	m_pSearch->Update(centerPos);

	//���񂾎�
	if (m_status.s_hp <= 0.0f)
	{
		//�A�j���[�V������������
		m_anim.s_attack = false;
		m_anim.s_hit = false;
		m_anim.s_moveflag = false;

		Death();

		if (m_nowFrame == 60)
		{
			//���SSE�Đ�
			PlaySoundMem(se.GetDiedSE(), DX_PLAYTYPE_BACK, true);
		}

		cDead = true;
		m_isBossDead = true;
	}
	else
	{
		m_isBossDead = false;
	}

}

/// <summary>
/// �{�X�̍s��
/// </summary>
/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
/// <param name="isChase">�v���C���[</param>
void Bear::Action(MyLibrary::LibVec3 playerPos, bool isChase, SEManager& se)
{
	//�G���v���C���[�̈ʒu�ɂ���ĕ�����␳����
	float Cx = m_modelPos.x - playerPos.x;
	float Cz = m_modelPos.z - playerPos.z;

	m_correctionAngle = atan2f(Cx, Cz);
	//m_correctionAngle = NormalizeAngle(m_correctionAngle);

	if (m_correctionAngle >= 3.0f)
	{
		if (m_enemyAnim.s_turnRight)
		{
			int a = 1;
		}

		cTurn = true;
	}
	else if (m_correctionAngle <= -3.0f)
	{
		if (m_enemyAnim.s_turnLeft)
		{
			int a = 1;
		}

		cTurn = true;
	}
	else
	{
		cTurn = false;
		
	}
	//����
	if (!IsPlayerInView(playerPos))
	{
		a = 1;
	}
	else
	{
		a = 0;
	}

	//�E��
	if (IsPlayerOnRight(playerPos))
	{
		b = 1;
	}
	else
	{
		b = 0;
	}

	//����
	if (IsPlayerOnLeft(playerPos))
	{
		c = 1;
	}
	else
	{
		c = 0;
	}

	
	//�����Ă��鎞�͉�]���Ȃ��Ȃ���
	if (!cTurn)
	{
		//���ɉ�]����
		if (m_angle > m_correctionAngle + 0.8f)
		{
			m_enemyAnim.s_turnLeft = true;
			m_enemyAnim.s_turnRight = false;
			m_anim.s_moveflag = false;
		}
		//�E�ɉ�]����
		if (m_angle < m_correctionAngle - 0.8f)
		{
			m_enemyAnim.s_turnRight = true;
			m_enemyAnim.s_turnLeft = false;
			m_anim.s_moveflag = false;
		}
		
	}
	

	//����肵�Ă���Ƃ�
	if (m_enemyAnim.s_turnLeft)
	{
		//�E��]������
		if (m_angle < m_correctionAngle - 0.5f)
		{
			m_enemyAnim.s_turnRight = true;
			m_enemyAnim.s_turnLeft = false;
		}
	}
	//�E��肵�Ă���Ƃ�
	else if (m_enemyAnim.s_turnRight)
	{
		//����]������
		if (m_angle > m_correctionAngle + 0.5f)
		{
			m_enemyAnim.s_turnLeft = true;
			m_enemyAnim.s_turnRight = false;
		}
	}

	//�U�����ĂȂ���
	if (!m_anim.s_attack)
	{
		if (!m_enemyAnim.s_turnLeft && !m_enemyAnim.s_turnRight)
		{
			//���������߂�
			AngleUpdate(playerPos);
		}
	}

	//�U�����ĂȂ���
	if (!m_anim.s_attack)
	{
		//�����Ȃ��悤�ɂ���
		m_moveVec = MyLibrary::LibVec3(0.0f, 0.0f, 0.0f);

		//����]������
		if (m_enemyAnim.s_turnLeft)
		{
			m_nowAnimIdx = m_animIdx["LeftWalk"];
			ChangeAnim(m_nowAnimIdx, m_animOne[6], m_animOne);

			//�����
			if (m_angle > m_correctionAngle + 0.8f)
			{
				m_angle -= 0.05f;
			}

		}
		//�E��]������
		if (m_enemyAnim.s_turnRight)
		{
			m_nowAnimIdx = m_animIdx["RightWalk"];
			ChangeAnim(m_nowAnimIdx, m_animOne[7], m_animOne);

			//�E���
			if (m_angle < m_correctionAngle - 0.8f)
			{
				m_angle += 0.05f;
			}

		}
	}

	//�v���C���[���͈͊O��������
	if (!m_pSearch->GetIsStay() && !m_enemyAnim.s_turnLeft && !m_enemyAnim.s_turnRight)
	{
		WalkUpdate("Walk", 2);

		//�U�����ĂȂ���
		if (!m_anim.s_attack)
		{
			//�����A�j���[�V����
			m_anim.s_moveflag = true;
			//�X�s�[�h
			m_status.s_speed = 0.01f;

			m_move = VScale(m_difPlayer, m_status.s_speed);

			//�ړ�����
			m_moveVec = MyLibrary::LibVec3(m_move.x, m_move.y, m_move.z);
		}
		
	}
	//�˒������ɓ�����
	else if (m_pSearch->GetIsStay() && !m_enemyAnim.s_turnLeft && !m_enemyAnim.s_turnRight)
	{
		//�����_���s����0���o����
		if (m_randomAction == 0)
		{
			m_anim.s_moveflag = false;
			m_anim.s_attack = true;

			AttackUpdate("Attack1", 3);
			//�U����
			//m_status.s_attack = 30.0f;
		}
		//�����_���s����1���o����
		else if (m_randomAction == 1)
		{
			m_anim.s_moveflag = false;
			m_anim.s_attack = true;

			AttackUpdate("Attack2", 4);
			//�U����
			//m_status.s_attack = 50.0f;

		}
		//�����_���s����2���o����
		else if (m_randomAction == 2)
		{
			m_anim.s_moveflag = false;
			m_anim.s_attack = true;

			AttackUpdate("Attack3", 5);
			//�U����
			//m_status.s_attack = 80.0f;
		}

		m_moveVec = MyLibrary::LibVec3(0.0f, 0.0f, 0.0f);
	}

	//�U���ł̔���
	if (m_anim.s_attack)
	{
		//�����_����0���o����
		if (m_randomAction == 0)
		{
			//�U���̏�����
			if (m_nowFrame == 5.0f)
			{
				//�U���̏�����
				InitAttack(cAttackRadius1);

				InitAttackDamage(m_status.s_attack);
			}
			else if (m_nowFrame > 5.0f)
			{
				//�U������̍X�V
				m_attackPos = MyLibrary::LibVec3(rigidbody.GetPos().x + sinf(m_angle) * -75.0f, rigidbody.GetPos().y, rigidbody.GetPos().z - cosf(m_angle) * 75.0f);
				m_pAttack->Update(m_attackPos);
			}

			//�A�j���[�V�����t���[�����ɍU��������o��
			if (m_nowFrame == 7.0f)
			{
				//�U��SE�Đ�
				PlaySoundMem(se.GetBossAttackSE1(), DX_PLAYTYPE_BACK, true);

				InitAttackUpdate(m_status.s_attack);
			}
			else if (m_nowFrame >= 12.0f)
			{
				//��������Z�b�g
				m_pAttack->CollisionEnd();
			}

		}
		//�����_����1���o����
		else if (m_randomAction == 1)
		{
			//�U���̏�����
			if (m_nowFrame == 5.0f)
			{
				//�U���̏�����
				InitAttack(cAttackRadius2);

				InitAttackDamage(m_status.s_attack1);
			}
			else if (m_nowFrame > 5.0f)
			{
				//�U������̍X�V
				m_attackPos = MyLibrary::LibVec3(rigidbody.GetPos().x + sinf(m_angle) * -75.0f, rigidbody.GetPos().y, rigidbody.GetPos().z - cosf(m_angle) * 75.0f);
				m_pAttack->Update(m_attackPos);
			}

			//�U�������܂ł̓v���C���[������
			if (m_nowFrame > 0.1f && m_nowFrame < 38.0f)
			{
				AngleUpdate(playerPos);
			}
			//�A�j���[�V�����t���[�����ɍU��������o��
			else if (m_nowFrame == 38.0f)
			{
				//�U��SE�Đ�
				PlaySoundMem(se.GetBossAttackSE2(), DX_PLAYTYPE_BACK, true);

				InitAttackUpdate(m_status.s_attack1);
			}
			else if (m_nowFrame >= 45.0f)
			{
				//��������Z�b�g
				m_pAttack->CollisionEnd();
			}
		}
		//�����_����2���o����
		else if (m_randomAction == 2)
		{
			//�U���̏�����
			if (m_nowFrame == 5.0f)
			{
				//�U���̏�����
				InitAttack(cAttackRadius3);

				InitAttackDamage(m_status.s_attack2);

				//���KSE�Đ�
				PlaySoundMem(se.GetBossVoiceSE(), DX_PLAYTYPE_BACK, true);
			}
			else if (m_nowFrame > 5.0f)
			{
				//�U������̍X�V
				m_attackPos = MyLibrary::LibVec3(rigidbody.GetPos().x, rigidbody.GetPos().y, rigidbody.GetPos().z);
				m_pAttack->Update(m_attackPos);
			}

			//�G�t�F�N�g���o��
			if (m_nowFrame == 25.0f)
			{
				//�U��SE�Đ�
				PlaySoundMem(se.GetBossAttackSE3(), DX_PLAYTYPE_BACK, true);

				cEffect.EffectCreate("BearLance", VGet(rigidbody.GetPos().x, rigidbody.GetPos().y - 25.0f, rigidbody.GetPos().z));
			}

			//�A�j���[�V�����t���[�����ɍU��������o��
			if (m_nowFrame == 58.0f)
			{
				InitAttackUpdate(m_status.s_attack2);
			}
			else if (m_nowFrame >= 68.0f)
			{
				//��������Z�b�g
				m_pAttack->CollisionEnd();
			}
		}
	}

	//�A�j���[�V�������I�������p�x������
	if (m_enemyAnim.s_turnLeft && m_isAnimationFinish && !m_anim.s_attack && !m_anim.s_moveflag)
	{
		m_angle = m_correctionAngle;
		m_enemyAnim.s_turnLeft = false;
	}
	//�A�j���[�V�������I�������p�x������
	if (m_enemyAnim.s_turnRight && m_isAnimationFinish && !m_anim.s_attack && !m_anim.s_moveflag)
	{
		m_angle = m_correctionAngle;
		m_enemyAnim.s_turnRight = false;
	}

	//�A�j���[�V�������I���x�Ƀ����_���ȍs�����s��
	if (m_isAnimationFinish && m_anim.s_attack)
	{
		m_anim.s_attack = false;

		//�߂��Ƃ��̃����_���s��
		if (m_difPSize <= 80.0f)
		{
			m_randomAction = GetRand(2);
			//m_randomAction = 1;
		}
		//�߂��Ȃ��Ƃ��̃����_���s��
		else
		{
			m_randomAction = GetRand(1) + 1;
			//m_randomAction = 1;
		}
		
	}

	if (!m_anim.s_attack)
	{
		InitAttackDamage(0.0f);
	}

}

/// <summary>
/// �`�揈��
/// </summary>
void Bear::Draw(UI& ui)
{
	//�����蔻����W���擾���ă��f���̕`����W��ݒ肷��
	SetDrawModelPos(cModelPosY);
	//3D���f���̉�]�n���Z�b�g����
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle, 0.0f));
	//���f���̕`��
	MV1DrawModel(m_modelHandle);

	//DrawFormatString(200, 400, 0xffffff, "m_playerHit : %d", m_isPlayerHit);

	if (m_isBossDiscovery && !cDead)
	{
		ui.BossHPDraw(m_status.s_hp, m_maxHP, m_bossName, m_subName);
	}

	DrawFormatString(200, 300, 0xffffff, "%d", a);
	DrawFormatString(200, 400, 0xffffff, "�E�� : %d", b);
	DrawFormatString(200, 500, 0xffffff, "���� : %d", c);

#if false
	DrawFormatString(200, 300, 0xffffff, "m_angle : %f", m_angle);
	DrawFormatString(200, 350, 0xffffff, "m_correctionAngle : %f", m_correctionAngle);
	DrawFormatString(200, 400, 0xffffff, "m_left : %d", m_enemyAnim.s_turnLeft);
	DrawFormatString(200, 450, 0xffffff, "m_right : %d", m_enemyAnim.s_turnRight);
#endif
}
