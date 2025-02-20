#include "MapFirst.h"
#include "MapRest.h"
#include "Manager/EffectManager.h"

namespace
{
	//�}�b�v�̃p�X
	constexpr const char* cMapName = "Data/Map/Map.mv1";
	//�}�b�v�̃R���W�����p�X
	constexpr const char* cMapCollisionName = "Data/Map/Collision.mv1";
	//�R�A�̃p�X
	constexpr const char* cCoreName = "Data/Object/Core.mv1";
	//�x���n�_�̔��a
	constexpr float cRestRadius = 50.0f;
	//�{�X����������̔��a
	constexpr float cBossRommRadius = 60.0f;
	//�R�A�̔��a
	constexpr float cCoreRadius = 70.0f;
	//�R�A�̃T�C�Y
	constexpr float cCoreSize = 0.5f;
	//�{�X�����̕�
	constexpr float cBossWidth = 20.0f;
	//�{�X�����̉�
	constexpr float cBossHight = 100.0f;
	//�{�X�����̉��s
	constexpr float cBossDepth = 30.0f;
	//�{�X�����̓����������
	bool cOne = false;

	//�G�t�F�N�g�̍Đ�����
	int cEffectTime = 50;
	//�G�t�F�N�g�Đ�
	bool cEffectOne = false;
	//�G�t�F�N�g�̍Đ��ʒu
	VECTOR cEffectPos = VGet(-10.0f, 50.0f, 0.0f);

