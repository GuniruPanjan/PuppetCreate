#include "AttackObject.h"

AttackObject::AttackObject(float radius) :
	ObjectBase(Priority::Middle, ObjectTag::Attack),
	m_isCollisionOn(false),
	m_attack(0)
{
	//ƒJƒvƒZƒ‹Œ^‚É‚µ‚Ä˜r‚É“–‚½‚è”»’è‚ğ‚Â‚¯‚éy•Ší‚É‚àz
	//“–‚½‚è”»’è‚Ìİ’è
	auto collider = Collidable::AddCollider(MyLibrary::CollidableData::Kind::Sphere, true);
	auto sphereCol = dynamic_cast<MyLibrary::CollidableDataSphere*>(collider.get());
	sphereCol->m_radius = radius;
}

AttackObject::~AttackObject()
{
}

void AttackObject::Init(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_isCollisionOn = true;

	m_pPhysics = physics;

	Collidable::Init(m_pPhysics);

	rigidbody.Init();
}

void AttackObject::Update(MyLibrary::LibVec3 pos)
{
	rigidbody.SetPos(pos);
}

void AttackObject::Finalize(std::shared_ptr<MyLibrary::Physics> physics)
{
	Collidable::Finalize(physics);
}

void AttackObject::CollisionEnd()
{
	if (m_isCollisionOn)
	{
		m_isCollisionOn = false;
		Finalize(m_pPhysics);
	}
}

void AttackObject::OnTriggerEnter(const std::shared_ptr<Collidable>& collidable)
{
	auto tag = collidable->GetTag();
	if (tag == ObjectTag::Enemy)
	{
		CollisionEnd();
		m_isCollisionOn = false;
	}
}

bool AttackObject::GetIsTrigger()
{
	return m_isCollisionOn;
}

void AttackObject::IsTriggerReset()
{
	m_isCollisionOn = false;
}

