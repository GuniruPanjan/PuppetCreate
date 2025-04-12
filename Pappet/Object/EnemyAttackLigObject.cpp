#include "EnemyAttackLigObject.h"

EnemyAttackLigObject::EnemyAttackLigObject(MyLibrary::LibVec3 pos1, MyLibrary::LibVec3 pos2, float radius) :
	ObjectBase(Priority::Middle, ObjectTag::EnemyAttack),
	m_isCollisionOn(false),
	m_attack(0)
{
	//ìñÇΩÇËîªíËÇÃê›íË
	auto collider = Collidable::AddCollider(MyLibrary::CollidableData::Kind::AttackCapsule, true);
	auto capsuleCol = dynamic_cast<MyLibrary::CollidableDataAttackCapsule*>(collider.get());
	capsuleCol->m_pos1 = pos1;
	capsuleCol->m_pos2 = pos2;
	capsuleCol->m_radius = radius;
}

EnemyAttackLigObject::~EnemyAttackLigObject()
{
}

void EnemyAttackLigObject::Init(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_isCollisionOn = true;

	m_pPhysics = physics;

	Collidable::Init(m_pPhysics);

	rigidbody->Init();
}

void EnemyAttackLigObject::Update(MyLibrary::LibVec3 pos1, MyLibrary::LibVec3 pos2)
{
	rigidbody->SetAttackPos1(pos1);
	rigidbody->SetAttackPos2(pos2);
}

void EnemyAttackLigObject::Finalize(std::shared_ptr<MyLibrary::Physics> physics)
{
	Collidable::Finalize(physics);
}

void EnemyAttackLigObject::CollisionEnd()
{
	if (m_isCollisionOn)
	{
		m_isCollisionOn = false;
		Finalize(m_pPhysics);
	}
}

void EnemyAttackLigObject::OnTriggerEnter(const std::shared_ptr<Collidable>& collidable)
{
	auto tag = collidable->GetTag();
	if (tag == ObjectTag::Player || tag == ObjectTag::Shield)
	{
		CollisionEnd();
		m_isCollisionOn = false;
	}
}

bool EnemyAttackLigObject::GetIsTrigger()
{
	return m_isCollisionOn;
}

void EnemyAttackLigObject::IsTriggerReset()
{
	m_isCollisionOn = false;
}