	//�V���O���g��
	auto& cEffect = EffectManager::GetInstance();
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
MapFirst::MapFirst()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
MapFirst::~MapFirst()
{
	//���������
	MV1DeleteModel(m_handle);
	MV1DeleteModel(m_collisionHandle);
	MV1DeleteModel(m_coreHandle);
	DeleteLightHandle(m_light);
}

/// <summary>
/// �f�[�^������
/// </summary>
void MapFirst::DataInit()
{
	//�f�[�^���[�h
	LoadData(cMapName, cMapCollisionName, cCoreName);

	m_stageName = "stage1";
}

/// <summary>
/// ����������
/// </summary>
void MapFirst::Init(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pPhysics = physics;

	//�}�b�v�̃T�C�Y
	m_size = 0.12f;
	m_width = 20.0f;
	m_hight = 100.0f;
	m_depth = 100.0f;

	//���f���̃T�C�Y�ύX
	MV1SetScale(m_handle, VGet(m_size, m_size, m_size));
	MV1SetScale(m_collisionHandle, VGet(m_size, m_size, m_size));

	//�|�W�V�����ݒ�
	m_mapPos = VGet(0.0f, 250.0f, 0.0f);
	m_mapCollisionPos = VGet(-241.0f, -277.0f, -173.0f);
	m_mapCorePos = VGet(-830.0f, 50.0f, 0.0f);
	m_mapRestPos = MyLibrary::LibVec3(100.0f, 0.0f, -75.0f);
	m_mapBossRoomPos = MyLibrary::LibVec3(-80.0f, 0.0f, 0.0f);
	m_mapBossEnterPos = MyLibrary::LibVec3(-10.0f, 50.0f, 0.0f);
	//m_mapBossEnterPos = MyLibrary::LibVec3(0.0f, 400.0f, 0.0f);
	m_mapCoreCollisionePos = MyLibrary::LibVec3(m_mapCorePos.x, 0.0f, m_mapCorePos.z);
	m_mapBossEnterTriggerPos = MyLibrary::LibVec3(10.0f, 50.0f, 0.0f);

	//���C�g�֌W
	ChangeLightTypeDir(VGet(-1.0f, 0.0f, 0.0f));
	m_light = CreateDirLightHandle(VGet(1.0f, 0.0f, 0.0f));

	//���G���菉����
	InitSearch(cRestRadius, m_mapRestPos);
	//�{�X���������
	InitBossRoom(cBossRommRadius, m_mapBossEnterPos);
	//�{�X���������
	InitRect(m_width, m_hight, m_depth, m_mapBossEnterPos);
	//�R�A�̔��菉����
	InitCore(cCoreRadius, m_mapCoreCollisionePos);
	//�{�X�����̔��菉����
	InitBossEnter(cBossWidth, cBossHight, cBossDepth, m_mapBossEnterTriggerPos);

	cOne = false;
}

/// <summary>
/// �X�V����
/// </summary>
/// <returns>���݂̃}�b�v</returns>
std::shared_ptr<MapBase> MapFirst::Update(bool warp, bool enter, bool Dead)
{
	MyLibrary::LibVec3::Size size = MyLibrary::LibVec3::Size(m_width, m_hight, m_depth);
	MyLibrary::LibVec3::Size triggerSize = MyLibrary::LibVec3::Size(cBossWidth, cBossHight, cBossDepth);

	m_pSearch->Update(m_mapRestPos);
	m_pBossRoom->Update(m_mapBossRoomPos);
	m_pRect->Update(m_mapBossEnterPos, size);
	m_pRectTrigger->Update(m_mapBossEnterTriggerPos, triggerSize);

	//�{�X�����񂾂Ƃ�
	if (Dead)
	{
		m_mapBossRoomPos = MyLibrary::LibVec3(-80.0f, 400.0f, 0.0f);
		m_mapBossEnterTriggerPos = MyLibrary::LibVec3(10.0f, 400.0f, 0.0f);

		m_pCore->Update(m_mapCoreCollisionePos);

	}
	else if (!Dead)
	{
		m_pCore->Update(MyLibrary::LibVec3(-1000.0f, -1000.0f, -1000.0f));
	}

	if (enter || Dead)
	{
		//��񂾂����s
		if (!cOne)
		{
			//�{�X����������̃|�W�V����
			m_mapBossEnterPos = MyLibrary::LibVec3(-10.0f, 400.0f, 0.0f);

			cOne = true;
		}
	}
	else
	{
		//��񂾂����s
		if (cOne)
		{
			//�{�X����������̃|�W�V����
			m_mapBossEnterPos = MyLibrary::LibVec3(-10.0f, 50.0f, 0.0f);

			cOne = false;
		}
	}

	//�G�t�F�N�g�̐���
	if (cEffectTime >= 20 && !Dead)
	{
		cEffect.EffectCreate("Smoke", cEffectPos);

		cEffectTime = 0;
	}
	else
	{
		cEffectTime++;
	}
	//�G�t�F�N�g����
	if (!cEffectOne && enter && !Dead)
	{
		cEffect.EffectCreate("Distortion", cEffectPos);

		cEffectOne = true;
	}
	else if(!enter)
	{
		cEffectOne = false;
	}

	return shared_from_this();   //���g�̃|�C���^
}

/// <summary>
/// ���[�v�̍X�V����
/// </summary>
/// <param name="warp"></param>
/// <returns></returns>
std::shared_ptr<MapBase> MapFirst::WarpUpdate(bool warp)
{
	if (warp)
	{
		return std::make_shared<MapRest>();
	}

	return shared_from_this();   //���g�̃|�C���^
}

/// <summary>
/// �����������̔���
/// </summary>
void MapFirst::JudgeUpdate()
{
	if (m_pSearch->GetIsStay())
	{
		int a = 0;
	}
}

/// <summary>
/// �{�X�����񂾂�o��
/// </summary>
void MapFirst::CoreUpdate()
{
	m_angle += 0.001f;

	if (m_pCore->GetIsStay())
	{
		int a = 1;
	}
}

/// <summary>
/// �`�揈��
/// </summary>
void MapFirst::Draw()
{
	//3D���f���̃|�W�V�����ݒ�
	MV1SetPosition(m_handle, m_mapPos);
	MV1SetPosition(m_collisionHandle, m_mapCollisionPos);

	//3D���f���`��
	MV1DrawModel(m_handle);
}

/// <summary>
/// �R�A�`��
/// </summary>
void MapFirst::CoreDraw()
{
	//3D���f���̃|�W�V�����ݒ�
	MV1SetPosition(m_coreHandle, m_mapCorePos);

	//3D���f���̉�]
	MV1SetRotationXYZ(m_coreHandle, VGet(0.0f, m_angle, 0.0f));
	//�傫����ς���
	MV1SetScale(m_coreHandle, VGet(cCoreSize, cCoreSize, cCoreSize));

	//3D���f���`��
	MV1DrawModel(m_coreHandle);
}

/// <summary>
/// �I������
/// </summary>
void MapFirst::End(std::shared_ptr<MyLibrary::Physics> physics, bool title)
{
	//���������
	MV1DeleteModel(m_handle);
	MV1DeleteModel(m_collisionHandle);
	MV1DeleteModel(m_coreHandle);
	DeleteLightHandle(m_light);

	if (!title)
	{
		Finalize(physics);
	}
	
}
