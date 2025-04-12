#include "PlayerSearchObject.h"

PlayerSearchObject::PlayerSearchObject(float radius) :
	ObjectBase(Priority::Low, ObjectTag::Search),
	m_isTriggerEnter(false),
	m_isTriggerExit(false),
	m_isTriggerStay(false),
	m_isCollisionOn(false)
{
	//ìñÇΩÇËîªíËÇÃê›íË
	auto collider = Collidable::AddCollider(MyLibrary::CollidableData::Kind::Sphere, true);
	auto sphereCol = dynamic_cast<MyLibrary::CollidableDataSphere*>(collider.get());
	sphereCol->m_radius = radius;
}

PlayerSearchObject::~PlayerSearchObject()
{
}

void PlayerSearchObject::Init(std::shared_ptr<MyLibrary::Physics> physics, MyLibrary::LibVec3 pos)
{
	m_isCollisionOn = true;

	m_pPhysics = physics;

	Collidable::Init(m_pPhysics);

	rigidbody->Init();
	rigidbody->SetPos(pos);
}

void PlayerSearchObject::Update(MyLibrary::LibVec3 pos)
{
	m_isTriggerStay = false;
	rigidbody->SetPos(pos);
}

void PlayerSearchObject::Finalize(std::shared_ptr<MyLibrary::Physics> physics)
{
	Collidable::Finalize(physics);

}

void PlayerSearchObject::CollisionEnd()
{
	if (m_isCollisionOn)
	{
		Finalize(m_pPhysics);
		m_isCollisionOn = false;
	}
}

void PlayerSearchObject::OnTriggerEnter(const std::shared_ptr<Collidable>& collidable)
{
	auto tag = collidable->GetTag();
	if (tag == ObjectTag::Enemy)
	{
		m_isTriggerEnter = true;
	}
}

void PlayerSearchObject::OnTriggerStay(const std::shared_ptr<Collidable>& collidable)
{
	auto tag = collidable->GetTag();
	if (tag == ObjectTag::Enemy)
	{
		m_isTriggerStay = true;
	}
}

void PlayerSearchObject::OnTriggerExit(const std::shared_ptr<Collidable>& collidable)
{
	auto tag = collidable->GetTag();
	if (tag == ObjectTag::Enemy)
	{
		m_isTriggerExit = true;
	}
}

bool PlayerSearchObject::GetIsTrigger()
{
	return m_isTriggerEnter;

}

bool PlayerSearchObject::GetIsStay() const
{
	return m_isTriggerStay;

}

bool PlayerSearchObject::GetIsExit()
{
	return m_isTriggerExit;

}

void PlayerSearchObject::IsTriggerReset()
{
	m_isTriggerEnter = false;

}
