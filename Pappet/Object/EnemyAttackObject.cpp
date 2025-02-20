#include "EnemyAttackObject.h"

EnemyAttackObject::EnemyAttackObject(float radius) :
	ObjectBase(Priority::Middle, ObjectTag::EnemyAttack),
	m_isCollisionOn(false),
	m_attack(0)
{
	//ìñÇΩÇËîªíËÇÃê›íË
	auto collider = Collidable::AddCollider(MyLibrary::CollidableData::Kind::Sphere, true);
	auto sphereCol = dynamic_cast<MyLibrary::CollidableDataSphere*>(collider.get());
	sphereCol->m_radius = radius;
}

EnemyAttackObject::~EnemyAttackObject()
{
}

void EnemyAttackObject::Init(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_isCollisionOn = true;

	m_pPhysics = physics;

	Collidable::Init(m_pPhysics);

	rigidbody.Init();
}

void EnemyAttackObject::Update(MyLibrary::LibVec3 pos)
{
	rigidbody.SetPos(pos);

}

void EnemyAttackObject::Finalize(std::shared_ptr<MyLibrary::Physics> physics)
{
	//ìoò^Ç≥ÇÍÇƒÇ¢Ç»Ç¢
	Collidable::Finalize(physics);
}

void EnemyAttackObject::CollisionEnd()
{
	if (m_isCollisionOn)
	{
		m_isCollisionOn = false;
		Finalize(m_pPhysics);
	}
}

void EnemyAttackObject::OnTriggerEnter(const std::shared_ptr<Collidable>& collidable)
{
	auto tag = collidable->GetTag();
	if (tag == ObjectTag::Player || tag == ObjectTag::Shield)
	{
		CollisionEnd();
		m_isCollisionOn = false;
	}
}

bool EnemyAttackObject::GetIsTrigger()
{
	return m_isCollisionOn;

}

void EnemyAttackObject::IsTriggerReset()
{
	m_isCollisionOn = false;

}
