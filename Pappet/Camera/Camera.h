#pragma once
#include "DxLib.h"
#include "Library/MyLibrary.h"
#include <cmath>

class EnemyManager;
class Player;
class Map;

/// <summary>
/// �J�������Ǘ�����N���X
/// </summary>
class Camera
{
public:
	//�R���X�g���N�^
	Camera();
	//�f�X�g���N�^
	virtual ~Camera();

	//����������
	void Init();
	//�X�V����
	void Update(Player& player);
	//�G��I�����鏈��
	void SelectNextTarget();
	void SelectPreviousTarget();
	void FilterEnemiesInRange(Player& player, EnemyManager& enemy, float range);
	//���b�N�I������
	void LockUpdate(Player& player, EnemyManager& enemy);
	//�{�X�̃��b�N�I������
	void LockBossUpdate(Player& player, EnemyManager& enemy);
	//�`�揈��
	void Draw();
	//�I������
	void End();
	//�J�����������Ă�����x�N�g�����擾
	const MyLibrary::LibVec3 GetDirection() const;

	VECTOR GetAngle() { return m_cameraAngle; }         //�J�����̃A���O������

private:
	DINPUT_JOYSTATE input;

	//�J�����̃|�W�V����
	VECTOR m_cameraPos = VGet(0.0f, 0.0f, 0.0f);

	//�J�����̃^�[�Q�b�g
	VECTOR m_cameraTarget = VGet(0.0f, 0.0f, 0.0f);

	//�J�����̃A���O��
	VECTOR m_cameraAngle = VGet(0.0f, 0.0f, 0.0f);

	//�G�l�~�[�̃|�W�V�������
	VECTOR m_enemyPos = VGet(0.0f, 0.0f, 0.0f);

	//�J�����̖ڕW
	VECTOR m_endPos = VGet(0.0f, 0.0f, 0.0f);
	VECTOR m_endTargetPos = VGet(0.0f, 0.0f, 0.0f);

	//�C�[�W���O�̑���ϐ�
	MyLibrary::LibVec3 m_lerp;
	MyLibrary::LibVec3 m_lerpTarget;

	int m_currentTargetIndex;
	std::vector<MyLibrary::LibVec3> m_enemyPositions;
	std::vector<MyLibrary::LibVec3> m_filterEnemyPositions;

	float m_easingTime;
	float m_easingDuration;

	float m_x, m_z;
	float m_radius;

};