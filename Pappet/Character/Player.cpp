#include "Player.h"
#include "Manager/EffectManager.h"
#include "Manager/HandleManager.h"
#include "Manager/EnemyManager.h"
#include "Manager/CoreManager.h"
#include "External/CsvLoad.h"
#include "Item/Weapon.h"
#include "Item/Shield.h"
#include "Item/Armor.h"
#include "Item/Tool.h"
#include "Manager/GameManager.h"
#include "Manager/SEManager.h"

#include <cassert>

namespace
{
	//���f���T�C�Y
	constexpr float cModelSizeScale = 0.4f;
	//�A�j���[�V�����̐؂�ւ��ɂ�����t���[����
	constexpr float cAnimChangeFrame = 5.0f;
	constexpr float cAnimChangeSpeed = 1.0f / cAnimChangeFrame;
	//�A�j���[�V�����u�����h���̍ő�
	constexpr float cAnimBlendRateMax = 1.0f;
	//�����ɂ��������鑬�x
	constexpr float cWalkSpeed = 1.5f;
	//�_�b�V���ɂ��������鑬�x
	constexpr float cDashSpeed = 4.0f;
	//�������[�V�����̃v���C�^�C��
	constexpr float cAnimWalkTime = 0.5f;
	//�����A�j���[�V�����̃g�[�^���^�C��
	constexpr float cAnimWalkReverseTimeInit = 35.0f;
	//�{�^����������Ă��邩�̊m�F�p�ϐ�
	int cAbutton = 0;
	int cRbutton = 0;
	bool cRstickButton = false;
	//�X�e�B�b�N�̓��͂𓾂�
	int cAnX = 0;
	int cAnY = 0;
	//����ł̈ړ�����
	float cAvoidanceMove = 0.0f;
	//����̕������������
	bool cOneAvoidance = false;
	//�U���ł̒ǉ��U������
	int cAddAttackTime = 0;
	//�s���ł̈ړ�����
	float cMove = 0.0f;
	//���̍U���͈�
	constexpr float cFistAttackRadius = 30.0f;
	//�U���̔���͈�
	constexpr float cPartAttackRadius = 8.0f;
	//���U���̍U���͈�
	constexpr float cStrengthAttackRadius = 100.0f;
	//���̕�
	constexpr float cShieldWidth = 20.0f;
	//���̉�
	constexpr float cShieldHight = 60.0f;
	//���̉��s
	constexpr float cShieldDepht = 10.0f;
	//���̍��G�͈�
	constexpr float cShieldSearchRadius = 100.0f;
	//���݂̃A�^�b�N�̃i���o�[������
	int cNowAttackNumber = 0;
	//�U���̏I������
	int cIsEndAttack = 0;
	//�����̍U���͏���
	float cEquipmentAttack = 0.0f;
	//�G����̃_���[�W�擾
	float cDamage = 0.0f;
	//�l�`�̃��f���p�X
	constexpr const char* cPath = "Data/Player/PlayerModelPuppet.mv1";
	//��������񂾂�����������
	bool cEquipmentOne = false;
	//���̒��ɓ��鎞�̃|�W�V�����ݒ�
	bool cEnterPos = false;

	//�V���O���g��
	auto& handle = HandleManager::GetInstance();
	auto& cEffect = EffectManager::GetInstance();


	int cAnimIdx;

	bool cOne = false;
	bool cTwo = false;

	bool cHit = false;         //�U����̂Ɏ󂯂�Ƃ��̔���
	bool cShieldHit = false;   //�U�������Ɏ󂯂�Ƃ��̔���
}

Player::Player() :
	CharacterBase(Collidable::Priority::Low, ObjectTag::Player),
	m_xpad(),
	m_attackNumber(0),
	m_mapNow(0),
	m_searchRadius(0.0f),
	m_updateX(0.0f),
	m_updateY(0.0f),
	m_updateZ(0.0f),
	m_attackDamage(0.0f),
	m_equipmentMuscle(0.0f),
	m_equipmentSkill(0.0f),
	m_menuOpen(false),
	m_restTouch(false),
	m_rest(false),
	m_itemPick(false),
	m_bigRest(false),
	m_lockonTarget(false),
	m_warp(false),
	m_bossStart(false),
	m_moveAnimFrameIndex(0),
	m_moveAnimFrameRight(0),
	m_moveAnimShieldFrameIndex(0),
	m_moveAnimShieldFrameHandIndex(0),
	m_cameraAngle(0.0f),
	m_lockAngle(0.0f),
	m_avoidanceNow(false),
	m_shieldNow(false),
	m_shieldOne(false),
	m_animReverse(false),
	m_deadReset(false),
	m_message(false),
	m_read(false),
	m_moveWeaponFrameMatrix(),
	m_moveShieldFrameMatrix(),
	m_rollMove(VGet(0.0f,0.0f,0.0f)),
	m_moveVector(VGet(0.0f,0.0f,0.0f)),
	m_shieldPos(),
	m_shieldSize(),
	m_shieldSearchPos(),
	m_notRoll(0)
{

	//�J�v�Z���^
	auto collider = Collidable::AddCollider(MyLibrary::CollidableData::Kind::Capsule, false);
	auto capsuleCol = dynamic_cast<MyLibrary::CollidableDataCapsule*>(collider.get());
	capsuleCol->m_len = 50.0f;
	capsuleCol->m_radius = 12.0f;
	capsuleCol->m_vec = MyLibrary::LibVec3(0.0f, capsuleCol->m_vec.y + 2.0f, 0.0f);

	//���x���̏�����
	m_levelStatus.sl_hp = 1;
	m_levelStatus.sl_stamina = 1;
	m_levelStatus.sl_muscle = 1;
	m_levelStatus.sl_skill = 1;

	//�A�j���[�V�����̔��菉����
	m_anim.s_attack = false;
	m_anim.s_hit = false;
	m_anim.s_isDead = false;
	m_anim.s_moveflag = false;
	m_animChange.sa_avoidance = false;
	m_animChange.sa_dashMove = false;
	m_animChange.sa_recovery = false;
	m_animChange.sa_taking = false;
	m_animChange.sa_touch = false;
	m_animChange.sa_bossEnter = false;
	m_animChange.sa_shieldIdle = false;
	m_animChange.sa_enterShield = false;
	m_animChange.sa_imapact = false;
	m_animChange.sa_strengthAttack = false;
	
	//�G�t�F�N�g�̏�����
	m_effect.s_heel = false;

	//���f���ǂݍ���
	m_modelHandle = handle.GetModelHandle(cPath);

	//���f���̃T�C�Y�ݒ�
	MV1SetScale(m_modelHandle, VGet(cModelSizeScale, cModelSizeScale, cModelSizeScale));

	//Csv�ɂ��f�[�^�̓ǂݍ���
	CsvLoad::GetInstance().AnimDataLoad("Player", m_animIdx);
	CsvLoad::GetInstance().StatusLoad(m_status, "Player");

	for (int i = 0; i < 10; i++)
	{
		m_armorOne[i] = false;
	}

	cEquipmentOne = false;
}

Player::~Player()
{
	//���������
	MV1DeleteModel(m_modelHandle);
	//�������폜
	handle.Clear();
}

