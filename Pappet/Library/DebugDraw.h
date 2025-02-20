#pragma once
#include <List>
#include "LibVec3.h"

namespace MyLibrary
{
	//�f�o�b�O�p�̕`������܂Ƃ߁A�\�����邽�߂̃N���X
	class DebugDraw
	{
	public:
		//�`����̍폜
		static void Clear();
		//�`��
		static void Draw();

		//�J�v�Z���̕`�惊�X�g�ɒǉ�����
		static void AddDrawCapsule(const LibVec3& center, const LibVec3& vec, const float& radius, const float& len, const unsigned int& color);

		//�U���p�̃J�v�Z���`�惊�X�g�ɒǉ�����
		static void AddDrawAttackCapsule(const LibVec3& pos1, const LibVec3& pos2, const float& radius, const unsigned int& color);

		//���̂̕`�惊�X�g�ɒǉ�����
		static void AddDrawSphere(const LibVec3& center, const float& radius, const unsigned int& color);

		//��`�̕`�惊�X�g�ɒǉ�����
		static void AddDrawRect(const LibVec3& center, const LibVec3::Size& size, const unsigned int& color);

	private:
		//�J�v�Z���̕`����
		struct CapsuleInfo
		{
			LibVec3 center;      //���S���W
			LibVec3 vec;         //�x�N�^�[
			float len;           //����
			float radius;        //���a
			unsigned int color;  //�F
		};

		//�U���p�̕`����
		struct AttackCapsuleInfo
		{
			LibVec3 pos1;        //���W1
			LibVec3 pos2;        //���W2
			float radius;        //���a
			unsigned int color;  //�F
		};

		//���̂̕`����
		struct SphereInfo
		{
			LibVec3 center;      //���S���W
			float radius;        //���a
			unsigned int color;  //�F
		};

		//��`�̕`����
		struct RectInfo
		{
			LibVec3 center;      //���S���W
			LibVec3::Size size;        //�T�C�Y
			unsigned int color;  //�F
		};

		//�J�v�Z���̕`���񃊃X�g
		static std::list<CapsuleInfo> m_capsuleInfo;
		//�U���p�̃J�v�Z���̕`���񃊃X�g
		static std::list<AttackCapsuleInfo> m_attackCapsuleInfo;
		//���̂̕`���񃊃X�g
		static std::list<SphereInfo> m_sphereInfo;
		//��`�̕`���񃊃X�g
		static std::list<RectInfo> m_rectInfo;
	};
}



