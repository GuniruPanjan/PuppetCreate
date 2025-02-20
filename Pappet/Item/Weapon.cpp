#include "Weapon.h"
#include "Manager/HandleManager.h"

namespace
{
	//�A�C�e����
	constexpr const char* cItemName = "Weapon";

	//�s��
	constexpr float cWeaponMatrixY = 5.655f;
	constexpr float cWeaponMatrixZ = 1.795f;

	//�V���O���g��
	auto& handle = HandleManager::GetInstance();
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Weapon::Weapon()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Weapon::~Weapon()
{
	//���������
	MV1DeleteModel(m_itemHandle);

	//���������
	handle.Clear();
}

/// <summary>
/// ����������
/// </summary>
void Weapon::Init()
{
	if (m_black.sw_equipment)
	{
		m_black.sw_attack = 30.0f;
		m_black.sw_muscle = 2.0f;   //�␳B
		m_black.sw_skill = 1.5f;    //�␳C
		m_black.sw_radius = 25.0f;
		m_itemHandle = handle.GetModelHandle("Data/Weapon/Sword.mv1");

		m_pos = VGet(-5.0f, 1.947f, -1.947f);
	}
}

/// <summary>
/// �}�b�v�A�C�e���Ƃ��Ă̏���������
/// </summary>
/// <param name="posX"></param>
/// <param name="posY"></param>
/// <param name="posZ"></param>
/// <param name="physics"></param>
void Weapon::ItemInit(float posX, float posY, float posZ, std::shared_ptr<MyLibrary::Physics> physics)
{
	//���
	m_pPhysics = physics;

	InitSearch(MyLibrary::LibVec3(posX, posY, posZ), 80.0f);

	m_centerPos = MyLibrary::LibVec3(posX, posY, posZ);
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="mat">�s��</param>
void Weapon::Update(MATRIX mat)
{
	MV1SetMatrix(m_itemHandle, MGetIdent());
	//�t���[������
	m_frameIndex = MV1SearchFrame(m_itemHandle, "2:Sphere");
	//�t���[���̃|�W�V����
	m_framePos = MV1GetFramePosition(m_itemHandle, m_frameIndex);

	m_pos = VAdd(m_framePos, m_pos);
	//�A�^�b�`���郂�f�����t���[���̍��W�����_�ɂ��邽�߂̕��s�ړ��s����쐬
	m_transMatrix = MGetTranslate(VScale(m_pos, -1.0f));

	m_transMatrix = MMult(m_transMatrix, MGetRotY(cWeaponMatrixY));
	m_transMatrix = MMult(m_transMatrix, MGetRotZ(cWeaponMatrixZ));

	m_mixMatrix = MMult(m_transMatrix, mat);

	MV1SetMatrix(m_itemHandle, m_mixMatrix);
}

void Weapon::ItemUpdate(bool taking)
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
/// <param name="mat">�s��</param>
void Weapon::Draw()
{
	//���f���`��
	MV1DrawModel(m_itemHandle);
}

/// <summary>
/// �I������
/// </summary>
void Weapon::End()
{
	//���������
	MV1DeleteModel(m_itemHandle);

	//���������
	handle.Clear();
}