/// <summary>
/// ����������
/// </summary>
/// <param name="physics">�����N���X�̃|�C���^</param>
void Player::Init(std::shared_ptr<MyLibrary::Physics> physics, GameManager* manager, Weapon& weapon, Shield& shield, Armor& armor, bool anim)
{
	m_pPhysics = physics;

	//�����蔻��̒ǉ�
	Collidable::Init(m_pPhysics);

	//�v���C���[�̏����ʒu�ݒ�
	rigidbody.Init(false);
	if (manager->GetThisMapName() == 0)
	{
		rigidbody.SetPos(MyLibrary::LibVec3(40.0f, 12.0f, 0.0f));
	}
	else if (manager->GetThisMapName() == 1)
	{
		rigidbody.SetPos(MyLibrary::LibVec3(485.0f, 12.0f, -800.0f));
	}
	// ���F�X�������߂̏������ʒu
	//rigidbody.SetPos(MyLibrary::LibVec3(40.0f, 12.0f, 0.0f));
	rigidbody.SetNextPos(rigidbody.GetPos());
	rigidbody.SetVec(MyLibrary::LibVec3(0.0f, 40.0f, 0.0f));
	m_collisionPos = rigidbody.GetPos();
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	m_updateX = 485.0f;
	m_updateY = 12.0f;
	m_updateZ = -800.0f;

	m_searchRadius = 200.0f;

	//���j���[�����
	m_menuOpen = false;

	//���������ꍇ
	m_attackRadius = cFistAttackRadius;

	m_pAttack = std::make_shared<AttackObject>(m_attackRadius);
	//m_pPartAttack = std::make_shared<AttackObjectPart>(cPartAttackRadius, 0.0f, 0.0f);
	m_pStrengthAttack = std::make_shared<AttackObject>(cStrengthAttackRadius);

	m_pSearch = std::make_shared<PlayerSearchObject>(m_searchRadius);
	m_pSearch->Init(m_pPhysics, rigidbody.GetPos());

	m_shieldSize = MyLibrary::LibVec3::Size(cShieldWidth, cShieldHight, cShieldDepht);

	m_pShield = std::make_shared<ShieldObject>(cShieldWidth, cShieldHight, cShieldDepht);
	m_pShieldSearch = std::make_shared<PlayerSearchObject>(cShieldSearchRadius);

	if (anim)
	{
		//�ҋ@�A�j���[�V�����ݒ�
		m_nowAnimNo = MV1AttachAnim(m_modelHandle, m_animIdx["Idle"]);
		m_nowAnimIdx = m_animIdx["Idle"];
	}

	//HP�̍ő�񕜗�
	m_maxHeel = 80;

	//�ړ�����
	cMove = 0.5f;

	m_staminaBreak = false;

	//����������
	if (!cEquipmentOne)
	{
		weapon.SetFist(true);
		shield.SetFist(true);
		armor.SetBody(true);

		cEquipmentOne = true;
	}

	m_armorOne[0] = true;
}

/// <summary>
/// �Q�[���̎d�l��ŏ���������
/// </summary>
/// <param name="physics"></param>
void Player::GameInit(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pPhysics = physics;

	m_searchRadius = 200.0f;

	//����ł����ꍇ
	if (m_anim.s_isDead)
	{
		Collidable::Init(m_pPhysics);
		m_pSearch->Init(m_pPhysics, rigidbody.GetPos());

		m_anim.s_isDead = false;
	}

	CsvLoad::GetInstance().StatusLoad(m_status, "Player");

	//�v���C���[�̏����ʒu�ݒ�
	rigidbody.Init(true);
	rigidbody.SetPos(MyLibrary::LibVec3(m_updateX, m_updateY, m_updateZ));
	rigidbody.SetNextPos(rigidbody.GetPos());
	rigidbody.SetVec(MyLibrary::LibVec3(0.0f, 40.0f, 0.0f));
	m_collisionPos = rigidbody.GetPos();
	SetModelPos();
	MV1SetPosition(m_modelHandle, m_modelPos.ConversionToVECTOR());

	m_deadReset = false;
	m_staminaBreak = false;

	//HP�̍ő�񕜗�
	m_maxHeel = 80;

}

void Player::Finalize()
{
	Collidable::Finalize(m_pPhysics);
	m_pSearch->Finalize(m_pPhysics);
}

