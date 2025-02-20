#include "DebugDraw.h"
#include "DxLib.h"

std::list<MyLibrary::DebugDraw::CapsuleInfo> MyLibrary::DebugDraw::m_capsuleInfo;
std::list<MyLibrary::DebugDraw::AttackCapsuleInfo> MyLibrary::DebugDraw::m_attackCapsuleInfo;
std::list<MyLibrary::DebugDraw::SphereInfo> MyLibrary::DebugDraw::m_sphereInfo;
std::list<MyLibrary::DebugDraw::RectInfo> MyLibrary::DebugDraw::m_rectInfo;

/// <summary>
/// �`����̍폜
/// </summary>
void MyLibrary::DebugDraw::Clear()
{
	//���ׂĂ̕`���񃊃X�g�̒��g��S�폜
	m_capsuleInfo.clear();
	m_attackCapsuleInfo.clear();
	m_sphereInfo.clear();
	m_rectInfo.clear();
}

/// <summary>
/// �`��
/// </summary>
void MyLibrary::DebugDraw::Draw()
{
#if _DEBUG

	//�J�v�Z���̕`���񃊃X�g�ɂ����񕪕`�悷��
	for (auto& capsule : m_capsuleInfo)
	{
		capsule.vec = capsule.vec.GetNormalized() * capsule.len;

		LibVec3 pos1 = capsule.center + capsule.vec;
		//LibVec3 pos2 = capsule.center - capsule.vec;

		//DrawCapsule3D(VAdd(capsule.center.ConversionToVECTOR(), capsule.vec.ConversionToVECTOR()), VSub(capsule.center.ConversionToVECTOR(), capsule.vec.ConversionToVECTOR()),
		//	capsule.radius, 16, capsule.color, capsule.color, false);

		DrawCapsule3D(pos1.GetVector(), capsule.center.ConversionToVECTOR(),
			capsule.radius, 16, capsule.color, capsule.color, false);

		//DrawFormatString(0, 100, 0xffffff, "posx : %f", capsule.center.x);
		//DrawFormatString(0, 200, 0xffffff, "posy : %f", capsule.center.y);
		//DrawFormatString(0, 300, 0xffffff, "posz : %f", capsule.center.z);
		//DrawFormatString(0, 400, 0xffffff, "pos1x : %f", pos1.x);
		//DrawFormatString(0, 450, 0xffffff, "pos1y : %f", pos1.y);
		//DrawFormatString(0, 500, 0xffffff, "pos1z : %f", pos1.z);
		//DrawFormatString(0, 550, 0xffffff, "pos2x : %f", pos2.x);
		//DrawFormatString(0, 600, 0xffffff, "pos2y : %f", pos2.y);
		//DrawFormatString(0, 650, 0xffffff, "pos2z : %f", pos2.z);
	}

	//�U���p�̃J�v�Z���`���񃊃X�g�ɂ����񕪕`�悷��
	for (auto& attackcapsule : m_attackCapsuleInfo)
	{
		DrawCapsule3D(attackcapsule.pos1.GetVector(), attackcapsule.pos2.GetVector(),
			attackcapsule.radius, 16, attackcapsule.color, attackcapsule.color, false);
	}

	//���̂̕`���񃊃X�g�ɂ����񕪕`�悷��
	for (auto& sphere : m_sphereInfo)
	{
		DrawSphere3D(sphere.center.ConversionToVECTOR(), sphere.radius, 16, sphere.color, sphere.color, false);
	}

	//��`�̕`���񃊃X�g�ɂ����񕪕`�悷��
	for (auto& rect : m_rectInfo)
	{
		float halfW = rect.size.width * 0.5f;
		float halfH = rect.size.height * 0.5f;
		float halfD = rect.size.depth * 0.5f;

		float right = rect.center.x + halfW;
		float left = rect.center.x - halfW;
		float top = rect.center.y + halfH;
		float bottom = rect.center.y - halfH;
		float front = rect.center.z - halfD;
		float back = rect.center.z + halfD;

		// ���̐�
		DrawLine3D(VGet(left, bottom, front), VGet(right, bottom, front), rect.color);
		DrawLine3D(VGet(left, top, front), VGet(right, top, front), rect.color);
		DrawLine3D(VGet(left, bottom, back), VGet(right, bottom, back), rect.color);
		DrawLine3D(VGet(left, top, back), VGet(right, top, back), rect.color);
		// �c�̐�
		DrawLine3D(VGet(left, top, front), VGet(left, bottom, front), rect.color);
		DrawLine3D(VGet(right, top, front), VGet(right, bottom, front), rect.color);
		DrawLine3D(VGet(left, top, back), VGet(left, bottom, back), rect.color);
		DrawLine3D(VGet(right, top, back), VGet(right, bottom, back), rect.color);
		// �O��̐�
		DrawLine3D(VGet(left, top, front), VGet(left, top, back), rect.color);
		DrawLine3D(VGet(left, bottom, front), VGet(left, bottom, back), rect.color);
		DrawLine3D(VGet(right, top, front), VGet(right, top, back), rect.color);
		DrawLine3D(VGet(right, bottom, front), VGet(right, bottom, back), rect.color);
	}

#endif
}

