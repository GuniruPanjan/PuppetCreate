#pragma once
#include "CollidableData.h"

namespace MyLibrary
{
	class CollidableDataSphere : public CollidableData
	{
	public:
		//コンストラクタ
		CollidableDataSphere(bool isTrigger);

		//半径
		float m_radius;
	};
}