void Player::Update(Weapon& weapon, Shield& shield, Armor& armor, EnemyManager& enemy, CoreManager& core, VECTOR restpos, Tool& tool, SEManager& se, bool boss, bool dead, std::shared_ptr<MyLibrary::Physics> physics)
{
	//�Ƃ肠��������Ƃ�
	m_status.s_core = core.GetCore();
	m_levelStatus.sl_all = m_levelStatus.sl_hp + m_levelStatus.sl_muscle + m_levelStatus.sl_skill + m_levelStatus.sl_stamina;

	//�A�j���[�V�����ňړ����Ă���t���[���̔ԍ�����������
	m_moveAnimFrameIndex = MV1SearchFrame(m_modelHandle, "mixamorig:Hips");
	m_moveAnimFrameRight = MV1SearchFrame(m_modelHandle, "mixamorig:RightHandThumb2");

	//�����\���鎞�̃A�j���[�V�����t���[���擾
	m_moveAnimShieldFrameIndex = MV1SearchFrame(m_modelHandle, "mixamorig:LeftShoulder");
	m_moveAnimShieldFrameHandIndex = MV1SearchFrame(m_modelHandle, "mixamorig:LeftHand");

	//����⏂���A�^�b�`����t���[���̃��[�J�������[���h�ϊ��s����擾����
	m_moveWeaponFrameMatrix = MV1GetFrameLocalWorldMatrix(m_modelHandle, m_moveAnimFrameRight);
	m_moveShieldFrameMatrix = MV1GetFrameLocalWorldMatrix(m_modelHandle, m_moveAnimShieldFrameHandIndex);


	//�p�b�h���͎擾
	GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

	//�A�j���[�V�����̐؂�ւ�
	if (m_prevAnimNo != -1)
	{
		//�t���[���ŃA�j���[�V������؂�ւ���
		m_animBlendRate += cAnimChangeSpeed;
		if (m_animBlendRate >= cAnimBlendRateMax)
		{
			m_animBlendRate = cAnimBlendRateMax;
		}

		//�A�j���[�V�����̃u�����h����ݒ肷��
		MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, cAnimBlendRateMax - m_animBlendRate);
		MV1SetAttachAnimBlendRate(m_modelHandle, m_nowAnimNo, m_animBlendRate);
	}

	//���񂾎��̃A�j���[�V����
	if (m_status.s_hp <= 0)
	{
		if (!m_anim.s_isDead)
		{
			m_anim.s_isDead = true;

			m_nowAnimIdx = m_animIdx["Death"];

			ChangeAnim(m_nowAnimIdx, m_animOne[0], m_animOne);
			m_lockonTarget = false;

			Finalize();
		}
	}

	//���S�A�j���[�V�������I��������
	if (m_anim.s_isDead && m_nowFrame >= 68.0f)
	{
		m_deadReset = true;
	}

	//�A�i���O�X�e�B�b�N���g���Ĉړ�
	int analogX = 0;
	int analogY = 0;

	float SetAngleX = 0.0f;
	float SetAngleY = 0.0f;

	if (!m_anim.s_isDead && !m_animChange.sa_avoidance && !m_anim.s_attack && !m_animChange.sa_recovery && !m_anim.s_hit && !m_animChange.sa_bossEnter
		&& !m_animChange.sa_imapact && !m_rest && !m_animChange.sa_taking && !m_animChange.sa_strengthAttack)
	{
		GetJoypadAnalogInput(&analogX, &analogY, DX_INPUT_PAD1);

		cOneAvoidance = false;
	}

	if (m_animChange.sa_avoidance && !cOneAvoidance)
	{
		GetJoypadAnalogInput(&analogX, &analogY, DX_INPUT_PAD1);

		cOneAvoidance = true;
	}

	//�A�i���O�X�e�B�b�N�̓��͂��i�[
	cAnX = analogX;
	cAnY = analogY;

	//�ړ�������ݒ肷��
	auto moveVec = VGet(static_cast<float>(-analogX), 0.0f, static_cast<float>(analogY));    //�x�N�g���̒���

	//�x�N�g���̒������擾����
	float len = VSize(moveVec);
	//�x�N�g���̒�����0.0�`1.0�̊����ɕϊ�����
	float rate = len / 1000.0f;
	//�A�i���O�X�e�B�b�N�����Ȕ͈͂����O����
	rate = (rate - 0.1f) / (0.8f - 0.1f);
	rate = min(rate, 1.0f);
	rate = max(rate, 0.0f);

	//���x������ł���̂ňړ��x�N�g���ɔ��f����
	moveVec = VNorm(moveVec);
	float speed = m_status.s_speed * rate;

	moveVec = VScale(moveVec, speed);

	//camera�̊p�x����
	//�R���g���[���[�ɂ��ړ����������肷��
	MATRIX mtx = MGetRotY(m_cameraAngle + DX_PI_F);
	moveVec = VTransform(moveVec, mtx);

	//���C�u�����̃x�N�^�[�ɕϊ�����
	m_moveVec = MyLibrary::LibVec3(static_cast<float>(moveVec.x), static_cast<float>(moveVec.y), static_cast<float>(moveVec.z));

	//�ړ���������v���C���[�ւ̌������������肷��
	//�ړ����Ă��Ȃ��ꍇ(�[���x�N�g��)�̏ꍇ�͕ύX���Ȃ�
	if (VSquareSize(moveVec) > 0.0f)
	{
		//���b�N�I�����ĂȂ����Ƒ������Ƃ��̃A���O��
		if (!m_lockonTarget || m_animChange.sa_dashMove && !m_animChange.sa_avoidance)
		{
			//�A���O��������
			m_angle = atan2f(-moveVec.z, moveVec.x) - DX_PI_F / 2;
		}
		//���b�N�I���������̃A���O��
		else if (m_lockonTarget && !m_animChange.sa_avoidance)
		{
			//�A���O��������
			m_angle = m_lockAngle;
		}
		//�������Ƃ�
		else if (m_animChange.sa_avoidance)
		{
			//�A���O��������
			m_angle = atan2f(-moveVec.z, moveVec.x) - DX_PI_F / 2;
		}

		//�v���C���[����������
		m_anim.s_moveflag = true;
	}
	//�v���C���[�������ĂȂ�������
	else if (VSquareSize(moveVec) <= 0.0f)
	{
		m_anim.s_moveflag = false;
	}
	

	//������ĂȂ��Ƃ�
	if (!m_animChange.sa_avoidance)
	{
		MyLibrary::LibVec3 prevVelocity = rigidbody.GetVelocity();
		MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(m_moveVec.x, prevVelocity.y, m_moveVec.z);
		rigidbody.SetVelocity(newVelocity);
	}
	//������Ă�Ƃ�
	else if(m_animChange.sa_avoidance)
	{
		//����ňړ����鋗��
		m_rollMove = VScale(VGet(sinf(m_angle), 0.0f, cosf(m_angle)), cAvoidanceMove);

		//�A���O���̕����Ɉ�苗���ړ���������
		MyLibrary::LibVec3 prevVelocity = rigidbody.GetVelocity();
		MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(-m_rollMove.x, prevVelocity.y, -m_rollMove.z);
		rigidbody.SetVelocity(newVelocity);
	}

	//�������Ă��Ȃ��Ƃ�
	if (weapon.GetFist() && shield.GetFist())
	{
		//������
		if (m_lockonTarget && !m_animChange.sa_dashMove && cAnY > 0 && cAnX < 500 && cAnX > -500)
		{
			//�t�Đ���i�߂�
			m_animReverse = true;
		}
		else
		{
			m_animReverse = false;
		}
	}
	//�������Ă���Ƃ�
	else if (!weapon.GetFist() || !shield.GetFist())
	{
		//������
		if (m_lockonTarget && !m_animChange.sa_dashMove && cAnY > 0 && cAnX < 500 && cAnX > -500)
		{
			//�t�Đ���i�߂�
			m_animReverse = true;
		}
		//�E����
		else if (m_lockonTarget && !m_animChange.sa_dashMove && cAnX > 500)
		{
			//�t�Đ���i�߂�
			m_animReverse = true;
		}
		else
		{
			m_animReverse = false;
		}
	}

	//���̍��G�̃|�W�V�����X�V
	m_shieldSearchPos = MyLibrary::LibVec3(rigidbody.GetPos().x + sinf(m_angle) * -100.0f, rigidbody.GetPos().y + 15.0f, rigidbody.GetPos().z - cosf(m_angle) * 100.0f);


	//�v���C���[�������Ă��鎞����
	if (!m_anim.s_isDead)
	{
		//���̔���
		if (m_shieldNow)
		{
			if (!m_shieldOne)
			{
				m_pShield->Init(m_pPhysics, m_shieldPos);
				m_pShieldSearch->Init(m_pPhysics, m_shieldSearchPos);
				m_shieldOne = true;
			}

			//���\�K���ɂ������ǂ���ł����Ǝv��
			if (m_pShieldSearch->GetIsStay())
			{
				if (m_pShield->GetIsStay())
				{
					m_animChange.sa_imapact = true;
					cHit = false;
					cShieldHit = false;
				}
			}

		}
		else
		{
			m_pShield->CollisionEnd();
			m_pShieldSearch->CollisionEnd();
			m_shieldOne = false;
		}

		//�_���[�W��H�炤����
		if (cHit && !m_animChange.sa_imapact)
		{
			for (auto damage : enemy.GetEnemyDamage())
			{
				if (damage > 0)
				{
					m_status.s_hp -= damage - (m_status.s_defense / 10);
				}
			}
			//Hit�G�t�F�N�g
			cEffect.EffectCreate("Hit", VGet(rigidbody.GetPos().x, rigidbody.GetPos().y + 20.0f, rigidbody.GetPos().z));
			//HitSe�Đ�
			PlaySoundMem(se.GetPlayerHitSE(), DX_PLAYTYPE_BACK, true);

			cHit = false;
		}
		//���󂯂����Ƃ��̏���
		else if (!cShieldHit && m_animChange.sa_imapact)
		{
			for (auto damage : enemy.GetEnemyDamage())
			{
				if (damage > 0)
				{
					m_status.s_stamina -= damage - (shield.GetStrengthUgly() / 10);
				}
			}

			cShieldHit = true;
		}

		EffectAction();

		//���j���[���J���Ă���Ԃ̓A�N�V�����ł��Ȃ�
		if (!m_menuOpen)
		{
			//�A�N�V�������ł��Ȃ�����
			if (!m_animChange.sa_avoidance && !m_anim.s_hit && !m_animChange.sa_recovery && !m_animChange.sa_bossEnter && !m_animChange.sa_imapact
				&& !m_rest && !m_animChange.sa_strengthAttack)
			{
				Action(restpos, tool, shield, se, boss, dead);
			}
		}

		//���������Ă��Ȃ����̃A�j���[�V����
		if (weapon.GetFist() && shield.GetFist())
		{
			NotWeaponAnimation();
		}
		//���������Ƃ��̃A�j���[�V����
		else if (!weapon.GetFist() || !shield.GetFist())
		{
			WeaponAnimation(shield);
		}

		AllAnimation();

		//�h������Ă��Ȃ����̏���
		if (armor.GetBody())
		{
			ArmorChange(0, cPath);	
		}
		//�h����������̏���
		else if (!armor.GetBody())
		{
			ArmorChange(1, armor.GetPath());	
		}
		
	}

	if (!m_anim.s_isDead)
	{
		//�A�j���[�V�����̍X�V
		m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);
	}
	//���S�����Ƃ��̃A�j���[�V�����X�V
	else if (m_anim.s_isDead && m_nowFrame <= 68.0f)
	{
		m_isAnimationFinish = UpdateAnim(m_nowAnimNo, ANIMATION_MAX);
	}

	//�v���C���[�̃|�W�V����������
	SetModelPos();
	
	//�����̍X�V
	weapon.Update(m_moveWeaponFrameMatrix);
	shield.Update(m_moveShieldFrameMatrix);

	//�����ł̃X�e�[�^�X�㏸���v���X����
	if (weapon.GetFist())
	{
		cEquipmentAttack = 0.0f;
		m_equipmentMuscle = 0.0f;
		m_equipmentSkill = 0.0f;
	}
	else if (weapon.GetBlack())
	{
		cEquipmentAttack = weapon.GetBlackAttack();
		m_equipmentMuscle = weapon.GetBlackMuscle();
		m_equipmentSkill = weapon.GetBlackSkill();
	}

	if (armor.GetBody())
	{
		m_status.s_defense = 0.0f;
	}
	else if (armor.GetCommon())
	{
		m_status.s_defense = armor.GetCommonDefence();
	}

	//�X�^�~�i��
	if (m_status.s_stamina < ms_maxStatus.sm_stamina && !m_animChange.sa_avoidance && !m_anim.s_attack && !m_animChange.sa_dashMove && !m_animChange.sa_strengthAttack && !m_shieldNow)
	{
		m_status.s_stamina += 0.5f;
	}
	//�����\������Ԃ��Ɖ񕜂��x���Ȃ�
	else if (m_shieldNow && m_status.s_stamina < ms_maxStatus.sm_stamina)
	{
		m_status.s_stamina += 0.2f;
	}

	//�X�^�~�i�؂�
	if (m_status.s_stamina <= 0.0f)
	{
		m_staminaBreak = true;
		m_animChange.sa_dashMove = false;
		m_status.s_speed = cWalkSpeed;
	}
	else if (m_status.s_stamina >= 50.0f)
	{
		m_staminaBreak = false;
	}

	//����̃|�W�V�����X�V
	MyLibrary::LibVec3 centerPos = rigidbody.GetPos();
	MyLibrary::LibVec3 attackPos = MyLibrary::LibVec3(rigidbody.GetPos().x + sinf(m_angle) * -25.0f, rigidbody.GetPos().y + 15.0f, rigidbody.GetPos().z - cosf(m_angle) * 25.0f);
	MyLibrary::LibVec3 StrengthAttackPos = MyLibrary::LibVec3(rigidbody.GetPos().x, rigidbody.GetPos().y, rigidbody.GetPos().z);
	m_shieldPos = MyLibrary::LibVec3(rigidbody.GetPos().x + sinf(m_angle) * -15.0f, rigidbody.GetPos().y + 25.0f, rigidbody.GetPos().z - cosf(m_angle) * 15.0f);

	//sin��X����width�̃T�C�Y���o��
	if (sinf(m_angle) > 0)
	{
		m_shieldSize.width = 30.0f + sinf(m_angle) * -15.0f;
	}
	else if (sinf(m_angle) < 0)
	{
		m_shieldSize.width = 30.0f - sinf(m_angle) * -15.0f;
	}

	//cos��Z����depth�T�C�Y���o��
	if (cosf(m_angle) > 0)
	{
		m_shieldSize.depth = 30.0f + cosf(m_angle) * -15.0f;
	}
	else if (cosf(m_angle) < 0)
	{
		m_shieldSize.depth = 30.0f - cosf(m_angle) * -15.0f;
	}

	//���󂯏I��
	if (m_animChange.sa_imapact && m_isAnimationFinish)
	{
		m_animChange.sa_imapact = false;
	}


	//����̍X�V
	m_pSearch->Update(centerPos);
	m_pAttack->Update(attackPos);
	//m_pPartAttack->Update(attackPos);
	m_pStrengthAttack->Update(StrengthAttackPos);
	m_pShield->Update(m_shieldPos, m_shieldSize);
	m_pShieldSearch->Update(m_shieldSearchPos);

	//���ݒ�
	if (m_anim.s_hit)
	{
		//�U�����胊�Z�b�g
		m_pAttack->CollisionEnd();
		//m_pPartAttack->CollisionEnd();
	}

	//���݂��I��点��
	if (m_anim.s_hit && m_isAnimationFinish)
	{
		m_anim.s_hit = false;
	}

	//���������̃X�^�~�i����
	if (m_animChange.sa_dashMove)
	{
		m_status.s_stamina -= 0.1f;
	}

	//����s����
	if (!m_isAnimationFinish && m_animChange.sa_avoidance && !m_anim.s_hit)
	{
		//�t���[�����
		if (m_nowFrame >= 0.0f && m_nowFrame <= 20.0f)
		{
			m_avoidanceNow = true;

			cAvoidanceMove = 4.0f;
		}
		else if (m_nowFrame >= 20.0f && m_nowFrame <= 30.0f)
		{
			m_avoidanceNow = false;

			cAvoidanceMove = 1.0f;
		}
		else
		{
			m_avoidanceNow = false;

			cAvoidanceMove = 0.2f;
		}
	}
	//����I��
	else if (m_isAnimationFinish && m_animChange.sa_avoidance)
	{
		m_animChange.sa_avoidance = false;
	}

	//�U���͊i�[
	m_attackDamage = (m_status.s_attack + (m_status.s_muscle * 0.5f) + (m_status.s_skill * 0.5f)) + (cEquipmentAttack + ((m_status.s_muscle - 1) * m_equipmentMuscle) + ((m_status.s_skill - 1) * m_equipmentSkill));

	//�U����
	if (!m_isAnimationFinish && m_anim.s_attack)
	{
		//��i�K�ڂ̍U��
		if (m_nowFrame <= 40.0f)
		{
			//���݂̃A�^�b�N�i���o�[
			cNowAttackNumber = 1;

			m_pAttack->SetAttack(m_attackDamage);
			//m_pPartAttack->SetAttack(m_status.s_attack + cEquipmentAttack);

			//�U�����蔭���t���[��
			if (m_nowFrame == 25.0f)
			{
				//�U��SE�Đ�
				PlaySoundMem(se.GetAttackSE(), DX_PLAYTYPE_BACK, true);

				m_status.s_stamina -= 25.0f;
				m_pAttack->Init(m_pPhysics);
				//m_pPartAttack->Init(m_pPhysics);
			}
			else if (m_nowFrame >= 35.0f && m_nowFrame < 40.0f)
			{
				//��������Z�b�g
				m_pAttack->CollisionEnd();
				//m_pPartAttack->CollisionEnd();
			}
			//�U���I��
			else if (m_nowFrame >= 40.0f && m_attackNumber == 1)
			{
				cIsEndAttack = 1;
			}

		}
		//��i�K�ڂ̍U��
		else if (m_nowFrame <= 70.0f && cIsEndAttack == 1)
		{
			//���݂̃A�^�b�N�i���o�[
			cNowAttackNumber = 2;

			m_pAttack->SetAttack((m_attackDamage) * 1.1);
			//m_pPartAttack->SetAttack(m_status.s_attack + cEquipmentAttack * 1.1);

			//�U�����蔭���t���[��
			if (m_nowFrame == 55.0f)
			{
				//�U��SE�Đ�
				PlaySoundMem(se.GetAttackSE(), DX_PLAYTYPE_BACK, true);

				m_status.s_stamina -= 25.0f;
				m_pAttack->Init(m_pPhysics);
				//m_pPartAttack->Init(m_pPhysics);
			}
			else if (m_nowFrame >= 65.0f && m_nowFrame < 70.0f)
			{
				//�U�����胊�Z�b�g
				m_pAttack->CollisionEnd();
				//m_pPartAttack->CollisionEnd();

			}
			//�U���I��
			else if (m_nowFrame >= 70.0f && m_attackNumber == 2)
			{
				cIsEndAttack = 2;
			}
		}
		//�O�i�K�ڂ̍U��
		else if (m_nowFrame <= 110.0f && cIsEndAttack == 2)
		{
			//���݂̃A�^�b�N�i���o�[
			cNowAttackNumber = 3;

			m_pAttack->SetAttack((m_attackDamage) * 1.2);
			//m_pPartAttack->SetAttack(m_status.s_attack + cEquipmentAttack * 1.2);

			//�U�����蔭���t���[��
			if (m_nowFrame == 85.0f)
			{
				//�U��SE�Đ�
				PlaySoundMem(se.GetAttackSE(), DX_PLAYTYPE_BACK, true);

				m_status.s_stamina -= 25.0f;
				m_pAttack->Init(m_pPhysics);
				//m_pPartAttack->Init(m_pPhysics);
			}
			else if (m_nowFrame >= 95.0f && m_nowFrame < 110.0f)
			{
				//�U�����胊�Z�b�g
				m_pAttack->CollisionEnd();
				//m_pPartAttack->CollisionEnd();
			}
			//�U���I��
			else if (m_nowFrame >= 110.0f)
			{
				cIsEndAttack = 0;
			}
		}
		else
		{
			m_anim.s_attack = false;

			//�U���i�K������������
			m_attackNumber = 0;
			//�U���I��
			cIsEndAttack = 0;
			//�U�����胊�Z�b�g
			m_pAttack->CollisionEnd();
			//m_pPartAttack->CollisionEnd();
		}
	}
	//�U���I��
	else if (m_isAnimationFinish && m_anim.s_attack)
	{
		m_anim.s_attack = false;

		//�U���i�K������������
		m_attackNumber = 0;
		//�U���I��
		cIsEndAttack = 0;
	}

	//���U��
	if (!m_isAnimationFinish && m_animChange.sa_strengthAttack)
	{
		m_pStrengthAttack->SetAttack(120.0f);

		//�G�t�F�N�g���o��
		if (m_nowFrame == 25.0f)
		{
			//�U��SE�Đ�
			PlaySoundMem(se.GetBossAttackSE3(), DX_PLAYTYPE_BACK, true);

			cEffect.EffectCreate("BearLance", VGet(rigidbody.GetPos().x, rigidbody.GetPos().y - 12.0f, rigidbody.GetPos().z));
		}

		//�t���[�����ɍU���𔭐�
		if (m_nowFrame == 58.0f)
		{
			m_status.s_stamina -= 50.0f;
			m_pStrengthAttack->Init(m_pPhysics);
		}
		else if (m_nowFrame >= 68.0f)
		{
			//���胊�Z�b�g
			m_pStrengthAttack->CollisionEnd();
		}
	}
	//���U���I��
	if (m_animChange.sa_strengthAttack && m_isAnimationFinish)
	{
		m_animChange.sa_strengthAttack = false;
	}


	//�񕜂���
	if (!m_isAnimationFinish && m_animChange.sa_recovery)
	{
		//HP�̍ő�l�𒴂��Ȃ��悤�ɉ񕜂���
		if (ms_maxStatus.sm_hp > m_status.s_hp && m_heel < m_maxHeel)
		{
			m_heel++;

			m_status.s_hp++;
		}
	}
	//�񕜏I��
	else if (m_isAnimationFinish && m_animChange.sa_recovery)
	{
		m_animChange.sa_recovery = false;

		m_heel = 0;
	}

	
	//�{�X�����ɓ��郂�[�V�����I��
	if (m_isAnimationFinish && m_animChange.sa_bossEnter)
	{
		m_animChange.sa_bossEnter = false;
	}
	//�{�X�����ɓ��郂�[�V������
	else if (m_animChange.sa_bossEnter)
	{
		//�Ƃ肠�����Ë��_�Ō�Œ���
		//��Ƀv���C���[�̓����蔻��������A���f�������𓮂������f���̍ŏI�n�_�ɓ����蔻����o��
		//�����s
		if (cEnterPos)
		{
			rigidbody.SetPos(MyLibrary::LibVec3(15.0f, 12.0f, 0.0f));

			m_angle = 1.5f;

			cEnterPos = false;
		}


		//����ňړ����鋗��
		m_moveVector = VScale(VGet(sinf(m_angle), 0.0f, cosf(m_angle)), cMove);

		//�A���O���̕����Ɉ�苗���ړ���������
		MyLibrary::LibVec3 prevVelocity = rigidbody.GetVelocity();
		MyLibrary::LibVec3 newVelocity = MyLibrary::LibVec3(-m_moveVector.x, prevVelocity.y, -m_moveVector.z);
		rigidbody.SetVelocity(newVelocity);
	}

	//�A�C�e���擾�I��
	if (m_animChange.sa_taking)
	{
		if (m_nowFrame >= 60.0f)
		{
			m_animChange.sa_taking = false;
		}
		//�擾���ɊԈ���ĉ������������A�C�e���擾���I������
		if (m_animChange.sa_avoidance)
		{
			m_animChange.sa_taking = false;
		}

	}

	physics->CheckUpdate();
}

