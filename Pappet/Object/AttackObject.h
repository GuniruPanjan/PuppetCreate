#pragma once
#include "ObjectBase.h"
class AttackObject : public ObjectBase
{
public:
	AttackObject(float radius);
	~AttackObject();

	void Init(std::shared_ptr<MyLibrary::Physics> physics);
	void Update(MyLibrary::LibVec3 pos);

	void OnCollideEnter(const std::shared_ptr<Collidable>& collidable) {};

	//�����蔻����폜
	void Finalize(std::shared_ptr<MyLibrary::Physics> physics) override;

	void CollisionEnd();

	void OnTriggerEnter(const std::shared_ptr<Collidable>& collidable) override;
	//���̃I�u�W�F�N�g�ɓ����������ǂ���
	bool GetIsTrigger();
	//�U���͂�ݒ�
	int SetAttack(int attack) { return m_attack = attack; }
	//�U���͂��擾
	const int GetAttack() const { return m_attack; }
	//��������Z�b�g
	void IsTriggerReset();

private:
	std::shared_ptr<MyLibrary::Physics> m_pPhysics;

	//�����蔻��̐؂�ւ�
	bool m_isCollisionOn = false;

	int m_attack;
};

