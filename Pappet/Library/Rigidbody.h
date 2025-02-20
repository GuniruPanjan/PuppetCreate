#pragma once
#include "DxLib.h"
#include "LibVec3.h"

namespace MyLibrary
{
	/// <summary>
	/// �����A�Փ˔���ɕK�v�ȃf�[�^�̉�
	/// </summary>
	class Rigidbody
	{
	public:
		//�R���X�g���N�^
		Rigidbody();

		//������
		void Init(bool gravity = false);

		//velocity�ɗ͂�������
		void AddForce(const LibVec3& force);

		//���ݍ��W���擾
		const LibVec3& GetPos() const { return m_pos; }
		//�傫�����擾
		const LibVec3::Size& GetSize() const { return m_size; }
		//�ړ��\����W
		const LibVec3& GetNextPos() const { return m_nextPos; }
		//����������
		const LibVec3& GetDir() const { return m_dir; }
		//�ړ����x������
		const LibVec3& GetVelocity() const { return m_velocity; }
		//�U���p�̃J�v�Z���̃|�W�V�������擾
		const LibVec3& GetAttackPos1() const { return m_pos1; }
		const LibVec3& GetAttackPos2() const { return m_pos2; }

		//���ݍ��W������
		const VECTOR GetPosVECTOR() { return m_pos.ConversionToVECTOR(); }
		//�ړ��\����W������
		const VECTOR GetNextPosVECTOR() { return m_nextPos.ConversionToVECTOR(); }
		//�x�N�^�[���擾
		const VECTOR GetVec() { return m_vec.ConversionToVECTOR(); }
		//����������
		const VECTOR GetDirVECTOR() { return m_dir.ConversionToVECTOR(); }
		//�ړ����x������
		const VECTOR GetVelocityVECTOR() { return m_velocity.ConversionToVECTOR(); }
		//�U���p�̃J�v�Z���̃|�W�V�������擾
		const VECTOR GetAttackPos1VECTOR() { return m_pos1.ConversionToVECTOR(); }
		const VECTOR GetAttackPos2VECTOR() { return m_pos2.ConversionToVECTOR(); }
		//�d�͂�^���邩�ǂ���������
		bool GetUseGravity() const { return m_useGravity; }


		//���ݍ��W��ݒ�
		void SetPos(const LibVec3& set) { m_pos = set; }
		//�ړ��\����W��ݒ�
		void SetNextPos(const LibVec3& set) { m_nextPos = set; }
		//�x�N�^�[��ݒ�
		void SetVec(const LibVec3& set) { m_vec = set; }
		//�傫����ݒ�
		void SetSize(const LibVec3::Size set) { m_size = set; }
		//������ݒ�
		void SetVelocity(LibVec3 set, float mul = 1.0f);
		//�ړ����x��ݒ�
		void SetUseGravity(bool set) { m_useGravity = set; }
		//�U���p�̃J�v�Z���̃|�W�V����
		void SetAttackPos1(const LibVec3& set) { m_pos1 = set; }
		void SetAttackPos2(const LibVec3& set) { m_pos2 = set; }

	private:
		//���ݍ��W
		LibVec3 m_pos;
		//�ړ��\����W
		LibVec3 m_nextPos;
		//�x�N�^�[
		LibVec3 m_vec;
		//�傫��
		LibVec3::Size m_size;
		//����
		LibVec3 m_dir;
		//�ړ����x
		LibVec3 m_velocity;
		//�U���p�̃J�v�Z���̃|�W�V����
		LibVec3 m_pos1, m_pos2;
		//�d�͂�^���邩���肷��
		bool m_useGravity;
		//����������Ȃ�����
		LibVec3 m_higth = MyLibrary::LibVec3(0.0f, 50.0f, 0.0f);
	};
}


