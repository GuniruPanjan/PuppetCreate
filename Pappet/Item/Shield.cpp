#include "Shield.h"
#include "Manager/HandleManager.h"

namespace
{
	//�A�C�e����
	constexpr const char* cItemName = "Shield";

	//����̍s��
	constexpr float cShieldMatrixY = 3.142f;
	constexpr float cShieldMatrixZ = 0.0f;

	//�V���O���g��
	auto& handle = HandleManager::GetInstance();
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Shield::Shield()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Shield::~Shield()
{
	//���������
	MV1DeleteModel(m_itemHandle);

	handle.Clear();
}

/// <summary>
/// ����������
/// </summary>
void Shield::Init()
{
	if (m_ugly.ss_equipment)
	{
		m_itemHandle = handle.GetModelHandle("Data/Weapon/Shield.mv1");
		m_ugly.ss_cut = 90.0f;
		m_ugly.ss_strength = 20.0f;

		m_pos = VGet(0.0f, 0.0f, 7.787f);
	}
	
}

/// <summary>
/// �}�b�v�A�C�e���Ƃ��Ă̏���������
/// </summary>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="posZ"></param>
/// <param name="physics"></param>
void Shield::ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
	//���
	m_pPhysics = physics;

	InitSearch(MyLibrary::LibVec3(posX, posY, posZ), 80.0f);

	m_centerPos = MyLibrary::LibVec3(posX, posY, posZ);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="mat">����̍s��</param>
void Shield::Update(MATRIX mat)
{
	MV1SetMatrix(m_itemHandle, MGetIdent());

	//�t���[��������
	m_frameIndex = MV1SearchFrame(m_itemHandle, "0:fbx");

	m_framePos = MV1GetFramePosition(m_itemHandle, m_frameIndex);

	m_pos = VAdd(m_framePos, m_pos);

	//�A�^�b�`���郂�f�����t���[���̍��W�����_�ɂ��邽�߂̕��s�ړ��s����쐬
	m_transMatrix = MGetTranslate(VScale(m_pos, -1.0f));

	m_transMatrix = MMult(m_transMatrix, MGetRotY(cShieldMatrixY));
	m_transMatrix = MMult(m_transMatrix, MGetRotZ(cShieldMatrixZ));

	m_mixMatrix = MMult(m_transMatrix, mat);

	MV1SetMatrix(m_itemHandle, m_mixMatrix);
}

void Shield::ItemUpdate(bool taking)
{
	//�A�C�e�����܂�����ĂȂ����̏���
	if (!m_isTaking && !taking)
	{
		if (m_pSearch->GetIsStay())
		{
			m_itemPick = true;
		}
		else
		{
			m_itemPick = false;
		}
	}
	//�A�C�e�������ꂽ���̏���
	else if (m_itemPick && taking)
	{
		//��񂾂����s

		ItemFinalize(m_pPhysics);

		m_itemPick = false;
		m_isTaking = true;
		m_itemBox = true;
	}

	m_pSearch->Update(m_centerPos);

}

/// <summary>
/// �`�揈��
/// </summary>
void Shield::Draw()
{
	//���f���`��
	MV1DrawModel(m_itemHandle);
}

void Shield::End()
{
	//���������
	MV1DeleteModel(m_itemHandle);

	handle.Clear();
}
