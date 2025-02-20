#include "SearchObject.h"

SearchObject::SearchObject(float radius) :
	ObjectBase(Priority::Low, ObjectTag::EnemySearch),
	m_isEnemy(false),
	m_isRest(false),
	m_isItem(false),
	m_isBossRoom(false),
	m_isCore(false),
	m_isTriggerEnter(false),
	m_isTriggerStay(false),
	m_isTriggerExit(false),
	m_pPhysics()
{
	//�����蔻��̐ݒ�
	auto collider = Collidable::AddCollider(MyLibrary::CollidableData::Kind::Sphere, true);
	auto sphereCol = dynamic_cast<MyLibrary::CollidableDataSphere*>(collider.get());
	sphereCol->m_radius = radius;
}

SearchObject::~SearchObject()
{
}

void SearchObject::Init(std::shared_ptr<MyLibrary::Physics> physics, MyLibrary::LibVec3 pos, bool isEnemy, bool isRest, bool isItem, bool isBossRoom, bool isCore)
{
	m_pPhysics = physics;
	m_isEnemy = isEnemy;
	m_isRest = isRest;
	m_isItem = isItem;
	m_isBossRoom = isBossRoom;
	m_isCore = isCore;

	Collidable::Init(m_pPhysics);

	rigidbody.Init();
	rigidbody.SetPos(pos);
}

void SearchObject::Update(MyLibrary::LibVec3 pos)
{
	m_isTriggerStay = false;
	rigidbody.SetPos(pos);
}

void SearchObject::Finalize(std::shared_ptr<MyLibrary::Physics> physics)
{
	Collidable::Finalize(physics);
}

void SearchObject::OnTriggerEnter(const std::shared_ptr<Collidable>& collidable)
{
	//�A�^�b�`�����I�u�W�F�N�g���G�Ȃ�
	if (m_isEnemy)
	{
		auto tag = collidable->GetTag();
		if (tag == ObjectTag::Player)
		{
			m_isTriggerEnter = true;
		}
	}
	//�A�^�b�`�����I�u�W�F�N�g���x���|�C���g�Ȃ�
	else if (m_isRest)
	{
		auto tag = collidable->GetTag();
		if (tag == ObjectTag::Player)
		{
			m_isTriggerEnter = true;
		}
	}
	//�A�^�b�`�����I�u�W�F�N�g���A�C�e���Ȃ�
	else if (m_isItem)
	{
		auto tag = collidable->GetTag();
		if (tag == ObjectTag::Player)
		{
			m_isTriggerEnter = true;
		}
	}
	//�A�^�b�`�����I�u�W�F�N�g��������Ȃ�
	else if (m_isBossRoom)
	{
		auto tag = collidable->GetTag();
		if (tag == ObjectTag::Player)
		{
			m_isTriggerEnter = true;
		}
	}
	//�A�^�b�`�����I�u�W�F�N�g���R�A�Ȃ�
	else if (m_isCore)
	{
		auto tag = collidable->GetTag();
		if (tag == ObjectTag::Player)
		{
			m_isTriggerEnter = true;
		}
	}
	
}

void SearchObject::OnTriggerStay(const std::shared_ptr<Collidable>& collidable)
{
	//�A�^�b�`�����I�u�W�F�N�g���G�Ȃ�
	if (m_isEnemy)
	{
		auto tag = collidable->GetTag();
		if (tag == ObjectTag::Player)
		{
			m_isTriggerStay = true;
		}
	}
	//�A�^�b�`�����I�u�W�F�N�g���x���|�C���g�Ȃ�
	else if (m_isRest)
	{
		auto tag = collidable->GetTag();
		if (tag == ObjectTag::Player)
		{
			m_isTriggerStay = true;
		}
	}
	//�A�^�b�`�����I�u�W�F�N�g���A�C�e���Ȃ�
	else if (m_isItem)
	{
		auto tag = collidable->GetTag();
		if (tag == ObjectTag::Player)
		{
			m_isTriggerStay = true;
		}
	}
	//�A�^�b�`�����I�u�W�F�N�g���R�A�Ȃ�
	else if (m_isCore)
	{
		auto tag = collidable->GetTag();
		if (tag == ObjectTag::Player)
		{
			m_isTriggerStay = true;
		}
	}
}

void SearchObject::OnTriggerExit(const std::shared_ptr<Collidable>& collidable)
{
	//�A�^�b�`�����I�u�W�F�N�g���G�Ȃ�
	if (m_isEnemy)
	{
		auto tag = collidable->GetTag();
		if (tag == ObjectTag::Player)
		{
			m_isTriggerExit = true;
		}
	}
	//�A�^�b�`�����I�u�W�F�N�g���x���|�C���g�Ȃ�
	else if (m_isRest)
	{
		auto tag = collidable->GetTag();
		if (tag == ObjectTag::Player)
		{
			m_isTriggerExit = true;
		}
	}
	//�A�^�b�`�����I�u�W�F�N�g���A�C�e���Ȃ�
	else if (m_isItem)
	{
		auto tag = collidable->GetTag();
		if (tag == ObjectTag::Player)
		{
			m_isTriggerExit = true;
		}
	}
	//�A�^�b�`�����I�u�W�F�N�g���R�A�Ȃ�
	else if (m_isCore)
	{
		auto tag = collidable->GetTag();
		if (tag == ObjectTag::Player)
		{
			m_isTriggerExit = true;
		}
	}

}

bool SearchObject::GetIsTrigger()
{
	return m_isTriggerEnter;
}

bool SearchObject::GetIsStay() const
{
	return m_isTriggerStay;
}

bool SearchObject::GetIsExit()
{
	return m_isTriggerExit;
}

void SearchObject::IsTriggerReset()
{
	m_isTriggerEnter = false;
}