/// <summary>
/// �v���C���[�̃A�N�V��������
/// </summary>
void Player::Action(VECTOR restpos, Tool& tool, Shield& shield, SEManager& se, bool boss, bool dead)
{
	
	if (boss && !dead)
	{
		if (!m_lockonTarget)
		{
			//��񂾂�����
			if (m_xpad.Buttons[7] == 1 && !cRstickButton)
			{
				m_lockonTarget = true;
				cRstickButton = true;
			}
			else if (m_xpad.Buttons[7] == 0)
			{
				cRstickButton = false;
			}
		}
		else if (m_lockonTarget)
		{
			//��񂾂�����
			if (m_xpad.Buttons[7] == 1 && !cRstickButton)
			{
				m_lockonTarget = false;
				cRstickButton = true;
			}
			else if (m_xpad.Buttons[7] == 0)
			{
				cRstickButton = false;
			}
		}
	}
	else
	{
		//�^�[�Q�b�g�ł��鎞
		if (!m_lockonTarget && m_pSearch->GetIsStay())
		{
			//��񂾂�����
			if (m_xpad.Buttons[7] == 1 && !cRstickButton)
			{
				m_lockonTarget = true;
				cRstickButton = true;
			}
			else if (m_xpad.Buttons[7] == 0)
			{
				cRstickButton = false;
			}
		}
		//�^�[�Q�b�g���O��
		else if (m_lockonTarget == true && m_pSearch->GetIsStay())
		{
			//��񂾂�����
			if (m_xpad.Buttons[7] == 1 && !cRstickButton)
			{
				m_lockonTarget = false;
				cRstickButton = true;
			}
			else if (m_xpad.Buttons[7] == 0)
			{
				cRstickButton = false;
			}
		}
		//�^�[�Q�b�g�𖳗����O��
		else if (m_pSearch->GetIsExit())
		{
			m_lockonTarget = false;
			cRstickButton = false;
		}
	}
	

	//A�{�^���������ꂽ��_�b�V�������
	//�X�^�~�i�������
	if (m_xpad.Buttons[12] == 1 && !m_anim.s_attack && !m_staminaBreak)
	{
		//�_�b�V��
		if (cAbutton > 30)
		{
			m_animChange.sa_avoidance = false;

			//�_�b�V��
			m_animChange.sa_dashMove = true;

			m_status.s_speed = cDashSpeed;
		}

		if (cAbutton < 31)
		{
			cAbutton++;
		}
	}
	else
	{
		m_animChange.sa_dashMove = false;

		m_status.s_speed = cWalkSpeed;

		if (m_notRoll >= 10)
		{
			//����ɕK�v�ȃX�^�~�i������ꍇ
			if (m_status.s_stamina >= 20 && !m_staminaBreak)
			{
				//���
				//�������u��
				if (cAbutton > 0 && cAbutton < 30 && m_animChange.sa_avoidance == false)
				{
					m_status.s_stamina -= 20;

					m_animChange.sa_avoidance = true;
				}
			}
		}
		else
		{
			m_notRoll++;
		}
		

		cAbutton = 0;
	}

	//�U���ɕK�v�ȃX�^�~�i������ꍇ
	if (m_status.s_stamina >= 25 && !m_staminaBreak)
	{
		//�U��
	    //R�{�^�����������ƂōU��
		if (m_xpad.Buttons[9] == 1)
		{
			cRbutton++;
			
			//��񂾂���������悤�ɂ���
			if (cRbutton == 1)
			{
				m_anim.s_attack = true;

				//�ǉ��U����t
				if (cAddAttackTime <= 30 && cAddAttackTime > 0)
				{
					//��i�K�ڂ̍U��
					if (cNowAttackNumber == 1)
					{
						m_attackNumber = 1;
					}
					//�O�i�K�ڂ̍U��
					else if (cNowAttackNumber == 2)
					{
						m_attackNumber = 2;
					}
				}

				//�ǉ��U�����Ԃ�������
				cAddAttackTime = 40;
			}
		}
		else
		{
			cRbutton = 0;
		}
	}
	
	//�ǉ��U����t���Ԃ����炷
	if (cAddAttackTime <= 40 && cAddAttackTime > 0 && --cAddAttackTime > -1);

	//���U��
	//ZR�{�^��
	//�U���ɕK�v�ȃX�^�~�i����������
	if (m_status.s_stamina >= 50.0f && !m_staminaBreak)
	{
		if (m_xpad.RightTrigger)
		{
			m_animChange.sa_strengthAttack = true;
		}
	}
	

	//�s�����͖h��ł��Ȃ�
	if (!m_anim.s_attack && !m_animChange.sa_avoidance && !m_animChange.sa_recovery && !shield.GetFist() && !m_staminaBreak)
	{
		//L�{�^���Ŗh��
		if (m_xpad.Buttons[8] == 1)
		{
			m_shieldNow = true;

			if (!m_animChange.sa_shieldIdle)
			{
				m_animChange.sa_enterShield = true;
			}
			
			//���̍\�����I�������Ƃ�
			if (m_animChange.sa_enterShield && m_isAnimationFinish)
			{
				m_animChange.sa_enterShield = false;
				m_animChange.sa_shieldIdle = true;
			}
		}
		else
		{
			//L�{�^���𗣂����u��
			if (m_animChange.sa_enterShield || m_animChange.sa_shieldIdle)
			{
				m_animChange.sa_enterShield = false;
				m_animChange.sa_shieldIdle = false;
			}

			m_shieldNow = false;

			FrameEndAnim(cAnimIdx, cOne, cTwo, m_moveAnimShieldFrameIndex);
			
		}
	}
	else
	{
		m_animChange.sa_enterShield = false;
		m_animChange.sa_shieldIdle = false;
		m_shieldNow = false;

		FrameEndAnim(cAnimIdx, cOne, cTwo, m_moveAnimShieldFrameIndex);

	}

	//��
	//X�{�^���������ꂽ��
	if (m_xpad.Buttons[14] == 1 && !m_anim.s_attack && tool.GetHeel().sa_number > 0)
	{
		//�����s
		if (!m_animChange.sa_recovery)
		{
			tool.SetHeel(1);

			//��SE�Đ�
			PlaySoundMem(se.GetHeelSE(), DX_PLAYTYPE_BACK, true);

			m_effect.s_heel = true;
			m_animChange.sa_recovery = true;
		}
	}
	
	//�x��
	//�x���ł�����
	if (m_restTouch && m_mapNow != 0)
	{
		//�������ʒu�X�V
		m_updateX = m_modelPos.x;
		m_updateY = m_modelPos.y;
		m_updateZ = m_modelPos.z;

		//Y�{�^���������ꂽ��
		if (m_xpad.Buttons[15] == 1)
		{
			m_rest = true;

			cEffect.EffectCreate("Rest", restpos);

			//�x��SE�Đ�
			PlaySoundMem(se.GetRestSE(), DX_PLAYTYPE_BACK, true);
		}
	}
	else if(m_mapNow != 0)
	{
		m_rest = false;
	}

	//���x�����グ����x��
	//�}�b�v�ŕ�����
	if (m_restTouch && m_mapNow == 0)
	{
		//�������ʒu�X�V
		m_updateX = m_modelPos.x;
		m_updateY = m_modelPos.y;
		m_updateZ = m_modelPos.z;

		//Y�{�^���������ꂽ��
		if (m_xpad.Buttons[15] == 1)
		{
			m_rest = true;
			m_bigRest = true;

			cEffect.EffectCreate("Rest", restpos);

			//�x��SE�Đ�
			PlaySoundMem(se.GetRestSE(), DX_PLAYTYPE_BACK, true);
		}
	}
	else if(m_mapNow == 0)
	{
		m_rest = false;
		m_bigRest = false;
	}

	//�{�X�̕����ɓ���
	if (m_bossStart)
	{
		//Y�{�^������������
		if (m_xpad.Buttons[15] == 1)
		{
			m_animChange.sa_bossEnter = true;

			cEnterPos = true;
		}
	}
	
	//�A�C�e�����E��
	if (m_itemPick)
	{
		//Y�{�^������������
		if (m_xpad.Buttons[15] == 1)
		{
			m_animChange.sa_taking = true;
		}
	}

	//���b�Z�[�W��ǂ߂�
	if (m_message)
	{
		//Y�{�^������������
		if (m_xpad.Buttons[15] == 1)
		{
			m_read = true;
		}
	}


	//���j���[���J��
	if (m_xpad.Buttons[4] == 1)
	{
		m_menuOpen = true;
	}
}

