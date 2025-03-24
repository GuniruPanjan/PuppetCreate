#include "ShieldObject.h"

ShieldObject::ShieldObject(float width, float hight, float depth) :
    ObjectBase(Priority::Static, ObjectTag::Shield),
    m_isEnemy(false),
    m_isTriggerEnter(false),
    m_isTriggerStay(false),
    m_isTriggerExit(false),
    m_isCollisionOn(false)
{
    //当たり判定の設定
    auto collider = Collidable::AddCollider(MyLibrary::CollidableData::Kind::ShieldRect, true);
    auto rectCol = dynamic_cast<MyLibrary::CollidableDataShield*>(collider.get());
    rectCol->m_size = MyLibrary::LibVec3::Size(width, hight, depth);
}


ShieldObject::~ShieldObject()
{
}

void ShieldObject::Init(std::shared_ptr<MyLibrary::Physics> physics, MyLibrary::LibVec3 pos, bool isEnemy)
{
    m_isCollisionOn = true;

    m_pPhysics = physics;
    m_isEnemy = isEnemy;

    Collidable::Init(m_pPhysics);

    rigidbody.Init();
    rigidbody.SetPos(pos);
}

void ShieldObject::Update(MyLibrary::LibVec3 pos, MyLibrary::LibVec3::Size size)
{
    m_isTriggerStay = false;
    rigidbody.SetPos(pos);
    rigidbody.SetSize(size);
}

void ShieldObject::Finalize(const std::shared_ptr<MyLibrary::Physics> physics)
{
    Collidable::Finalize(physics);
}

void ShieldObject::CollisionEnd()
{
    if (m_isCollisionOn)
    {
        Finalize(m_pPhysics);
        m_isCollisionOn = false;
    }
}

void ShieldObject::OnTriggerEnter(const std::shared_ptr<Collidable>& collidable)
{
    //アタッチしたオブジェクトが敵なら
    if (m_isEnemy)
    {
        auto tag = collidable->GetTag();
        if (tag == ObjectTag::Attack)
        {
            m_isTriggerEnter = true;
        }
        else if (tag == ObjectTag::StrengthAttack)
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

void ShieldObject::OnTriggerStay(const std::shared_ptr<Collidable>& collidable)
{
    //アタッチしたオブジェクトが敵なら
    if (m_isEnemy)
    {
        auto tag = collidable->GetTag();
        if (tag == ObjectTag::Attack)
        {
            m_isTriggerStay = true;
        }
        else if (tag == ObjectTag::StrengthAttack)
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

void ShieldObject::OnTriggerExit(const std::shared_ptr<Collidable>& collidable)
{
    //アタッチしたオブジェクトが敵なら
    if (m_isEnemy)
    {
        auto tag = collidable->GetTag();
        if (tag == ObjectTag::Attack)
        {
            m_isTriggerExit = true;
        }
        else if (tag == ObjectTag::StrengthAttack)
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

bool ShieldObject::GetIsTrigger()
{
    return m_isTriggerEnter;
}

bool ShieldObject::GetIsStay() const
{
    return m_isTriggerStay;
}

bool ShieldObject::GetIsExit()
{
    return m_isTriggerExit;
}

void ShieldObject::IsTriggerReset()
{
    m_isTriggerEnter = false;
}
