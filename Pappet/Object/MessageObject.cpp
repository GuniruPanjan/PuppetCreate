#include "MessageObject.h"

MessageObject::MessageObject(float radius) :
	ObjectBase(Priority::Low, ObjectTag::Message),
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

MessageObject::~MessageObject()
{
}

void MessageObject::Init(std::shared_ptr<MyLibrary::Physics> physics, MyLibrary::LibVec3 pos)
{
	m_pPhysics = physics;

	Collidable::Init(m_pPhysics);

	rigidbody.Init();
	rigidbody.SetPos(pos);
}

void MessageObject::Update(MyLibrary::LibVec3 pos)
{
	m_isTriggerStay = false;
	rigidbody.SetPos(pos);
}

void MessageObject::Finalize(std::shared_ptr<MyLibrary::Physics> physics)
{
	Collidable::Finalize(physics);
}

void MessageObject::OnTriggerEnter(const std::shared_ptr<Collidable>& collidable)
{
	auto tag = collidable->GetTag();
	if (tag == ObjectTag::Player)
	{
		m_isTriggerEnter = true;
	}
}

void MessageObject::OnTriggerStay(const std::shared_ptr<Collidable>& collidable)
{
	auto tag = collidable->GetTag();
	if (tag == ObjectTag::Player)
	{
		m_isTriggerStay = true;
	}
}

void MessageObject::OnTriggerExit(const std::shared_ptr<Collidable>& collidable)
{
	auto tag = collidable->GetTag();
	if (tag == ObjectTag::Player)
	{
		m_isTriggerExit = true;
	}
}

bool MessageObject::GetIsTrigger()
{
	return m_isTriggerEnter;
}

bool MessageObject::GetIsStay() const
{
	return m_isTriggerStay;
}

bool MessageObject::GetIsExit()
{
	return m_isTriggerExit;
}

void MessageObject::IsTriggerReset()
{
	m_isTriggerEnter = false;
}
