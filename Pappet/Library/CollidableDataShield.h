#pragma once
#include "CollidableData.h"
#include "LibVec3.h"

namespace MyLibrary
{
	class CollidableDataShield : public CollidableData
	{
	public:
		//コンストラクタ
		CollidableDataShield(bool isTrigger);

		//サイズ
		LibVec3::Size m_size;
	};
}



