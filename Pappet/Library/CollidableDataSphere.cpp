#include "CollidableDataSphere.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="isTrigger"></param>
MyLibrary::CollidableDataSphere::CollidableDataSphere(bool isTrigger):
	CollidableData(CollidableData::Kind::Sphere, isTrigger),
	m_radius(0.0f)
{
}