/// <summary>
/// �J�v�Z���^�̕`�惊�X�g�ɒǉ�����
/// </summary>
/// <param name="center">���S���W</param>
/// <param name="vec">�x�N�^�[</param>
/// <param name="radius">���a</param>
/// <param name="len">����</param>
/// <param name="color">�F</param>
void MyLibrary::DebugDraw::AddDrawCapsule(const LibVec3& center, const LibVec3& vec, const float& radius, const float& len, const unsigned int& color)
{
	//�J�v�Z���^�̕`�惊�X�g�ɒǉ�����
	CapsuleInfo addInfo;
	addInfo.center = center;
	addInfo.vec = vec;
	addInfo.radius = radius;
	addInfo.len = len;
	addInfo.color = color;
	m_capsuleInfo.emplace_back(addInfo);
}

/// <summary>
/// �U���p�̃J�v�Z���^�̕`�惊�X�g�ɒǉ�����
/// </summary>
/// <param name="pos1">���W�P</param>
/// <param name="pos2">���W�Q</param>
/// <param name="radius">���a</param>
/// <param name="color">�F</param>
void MyLibrary::DebugDraw::AddDrawAttackCapsule(const LibVec3& pos1, const LibVec3& pos2, const float& radius, const unsigned int& color)
{
	AttackCapsuleInfo addInfo;
	addInfo.pos1 = pos1;
	addInfo.pos2 = pos2;
	addInfo.radius = radius;
	addInfo.color = color;
	m_attackCapsuleInfo.emplace_back(addInfo);
}

/// <summary>
/// ���̂̕`�惊�X�g�ɒǉ�����
/// </summary>
/// <param name="center">���S���W</param>
/// <param name="radius">���a</param>
/// <param name="color">�F</param>
void MyLibrary::DebugDraw::AddDrawSphere(const LibVec3& center, const float& radius, const unsigned int& color)
{
	//���̂̕`�惊�X�g�ɒǉ�����
	SphereInfo addInfo;
	addInfo.center = center;
	addInfo.radius = radius;
	addInfo.color = color;
	m_sphereInfo.emplace_back(addInfo);
}

/// <summary>
/// ��`�̕`�惊�X�g�ɒǉ�����
/// </summary>
/// <param name="center">���S���W</param>
/// <param name="size">�T�C�Y</param>
/// <param name="color">�F</param>
void MyLibrary::DebugDraw::AddDrawRect(const LibVec3& center, const LibVec3::Size& size, const unsigned int& color)
{
	RectInfo addInfo;
	addInfo.center = center;
	addInfo.size = size;
	addInfo.color = color;
	m_rectInfo.emplace_back(addInfo);
}
