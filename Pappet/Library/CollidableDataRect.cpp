#include "CollidableDataRect.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="isTrigger"></param>
MyLibrary::CollidableDataRect::CollidableDataRect(bool isTrigger):
	CollidableData(CollidableData::Kind::Rect, isTrigger),
	m_size()
{
}
