#include "RectObjectTrigger.h"

RectObjectTrigger::RectObjectTrigger(float width, float hight, float depth) :
    ObjectBase(Priority::Static, ObjectTag::Rect),
    m_isEnter(false),
    m_isTriggerEnter(false),
    m_isTriggerStay(false),
    m_isTriggerExit(false),
    m_isCollisionOn(false)
{
    //当たり判定の設定
    auto collider = Collidable::AddCollider(MyLibrary::CollidableData::Kind::Rect, true);
    auto rectCol = dynamic_cast<MyLibrary::CollidableDataRect*>(collider.get());
    rectCol->m_size = MyLibrary::LibVec3::Size(width, hight, depth);
}

RectObjectTrigger::~RectObjectTrigger()
{
}

void RectObjectTrigger::Init(std::shared_ptr<MyLibrary::Physics> physics, MyLibrary::LibVec3 pos, bool isEnter)
{
    m_isCollisionOn = true;

    m_pPhysics = physics;
    m_isEnter = isEnter;

    Collidable::Init(m_pPhysics);

    rigidbody.Init();
    rigidbody.SetPos(pos);
}

void RectObjectTrigger::Update(MyLibrary::LibVec3 pos, MyLibrary::LibVec3::Size size)
{
    m_isTriggerStay = false;
    rigidbody.SetPos(pos);
    rigidbody.SetSize(size);
}

void RectObjectTrigger::Finalize(const std::shared_ptr<MyLibrary::Physics> physics)
{
    Collidable::Finalize(physics);

}

void RectObjectTrigger::CollisionEnd()
{
    if (m_isCollisionOn)
    {
        Finalize(m_pPhysics);
        m_isCollisionOn = false;
    }
}

void RectObjectTrigger::OnTriggerEnter(const std::shared_ptr<Collidable>& collidable)
{
    //アタッチしたオブジェクトがボス部屋入り口なら
    if (m_isEnter)
    {
        auto tag = collidable->GetTag();
        if (tag == ObjectTag::Player)
        {
            m_isTriggerEnter = true;
        }
    }
    else
    {
        auto tag = collidable->GetTag();
        if (tag == ObjectTag::EnemyAttack)
        {
            m_isTriggerEnter = true;
        }
    }
}

void RectObjectTrigger::OnTriggerStay(const std::shared_ptr<Collidable>& collidable)
{
    //アタッチしたオブジェクトがボス部屋入り口なら
    if (m_isEnter)
    {
        auto tag = collidable->GetTag();
        if (tag == ObjectTag::Player)
        {
            m_isTriggerStay = true;
        }
    }
    else
    {
        auto tag = collidable->GetTag();
        if (tag == ObjectTag::EnemyAttack)
        {
            m_isTriggerStay = true;
        }
    }
}

void RectObjectTrigger::OnTriggerExit(const std::shared_ptr<Collidable>& collidable)
{
    //アタッチしたオブジェクトがボス部屋入り口なら
    if (m_isEnter)
    {
        auto tag = collidable->GetTag();
        if (tag == ObjectTag::Player)
        {
            m_isTriggerExit = true;
        }
    }
    else
    {
        auto tag = collidable->GetTag();
        if (tag == ObjectTag::EnemyAttack)
        {
            m_isTriggerExit = true;
        }
    }
}

bool RectObjectTrigger::GetIsTrigger()
{
    return m_isTriggerEnter;

}

bool RectObjectTrigger::GetIsStay() const
{
    return m_isTriggerStay;
}

bool RectObjectTrigger::GetIsExit()
{
    return m_isTriggerExit;
}

void RectObjectTrigger::IsTriggerReset()
{
    m_isTriggerEnter = false;
}
