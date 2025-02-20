#pragma once
#include "CollidableData.h"
#include "LibVec3.h"

namespace MyLibrary
{
	class CollidableDataAttackCapsule : public CollidableData
	{
	public:
		//�R���X�g���N�^
		CollidableDataAttackCapsule(bool isTrigger);

		//�|�W�V�����P
		LibVec3 m_pos1;

		//�|�W�V����2
		LibVec3 m_pos2;

		//���a
		float m_radius;

	};
}