/// <summary>
/// �G�t�F�N�g�֌W
/// </summary>
void Player::EffectAction()
{
	//�񕜃G�t�F�N�g
	if (m_effect.s_heel)
	{
		cEffect.EffectCreate("Heel", m_collisionPos.ConversionToVECTOR());

		m_effect.s_heel = false;
	}
}

/// <summary>
/// �}�b�v�����[�v���邽�߂̊֐�
/// </summary>
void Player::WarpMap()
{
	//Y�{�^���������ꂽ��
	if (m_xpad.Buttons[15] == 1)
	{
		//�}�b�v��ς���
		m_warp = true;
	}
	else
	{
		m_warp = false;
	}
}

/// <summary>
/// ����������ĂȂ��Ƃ��̃A�j���[�V����
/// </summary>
void Player::NotWeaponAnimation()
{
	//�U�����������ĂȂ���
	//�{�X�����ɓ����Ă��Ȃ���
	if (!m_anim.s_hit && !m_animChange.sa_bossEnter && !m_animChange.sa_imapact)
	{
		//����
		if (m_animChange.sa_dashMove && m_anim.s_moveflag)
		{
			m_nowAnimIdx = m_animIdx["Run"];
			ChangeAnim(m_nowAnimIdx, m_animOne[1], m_animOne);
			NotInitAnim(false);

		}
		//����
		else if (m_anim.s_moveflag)
		{
			//�^�[�Q�b�g���Ă���Ƃ�
			if (!m_lockonTarget)
			{
				m_nowAnimIdx = m_animIdx["Walk"];
				ChangeAnim(m_nowAnimIdx, m_animOne[2], m_animOne);
				NotInitAnim(false);

			}
			//�^�[�Q�b�g���Ă���Ƃ�
			else if (m_lockonTarget)
			{
				//������
				if (cAnX < -500)
				{
					m_nowAnimIdx = m_animIdx["LeftWalk"];
					ChangeAnim(m_nowAnimIdx, m_animOne[3], m_animOne);
					NotInitAnim(false);

				}
				//�E����
				else if (cAnX > 500)
				{
					m_nowAnimIdx = m_animIdx["RightWalk"];
					ChangeAnim(m_nowAnimIdx, m_animOne[4], m_animOne);
					NotInitAnim(false);

				}
				//������or����
				if (cAnX < 500 && cAnX > -500)
				{
					m_nowAnimIdx = m_animIdx["Walk"];
					ChangeAnim(m_nowAnimIdx, m_animOne[2], m_animOne, cAnimWalkTime, m_animReverse, cAnimWalkReverseTimeInit);
					NotInitAnim(false);

				}
			}
			
		}
	}
}

