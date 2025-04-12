#include "AttackLigObject.h"

AttackLigObject::AttackLigObject(MyLibrary::LibVec3 pos1, MyLibrary::LibVec3 pos2, float radius) :
	ObjectBase(Priority::Middle, ObjectTag::Attack),
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

AttackLigObject::~AttackLigObject()
{
}

void AttackLigObject::Init(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_isCollisionOn = true;

	m_pPhysics = physics;

	Collidable::Init(m_pPhysics);

	rigidbody->Init();
}

void AttackLigObject::Update(MyLibrary::LibVec3 pos1, MyLibrary::LibVec3 pos2)
{
	rigidbody->SetAttackPos1(pos1);
	rigidbody->SetAttackPos2(pos2);
}

void AttackLigObject::Finalize(std::shared_ptr<MyLibrary::Physics> physics)
{
	Collidable::Finalize(physics);
}

void AttackLigObject::CollisionEnd()
{
	if (m_isCollisionOn)
	{
		m_isCollisionOn = false;
		Finalize(m_pPhysics);
	}
}

void AttackLigObject::OnTriggerEnter(const std::shared_ptr<Collidable>& collidable)
{
	auto tag = collidable->GetTag();
	if (tag == ObjectTag::Enemy)
	{
		CollisionEnd();
		m_isCollisionOn = false;
	}
}

bool AttackLigObject::GetIsTrigger()
{
	return m_isCollisionOn;
}

void AttackLigObject::IsTriggerReset()
{
	m_isCollisionOn = false;
}

