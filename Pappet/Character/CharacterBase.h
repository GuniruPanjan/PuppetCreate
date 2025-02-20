#pragma once
#include "Library/MyLibrary.h"
#include "Object/AttackObject.h"
#include "Object/SearchObject.h"
#include "Object/PlayerSearchObject.h"
#include "Object/EnemyAttackObject.h"
#include "Object/ShieldObject.h"
#include <map>
#include <string>
#define ANIMATION_MAX   30
#define D2R(deg) ((deg)*DX_PI_F/180.0f)

class CharacterBase : public MyLibrary::Collidable
{
public:
	//�X�e�[�^�X�\����
	struct Status
	{
		float s_hp;          //�̗�
		float s_stamina;     //�X�^�~�i
		float s_attack;      //�U����
		float s_attack1;     //�U����2
		float s_attack2;     //�U����
		int s_muscle;        //�ؗ�
		int s_skill;         //�Z��
		float s_defense;     //�h���
		float s_speed;       //���x
		int s_core;          //�R�A
	};

	//�A�j���[�V�����֌W�̍\����
	struct AnimationChange
	{
		bool s_moveflag;      //�L�����������������f����t���O
		bool s_isDead;        //�L�����̎��S����
		bool s_hit;           //���ݔ���
		bool s_attack;        //�L�������U���������̔���
	};

	//�G�t�F�N�g�֌W�̍\����
	struct EffectAction
	{
		bool s_heel;          //��
	};

public:
	CharacterBase(Priority priority, ObjectTag tag);
	virtual ~CharacterBase();

	//�Փ˂����Ƃ�
	virtual void OnCollideEnter(const std::shared_ptr<Collidable>& colider) = 0;

protected:
	//�A�j���[�V�����̍X�V
	bool UpdateAnim(int attachNo, int max, float startTime = 0.0f);
	//�A�j���[�V�����̕ύX
	void ChangeAnim(int animIndex, bool& one, bool(&all)[30], float animSpeed = 0.5f, bool reverse = false, float resetTime = 0.0f);
	//�A�j���[�V�����̃t���[���u�����h�ύX
	void FrameChangeAnim(int animIndex, bool& one, bool& two, int frame);
	//�A�j���[�V�����̃t���[���u�����h�ύX
	void FrameEndAnim(int animIndex, bool& one, bool& two, int frame);
	//�A�j���[�V�����̖�������
	void NotInitAnim(bool init = false);


protected:
	//�����N���X�̃|�C���^
	std::shared_ptr<MyLibrary::Physics> m_pPhysics;
	//���f���n���h��
	int m_modelHandle;
	//���f�����W
	MyLibrary::LibVec3 m_modelPos;
	//�����蔻����W
	MyLibrary::LibVec3 m_collisionPos;
	//�ړ��x�N�g��
	MyLibrary::LibVec3 m_moveVec;
	//�X�e�[�^�X
	Status m_status;
	//�A�j���[�V����
	AnimationChange m_anim;
	//�G�t�F�N�g
	EffectAction m_effect;

	//�A�j���[�V�����֌W
	std::map<std::string, int> m_animIdx;
	int m_nowAnimIdx;        
	int m_equipAnimNo;        //��O�̃A�j���[�V����
	int m_nowAnimNo;          //���݂̃A�j���[�V����
	int m_prevAnimNo;         //�ύX�O�̃A�j���[�V����
	int m_frameAnimNo;        //�t���[���������s���A�j���[�V����
	float m_animBlendRate;    //�A�j���[�V�����̍�������
	bool m_animOne[ANIMATION_MAX];      //�A�j���[�V�����֌W�����Z�b�g���邽�߂̔���
	float m_nowFrame;         //�A�j���[�V������i�߂鎞��
	float m_animTime;         //�A�j���[�V�����Đ����x
	bool m_isAnimationFinish; //�A�j���[�V�������I��������ǂ���
	float m_animSpeed;        //�A�j���[�V�����̐i�񂾃^�C��������
	bool m_animInit;          //�A�j���[�V���������������邩�ǂ���
	bool m_animReverse;       //�A�j���[�V�������t�Đ������邩�ǂ���
	VECTOR m_nowPos;        //���݂̃t���[���̍��W���擾����

	//�g���ϐ�
	float m_angle;            //�L�����̃A���O��
	float m_attackRadius;     //�U���̓����蔻��
	float m_searchRadius;     //���G�̓����蔻��
	int m_heel;               //HP�ɒǉ�����񕜕�
	int m_maxHeel;            //HP�̍ő�񕜗�
	
};