/// <summary>
/// �S�Ă̏�Ԃ̎��ɍs���A�j���[�V�����֐�
/// </summary>
void Player::AllAnimation()
{
	//�v���C���[�������Ă���Ƃ�����
	if (!m_anim.s_isDead)
	{
		//�U��������������
		//�{�X�����ɓ�������
		if (m_anim.s_hit && !m_animChange.sa_bossEnter && !m_animChange.sa_imapact)
		{
			m_nowAnimIdx = m_animIdx["Hit"];
			ChangeAnim(m_nowAnimIdx, m_animOne[5], m_animOne);
			NotInitAnim(false);

		}
		//�U�����������ĂȂ��Ƃ�
		//�{�X�����ɓ�������
		else if (!m_anim.s_hit && !m_animChange.sa_bossEnter && !m_animChange.sa_imapact)
		{
			//�����ĂȂ���
			if (!m_anim.s_moveflag && !m_animChange.sa_avoidance && !m_anim.s_attack && !m_animChange.sa_recovery && 
				!m_shieldNow && !m_animChange.sa_taking && !m_animChange.sa_strengthAttack)
			{
				m_nowAnimIdx = m_animIdx["Idle"];
				ChangeAnim(m_nowAnimIdx, m_animOne[6], m_animOne);
				NotInitAnim(false);

			}
			//���
			else if (m_animChange.sa_avoidance)
			{
				m_nowAnimIdx = m_animIdx["Roll"];
				ChangeAnim(m_nowAnimIdx, m_animOne[7], m_animOne, 0.8f);
				NotInitAnim(true);
			}
			//�U��
			else if (m_anim.s_attack && !m_animChange.sa_avoidance && !m_animChange.sa_recovery)
			{
				m_nowAnimIdx = m_animIdx["Attack1"];
				ChangeAnim(m_nowAnimIdx, m_animOne[8], m_animOne, 1.0f);
				NotInitAnim(false);
			}
			//���U��
			else if (m_animChange.sa_strengthAttack && !m_animChange.sa_avoidance && !m_animChange.sa_recovery)
			{
				m_nowAnimIdx = m_animIdx["Attack2"];
				ChangeAnim(m_nowAnimIdx, m_animOne[9], m_animOne, 0.5f);
				NotInitAnim(false);
			}
			//��
			else if (m_animChange.sa_recovery)
			{
				m_nowAnimIdx = m_animIdx["Recovery"];
				ChangeAnim(m_nowAnimIdx, m_animOne[10], m_animOne);
				NotInitAnim(false);

			}
			//�A�C�e�����擾����Ƃ�
			else if (m_animChange.sa_taking)
			{
				m_nowAnimIdx = m_animIdx["Taking"];
				ChangeAnim(m_nowAnimIdx, m_animOne[11], m_animOne);
				NotInitAnim(false);

			}
			//�M�~�b�N���쓮������Ƃ�
			else if (m_animChange.sa_touch)
			{
				m_nowAnimIdx = m_animIdx["Touch"];
				ChangeAnim(m_nowAnimIdx, m_animOne[12], m_animOne);
				NotInitAnim(false);
			}
		}
		//�{�X����������ɓ���Ƃ�
		else if (m_animChange.sa_bossEnter && !m_anim.s_hit && !m_animChange.sa_imapact)
		{
			m_nowAnimIdx = m_animIdx["BossEnter"];
			ChangeAnim(m_nowAnimIdx, m_animOne[13], m_animOne);
			NotInitAnim(true);
		}
	}
}

