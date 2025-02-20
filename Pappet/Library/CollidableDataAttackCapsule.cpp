#include "CollidableDataAttackCapsule.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="isTrigger"></param>
MyLibrary::CollidableDataAttackCapsule::CollidableDataAttackCapsule(bool isTrigger) :
	CollidableData(CollidableData::Kind::AttackCapsule, isTrigger),
	m_pos1(),
	m_pos2(),
	m_radius(0.0f)
{
}
