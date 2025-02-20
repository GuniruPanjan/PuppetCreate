#include "CollidableNpc.h"

CollidableNpc::CollidableNpc() :
	CharacterBase(Collidable::Priority::Low, ObjectTag::Enemy)
{
	//カプセル型
	auto collider = Collidable::AddCollider(MyLibrary::CollidableData::Kind::Capsule, false);
	auto capsuleCol = dynamic_cast<MyLibrary::CollidableDataCapsule*>(collider.get());
	capsuleCol->m_len = 50.0f;
	capsuleCol->m_radius = 12.0f;
	capsuleCol->m_vec = MyLibrary::LibVec3(0.0f, capsuleCol->m_vec.y + 2.0f, 0.0f);
}

CollidableNpc::~CollidableNpc()
{
}

void CollidableNpc::Init(std::shared_ptr<MyLibrary::Physics> physics)
{
	m_pPhysics = physics;
	//当たり判定の追加
	Collidable::Init(m_pPhysics);

	//初期位置設定
	rigidbody.Init(true);
	rigidbody.SetPos(MyLibrary::LibVec3(485.0f, 12.0f, -100.0f));
	rigidbody.SetNextPos(rigidbody.GetPos());
	m_collisionPos = rigidbody.GetPos();
	SetModelPos();
}

void CollidableNpc::Update()
{
}

void CollidableNpc::Draw()
{
	rigidbody.SetPos(rigidbody.GetNextPos());
	m_collisionPos = rigidbody.GetPos();

}

void CollidableNpc::OnCollideEnter(const std::shared_ptr<Collidable>& collidable)
{
}

void CollidableNpc::OnTriggerEnter(const std::shared_ptr<Collidable>& collidable)
{
}

void CollidableNpc::SetModelPos()
{
}