/// <summary>
/// ����������Ă��Ԃ̎��ɍs���A�j���[�V����
/// </summary>
void Player::WeaponAnimation(Shield& shield)
{
	//�Ƃ肠����
	//m_animChange.sa_imapact = false;

	//�v���C���[�������Ă��鎞����
	if (!m_anim.s_isDead)
	{
		//���󂯂����Ƃ�
		if (m_animChange.sa_imapact)
		{
			m_nowAnimIdx = m_animIdx["ShieldImpact"];
			ChangeAnim(m_nowAnimIdx, m_animOne[14], m_animOne);
			NotInitAnim(true);
		}
		//�U�����������ĂȂ������󂯂��Ă��Ȃ��Ƃ�
		else if (!m_anim.s_hit && !m_animChange.sa_imapact)
		{
			//����
			if (m_animChange.sa_dashMove && m_anim.s_moveflag)
			{
				m_nowAnimIdx = m_animIdx["ShieldRun"];
				ChangeAnim(m_nowAnimIdx, m_animOne[15], m_animOne);
				NotInitAnim(false);
			}
			//����
			else if (m_anim.s_moveflag)
			{
				//�^�[�Q�b�g���Ă��Ȃ��Ƃ�
				if (!m_lockonTarget)
				{
					m_nowAnimIdx = m_animIdx["ShieldWalk"];
					ChangeAnim(m_nowAnimIdx, m_animOne[16], m_animOne);
					NotInitAnim(false);
				}
				//�^�[�Q�b�g���Ă���Ƃ�
				else if (m_lockonTarget)
				{
					//���E����
					if (cAnX < -500 || cAnX > 500)
					{
						m_nowAnimIdx = m_animIdx["ShieldSideWalk"];
						ChangeAnim(m_nowAnimIdx, m_animOne[17], m_animOne, cAnimWalkTime, m_animReverse);
						NotInitAnim(false);

					}
					//������or����
					else if (cAnX < 500 && cAnX > -500)
					{
						m_nowAnimIdx = m_animIdx["ShieldWalk"];
						ChangeAnim(m_nowAnimIdx, m_animOne[16], m_animOne, cAnimWalkTime, m_animReverse, cAnimWalkReverseTimeInit);
						NotInitAnim(false);

					}
				}
			}
			//��������Ƃ�
			if (!shield.GetFist())
			{
				//�h��J�n�Ɩh��I��
				if (m_animChange.sa_enterShield)
				{
					//�L�����������Ă��Ȃ���
					if (!m_anim.s_moveflag)
					{
						m_nowAnimIdx = m_animIdx["ShieldStart"];
						ChangeAnim(m_nowAnimIdx, m_animOne[18], m_animOne, 1.0f);
						NotInitAnim(true);
					}
					//�L�����������Ă���Ƃ�
					else
					{
						
					}
					
				}
				//�h�䒆
				if (m_animChange.sa_shieldIdle)
				{
					//�L�����������Ă��Ȃ��Ƃ�
					if (!m_anim.s_moveflag)
					{
						FrameEndAnim(cAnimIdx, cOne, cTwo, m_moveAnimShieldFrameIndex);
						m_nowAnimIdx = m_animIdx["ShieldIdle"];
						ChangeAnim(m_nowAnimIdx, m_animOne[19], m_animOne);
						NotInitAnim(false);
					}
					//�L�����������Ă���Ƃ�
					else
					{
						
						cAnimIdx = m_animIdx["ShieldTransition"];
						FrameChangeAnim(cAnimIdx, cOne, cTwo, m_moveAnimShieldFrameIndex);
					}
					
				}
				
			}
		}
	}
}

