#pragma once
#include "CollidableData.h"
#include "LibVec3.h"

namespace MyLibrary
{
	class CollidableDataAttackCapsule : public CollidableData
	{
	public:
		//コンストラクタ
		CollidableDataAttackCapsule(bool isTrigger);

		//ポジション１
		LibVec3 m_pos1;

		//ポジション2
		LibVec3 m_pos2;

		//半径
		float m_radius;

	};
}


