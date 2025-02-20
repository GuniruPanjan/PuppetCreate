#include "CharacterBase.h"

namespace
{
	//�A�j���[�V�����̃}�b�N�X����������ϐ�
	int cAnimMax = 0;
}

CharacterBase::CharacterBase(Priority priority, ObjectTag tag) :
	Collidable(priority, tag),
	m_modelHandle(-1),
	m_modelPos(),
	m_collisionPos(),
	m_status(),
	m_moveVec(),
	m_nowPos(VGet(0.0f,0.0f,0.0f)),
	m_nowAnimNo(-1),
	m_equipAnimNo(-1),
	m_nowAnimIdx(-1),
	m_prevAnimNo(-1),
	m_frameAnimNo(-1),
	m_animBlendRate(1.0f),
	m_nowFrame(0.0f),
	m_animTime(0.5f),
	m_isAnimationFinish(false),
	m_animInit(false),
	m_animReverse(false),
	m_angle(0.0f),
	m_animSpeed(0.0f),
	m_attackRadius(0.0f),
	m_searchRadius(0.0f),
	m_heel(0),
	m_maxHeel(0)
{
	for (int i = 0; i < ANIMATION_MAX; i++)
	{
		m_animOne[i] = false;
	}
}

CharacterBase::~CharacterBase()
{
}

/// <summary>
/// �A�j���[�V�����̍X�V
/// </summary>
/// <param name="attachNo">�i�s���������A�j���[�V�����ԍ�</param>
/// <param name="startTime">���[�v�������ǂ���</param>
/// <returns></returns>
bool CharacterBase::UpdateAnim(int attachNo, int max, float startTime)
{
	cAnimMax = max;

	//�A�j���[�V�������ݒ肳��Ă��Ȃ������瑁�����^�[��
	if (attachNo == -1) return false;

	//���ݍĐ����̃A�j���[�V�����̑��J�E���g���擾����
	float totalAnimFrame = MV1GetAttachAnimTotalTime(m_modelHandle, attachNo);
	bool isLoop = false;

	//�A�j���[�V������i�s������
	if (!m_animReverse)
	{
		m_nowFrame += m_animTime;
	}
	//�A�j���[�V�������t�i�s������
	else if (m_animReverse)
	{
		m_nowFrame -= m_animTime;
	}

	//���������ɂȂ��Ă���
	//�A�j���[�V�������Đ������鎞
	if (!m_animReverse)
	{
		while (totalAnimFrame <= m_nowFrame)
		{
			m_nowFrame -= totalAnimFrame;
			m_nowFrame += startTime;
			isLoop = true;
		}
	}
	//�A�j���[�V�������t�Đ������鎞
	else if (m_animReverse)
	{
		while (m_nowFrame <= 0.0f)
		{
			m_nowFrame += totalAnimFrame;
			m_nowFrame -= startTime;
			isLoop = true;
		}
	}

	if (!m_animInit)
	{
		//�i�߂����Ԃɐݒ�
		MV1SetAttachAnimTime(m_modelHandle, attachNo, m_nowFrame);
	}
	else if (m_animInit)
	{
		MV1SetAttachAnimTime(m_modelHandle, attachNo, m_animSpeed);
	}
	
	m_animSpeed = m_nowFrame;

	return isLoop;
}

/// <summary>
/// �A�j���[�V�����̕ύX
/// </summary>
/// <param name="animIndex">�ύX��̃A�j���[�V�����ԍ�</param>
/// <param name="animSpeed">�A�j���[�V�����^�C��</param>
void CharacterBase::ChangeAnim(int animIndex, bool& one, bool (&all)[30], float animSpeed, bool reverse, float resetTime)
{
	//��񂾂����s
	if (one == false)
	{
		//����ɌÂ��A�j���[�V�������A�^�b�`����Ă���ꍇ�����̎��_�ŏ���
		if (m_prevAnimNo != -1)
		{
			MV1DetachAnim(m_modelHandle, m_prevAnimNo);
		}

		//���ݍĐ����̑ҋ@�A�j���[�V�����͕ύX�ڂ̃A�j���[�V�����̈����ɂ���
		m_prevAnimNo = m_nowAnimNo;

		//�ύX��̃A�j���[�V�����Ƃ��ĉ��߂Đݒ肷��
		m_nowAnimNo = MV1AttachAnim(m_modelHandle, animIndex);

		//�؂�ւ��̏u�Ԃ͕ύX�O�̃A�j���[�V�������Đ�������Ԃɂ���
		m_animBlendRate = 0.0f;
	
		m_nowFrame = resetTime;

		//�A�j���[�V�������t�Đ������邩�ǂ���
		m_animReverse = reverse;

		m_animTime = animSpeed;

		//�ύX�O�̃A�j���[�V����100%
		MV1SetAttachAnimBlendRate(m_modelHandle, m_prevAnimNo, 1.0f - m_animBlendRate);

		//�ύX��̃A�j���[�V����0%
		MV1SetAttachAnimBlendRate(m_modelHandle, m_nowAnimNo, m_animBlendRate);
		

		//����������
		for (int i = 0; i < cAnimMax; i++)
		{
			all[i] = false;
		}

		one = true;
	}
	
}

void CharacterBase::FrameChangeAnim(int animIndex, bool& one, bool& two, int frame)
{
	//��񂾂����s
	if (one == false)
	{
		m_frameAnimNo = MV1AttachAnim(m_modelHandle, animIndex);
		
		one = true;
	}

	//�t���[�������̃A�j���[�V����
	MV1SetAttachAnimBlendRate(m_modelHandle, m_frameAnimNo, 0.0f);
	//�t���[�������A�j���[�V����
	MV1SetAttachAnimBlendRateToFrame(m_modelHandle, m_nowAnimNo, frame, 0.0f);
	MV1SetAttachAnimBlendRateToFrame(m_modelHandle, m_frameAnimNo, frame, 1.0f);

	//�i�߂����Ԃɐݒ�
	MV1SetAttachAnimTime(m_modelHandle, m_frameAnimNo, m_nowFrame);

	two = false;
}

void CharacterBase::FrameEndAnim(int animIndex, bool& one, bool& two, int frame)
{
	if (one || two)
	{
		MV1DetachAnim(m_modelHandle, animIndex);

		//�t���[�������A�j���[�V����
		MV1SetAttachAnimBlendRateToFrame(m_modelHandle, m_nowAnimNo, frame, 1.0f);
		MV1SetAttachAnimBlendRateToFrame(m_modelHandle, m_frameAnimNo, frame, 0.0f);

		one = false;
		two = false;
	}
}

/// <summary>
/// �A�j���[�V�����^�C�������������邩�̔��f
/// </summary>
/// <param name="init">���������邩�̔���</param>
void CharacterBase::NotInitAnim(bool init)
{
	m_animInit = init;
}
