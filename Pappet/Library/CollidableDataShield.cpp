#include "CollidableDataShield.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="isTrigger"></param>
MyLibrary::CollidableDataShield::CollidableDataShield(bool isTrigger) :
	CollidableData(CollidableData::Kind::ShieldRect, isTrigger),
	m_size()
{
}
