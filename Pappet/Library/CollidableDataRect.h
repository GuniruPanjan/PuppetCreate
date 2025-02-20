#pragma once
#include "CollidableData.h"
#include "LibVec3.h"

namespace MyLibrary
{
	class CollidableDataRect : public CollidableData
	{
	public:
		//コンストラクタ
		CollidableDataRect(bool isTrigger);

		//サイズ
		LibVec3::Size m_size;
	};
}