/// <summary>
/// �`�揈��
/// </summary>
void Player::Draw(Armor& armor)
{
	rigidbody.SetPos(rigidbody.GetNextPos());
	m_collisionPos = rigidbody.GetPos();

#if false
	DrawFormatString(1000, 600, 0xffffff, "move : %d", m_anim.s_moveflag);
	DrawFormatString(1000, 700, 0xffffff, "attack : %d", m_anim.s_attack);
	DrawFormatString(1000, 800, 0xffffff, "hit : %d", m_anim.s_hit);
	DrawFormatString(1000, 100, 0xffffff, "death : %d", m_anim.s_isDead);
	DrawFormatString(1000, 150, 0xffffff, "avoidance : %d", m_animChange.sa_avoidance);
	DrawFormatString(1000, 200, 0xffffff, "bossEnter : %d", m_animChange.sa_bossEnter);
	DrawFormatString(1000, 250, 0xffffff, "dash : %d", m_animChange.sa_dashMove);
	DrawFormatString(1000, 300, 0xffffff, "enterShield : %d", m_animChange.sa_enterShield);
	DrawFormatString(1000, 350, 0xffffff, "imapact : %d", m_animChange.sa_imapact);
	DrawFormatString(1000, 400, 0xffffff, "recover : %d", m_animChange.sa_recovery);
	DrawFormatString(1000, 450, 0xffffff, "shieldIdle : %d", m_animChange.sa_shieldIdle);
	DrawFormatString(1000, 500, 0xffffff, "strengthAttack : %d", m_animChange.sa_strengthAttack);
	DrawFormatString(1000, 550, 0xffffff, "taking : %d", m_animChange.sa_taking);
	DrawFormatString(1000, 650, 0xffffff, "touch : %d", m_animChange.sa_touch);
#endif
#if false
	DrawFormatString(1000, 150, 0xffffff, "posx : %f", rigidbody.GetPos().x);   //15
	DrawFormatString(1000, 200, 0xffffff, "posy : %f", rigidbody.GetPos().y);   //12
	DrawFormatString(1000, 250, 0xffffff, "posz : %f", rigidbody.GetPos().z);   //0
	DrawFormatString(1000, 300, 0xffffff, "m_angle : %f", m_angle);             //1.5
#endif

#if false
	DrawFormatString(0, 400, 0xffffff, "m_nowAnim : %d", m_nowAnimIdx);
	DrawFormatString(0, 500, 0xffffff, "m_nowSpeed : %f", m_nowFrame);
	DrawFormatString(200, 100, 0xffffff, "m_blend : %f", m_animBlendRate);
	DrawFormatString(200, 200, 0xffffff, "addattacktime : %d", cAddAttackTime);
	DrawFormatString(200, 300, 0xffffff, "avoidancemove : %f", cAvoidanceMove);
	DrawFormatString(200, 400, 0xffffff, "attackNumber : %d", m_attackNumber);
	DrawFormatString(200, 500, 0xffffff, "nowAttackNumber : %d", cNowAttackNumber);

#endif
#if false
	//DrawFormatString(200, 100, 0xffffff, "animtime : %f", cAnimWalkTime);
	DrawFormatString(200, 600, 0xffffff, "weapon : %d", m_pWeapon->GetBlack());
	DrawFormatString(200, 650, 0xffffff, "shield : %d", m_pShield->GetUgly());
	DrawFormatString(200, 700, 0xffffff, "fist : %d", m_pWeapon->GetFist());
	DrawFormatString(200, 750, 0xffffff, "fist : %d", m_pShield->GetFist());
#endif

#if false
	DrawFormatString(200, 300, 0xffffff, "pickup : %d", m_itemPick);
#endif

	MV1SetPosition(m_modelHandle, VSub(m_modelPos.ConversionToVECTOR(), VGet(0.0f, 12.0f, 0.0f)));
	//���f���̉�]�n
	MV1SetRotationXYZ(m_modelHandle, VGet(0.0f, m_angle, 0.0f));
	//�`��
	MV1DrawModel(m_modelHandle);
	
}

void Player::End()
{
	//���������
	MV1DeleteModel(m_modelHandle);
	//�������폜
	handle.Clear();
}

void Player::OnCollideEnter(const std::shared_ptr<Collidable>& collidable)
{
#if _DEBUG
	std::string message = "�v���C���[��";
#endif
	auto tag = collidable->GetTag();
	switch (tag)
	{
	case ObjectTag::Enemy:
#if _DEBUG
		message += "�G";
#endif
		break;
	}
#if _DEBUG
	message += "�Ɠ�������\n";
	printfDx(message.c_str());
#endif
}

void Player::OnCollideStay(const std::shared_ptr<Collidable>& collidable)
{
#if _DEBUG
	std::string message = "�v���C���[��";
#endif
	auto tag = collidable->GetTag();
	switch (tag)
	{
	case ObjectTag::Enemy:
#if _DEBUG
		message += "�G";
#endif
		break;
	case ObjectTag::Rect:
#if _DEBUG
		message += "�{�X�����̓����";
#endif
		break;
	}
#if _DEBUG
	message += "�Ɠ�������\n";
	printfDx(message.c_str());
#endif
}

void Player::OnTriggerEnter(const std::shared_ptr<Collidable>& collidable)
{
#if _DEBUG
	std::string message = "�v���C���[��";
#endif
	auto tag = collidable->GetTag();
	switch (tag)
	{
	case ObjectTag::EnemyAttack:
#if _DEBUG
		message += "�U��";
#endif
		//HP��0�ȏゾ�ƃq�b�g����
		if (m_status.s_hp > 0.0f)
		{
			//��𒆂ƃq�b�g���ƃ{�X�����ɓ����Ă��鎞�͍U����������Ȃ�
			if (!m_avoidanceNow && !m_anim.s_hit && !m_animChange.sa_bossEnter && !m_animChange.sa_imapact)
			{
				cHit = true;

				m_anim.s_hit = true;
			}
		}
		break;
	case ObjectTag::EnemySearch:
#if _DEBUG
		message += "���G";
#endif
		break;
	case ObjectTag::Item:
#if _DEBUG
		message += "�A�C�e��";
#endif

		m_itemPick = true;

		break;
	case ObjectTag::Rest:
#if _DEBUG
		message += "�x��";
#endif
		break;
	case ObjectTag::Rect:
#if _DEBUG
		message += "�{�X�̓���";
#endif
		break;
	case ObjectTag::Message:
#if _DEBUG
		message += "���b�Z�[�W";
#endif

		m_message = true;

		break;
	}
#if _DEBUG

	message += "�Ɠ�������\n";
	printfDx(message.c_str());
#endif
}

/// <summary>
/// �h���ς���
/// </summary>
/// <param name="one">�h��̃i���o�[�w��</param>
/// <param name="path">�p�X</param>
void Player::ArmorChange(int one, std::string path)
{
	//�h���ς������ɃA�j���[�V�����̃o�O����������

	//��񂾂����s
	if (!m_armorOne[one])
	{
		//���������
		MV1DeleteModel(m_modelHandle);
		//���f���ǂݍ���
		m_modelHandle = MV1LoadModel(path.c_str());

		//���f���̃T�C�Y�ݒ�
		MV1SetScale(m_modelHandle, VGet(cModelSizeScale, cModelSizeScale, cModelSizeScale));

		//�ҋ@�A�j���[�V�����ݒ�
		m_nowAnimIdx = m_animIdx["Idle"];

		ChangeAnim(m_nowAnimIdx, m_animOne[20], m_animOne);


		m_armorOne[one] = true;

	}
	//���̔z���false�ɂ���
	for (int i = 0; i < 10; i++)
	{
		if (i != one)
		{
			m_armorOne[i] = false;
		}
	}
}

/// <summary>
/// ���x���A�b�v�ɂ��X�e�[�^�X�㏸
/// </summary>
void Player::ChangeStatus()
{
	CsvLoad::GetInstance().StatusLoad(m_status, "Player");

	//�Ƃ肠�����K���ɕ␳���Ă���
	m_status.s_hp = m_status.s_hp + (m_levelStatus.sl_hp * 30);
	m_status.s_stamina = m_status.s_stamina + (m_levelStatus.sl_stamina * 10);

	//����ł���
	m_status.s_muscle = m_levelStatus.sl_muscle;
	m_status.s_skill = m_levelStatus.sl_skill;
	//�X�e�[�^�X�̍ő�l������
	ms_maxStatus.sm_hp = m_status.s_hp;
	ms_maxStatus.sm_stamina = m_status.s_stamina;
}

void Player::SetModelPos()
{
	m_modelPos = m_collisionPos;
}
