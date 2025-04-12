#include "RectObject.h"

RectObject::RectObject(float width, float hight, float depth) :
	ObjectBase(Priority::Static, ObjectTag::Rect),
	m_isEnter(false),
	m_isTriggerEnter(false)
{
	//当たり判定の設定
	auto collider = Collidable::AddCollider(MyLibrary::CollidableData::Kind::Rect, false);
	auto rectCol = dynamic_cast<MyLibrary::CollidableDataRect*>(collider.get());
	rectCol->m_size = MyLibrary::LibVec3::Size(width, hight, depth);
}

RectObject::~RectObject()
{
}

void RectObject::Init(std::shared_ptr<MyLibrary::Physics> physics, MyLibrary::LibVec3 pos, bool isEnter)
{
	m_pPhysics = physics;
	m_isEnter = isEnter;

	Collidable::Init(m_pPhysics);

	rigidbody->Init();
	rigidbody->SetPos(pos);
}

void RectObject::Update(MyLibrary::LibVec3 pos, MyLibrary::LibVec3::Size size)
{
	rigidbody->SetPos(pos);
	rigidbody->SetSize(size);
}

void RectObject::Finalize(const std::shared_ptr<MyLibrary::Physics> physics)
{
	Collidable::Finalize(physics);
}

void RectObject::OnTriggerEnter(const std::shared_ptr<Collidable>& collidable)
{
	//アタッチしたオブジェクトがボス部屋入り口なら
	if(m_isEnter)
	{
		auto tag = collidable->GetTag();
		if (tag == ObjectTag::Player)
		{
			m_isTriggerEnter = true;
		}
	}
}

bool RectObject::GetIsTrigger()
{
	return m_isTriggerEnter;
}

void RectObject::IsTriggerReset()
{
	m_isTriggerEnter = false;
}

