#include "ItemObject.h"

ItemObject::ItemObject(float radius) :
	ObjectBase(Priority::Low, ObjectTag::Item),
	m_isTriggerEnter(false),
	m_isTriggerStay(false),
	m_isTriggerExit(false),
	m_pPhysics()
{
	//ìñÇΩÇËîªíËÇÃê›íË
	auto collider = Collidable::AddCollider(MyLibrary::CollidableData::Kind::Sphere, true);
	auto sphereCol = dynamic_cast<MyLibrary::CollidableDataSphere*>(collider.get());
	sphereCol->m_radius = radius;
}

ItemObject::~ItemObject()
{
}

void ItemObject::Init(std::shared_ptr<MyLibrary::Physics> physics, MyLibrary::LibVec3 pos)
{
	m_pPhysics = physics;

	Collidable::Init(m_pPhysics);

	rigidbody->Init();
	rigidbody->SetPos(pos);
}

void ItemObject::Update(MyLibrary::LibVec3 pos)
{
	m_isTriggerStay = false;
	rigidbody->SetPos(pos);
}

void ItemObject::Finalize(std::shared_ptr<MyLibrary::Physics> physics)
{
	Collidable::Finalize(physics);
}

void ItemObject::OnTriggerEnter(const std::shared_ptr<Collidable>& collidable)
{
	auto tag = collidable->GetTag();
	if (tag == ObjectTag::Player)
	{
		m_isTriggerEnter = true;
	}
}

void ItemObject::OnTriggerStay(const std::shared_ptr<Collidable>& collidable)
{
	auto tag = collidable->GetTag();
	if (tag == ObjectTag::Player)
	{
		m_isTriggerStay = true;
	}
}

void ItemObject::OnTriggerExit(const std::shared_ptr<Collidable>& collidable)
{
	auto tag = collidable->GetTag();
	if (tag == ObjectTag::Player)
	{
		m_isTriggerExit = true;
	}
}

bool ItemObject::GetIsTrigger()
{
	return m_isTriggerEnter;
}

bool ItemObject::GetIsStay() const
{
	return m_isTriggerStay;
}

bool ItemObject::GetIsExit()
{
	return m_isTriggerExit;
}

void ItemObject::IsTriggerReset()
{
	m_isTriggerEnter = false;
}
