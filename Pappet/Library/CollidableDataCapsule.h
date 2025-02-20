#pragma once
#include "CollidableData.h"
#include "LibVec3.h"

namespace MyLibrary
{
	class CollidableDataCapsule : public CollidableData
	{
	public:
		//コンストラクタ
		CollidableDataCapsule(bool isTrigger);

		LibVec3 m_vec;

		//長さ
		float m_len;

		//半径
		float m_radius;
	};
}



