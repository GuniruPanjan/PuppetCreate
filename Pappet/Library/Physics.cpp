#include "MyLibrary.h"
#include "DxLib.h"
#include <cassert>

namespace
{
	//�ő哖���蔻��|���S����
	constexpr int kMaxColHitPoly = 3000;
	//�ǉ����o�������̍ő厎�s��
	constexpr int kMaxColHitTry = 20;
	//�ǉ����o�����ɃX���C�h�����鋗��
	constexpr float kColHitSlideLength = 1.0f;
	//�ǃ|���S�������|���S�����𔻒f���邽�߂̕ϐ�
	constexpr float kWallPolyBorder = 0.4f;
	//�ǃ|���S���Ɣ��f���邽�߂̍����ϐ�
	constexpr float kWallPolyHeight = 1.0f;

	//�d��
	constexpr float kGravity = -0.018f;
	//�ő�d�͉����x
	constexpr float kMaxGravity = -2.0f;

	//�␳�O���F
	const int kBeforeColor = 0xffffff;
	//�␳�O�\����F
	const int kBeforePlanColor = 0x00ff00;
	//�␳����F
	const int kAfterColor = 0xffff00;

	float cSlidex = 0.0f;
	float cSlidez = 0.0f;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="mapStageCollisionHandle">�L�����N�^�[�̃X�e�[�W�����蔻��n���h��</param>
MyLibrary::Physics::Physics(int mapStageCollisionHandle) :
	m_ret(VGet(0.0f,0.0f,0.0f)),
	m_isHitWallFlag(false)
{
	m_stageCollisionHandle = mapStageCollisionHandle;
}

/// <summary>
/// �Փ˕��̓o�^
/// </summary>
/// <param name="collidable">�ǉ����铖���蔻��</param>
void MyLibrary::Physics::Entry(std::shared_ptr<Collidable> collidable)
{
	//�o�^
	bool found = (std::find(m_collidables.begin(), m_collidables.end(), collidable) != m_collidables.end());

	if (!found)
	{
		m_collidables.emplace_back(collidable);
	}
	//���ɓo�^����Ă���G���[
	else
	{
		assert(0 && "�w���collidable�͓o�^�ς݂ł�");
	}

}

/// <summary>
/// �Փ˕��̓o�^�폜
/// </summary>
/// <param name="collidable">�폜���铖���蔻��</param>
void MyLibrary::Physics::Exit(std::shared_ptr<Collidable> collidable)
{
	bool found = (std::find(m_collidables.begin(), m_collidables.end(), collidable) != m_collidables.end());
	if (found)
	{
		m_collidables.remove(collidable);
	}
	//�o�^����Ă��Ȃ�������G���[
	else
	{
		assert(0 && "�w���collidable���o�^����Ă��܂���");
	}

}

/// <summary>
/// �X�V
/// </summary>
void MyLibrary::Physics::Update()
{
	//�����蔻��̃��O���c��
	m_preCollideInfo = m_newCollideInfo;
	m_newCollideInfo.clear();
	m_preTriggerInfo = m_newTriggerInfo;
	m_newTriggerInfo.clear();
	m_onCollideInfo.clear();

	//�ړ�
	for (auto& item : m_collidables)
	{
		auto pos = item->rigidbody.GetPos();
		auto size = item->rigidbody.GetSize();
		auto velocity = item->rigidbody.GetVelocity();

		//�d�͂𗘗p����ݒ�Ȃ�A�d�͂�ǉ�
		if (item->rigidbody.GetUseGravity())
		{
			velocity = velocity + LibVec3(0.0f, kGravity, 0.0f);

			//�ő�d�͂��傫��������
			if (velocity.y < kMaxGravity)
			{
				velocity = LibVec3(velocity.x, kMaxGravity, velocity.z);
			}
		}

		auto nextPos = pos + velocity;

		item->rigidbody.SetVelocity(velocity);

		//���Ƃ��Ƃ̏��A�\������f�o�b�O�\��
#if _DEBUG

		for (const auto& collider : item->m_colliders)
		{
			auto kind = collider->GetKind();
			if (kind == CollidableData::Kind::Capsule)
			{
				auto capsuleData = dynamic_cast<MyLibrary::CollidableDataCapsule*> (collider.get());
				auto vec = capsuleData->m_vec;
				auto len = capsuleData->m_len;
				auto radius = capsuleData->m_radius;
				//�΂��R���W����
				MyLibrary::DebugDraw::AddDrawCapsule(pos, vec, radius, len, kBeforeColor);
				//�����|�W�V����
				MyLibrary::DebugDraw::AddDrawCapsule(nextPos, vec, radius, len, kBeforePlanColor);
			}
			else if (kind == CollidableData::Kind::AttackCapsule)
			{
				auto attackCapsuleData = dynamic_cast<MyLibrary::CollidableDataAttackCapsule*> (collider.get());
				auto pos1 = attackCapsuleData->m_pos1;
				auto pos2 = attackCapsuleData->m_pos2;
				auto radius = attackCapsuleData->m_radius;
				MyLibrary::DebugDraw::AddDrawAttackCapsule(pos1, pos2, radius, kBeforeColor);
			}
			else if (kind == CollidableData::Kind::Sphere)
			{
				auto sphereData = dynamic_cast<MyLibrary::CollidableDataSphere*> (collider.get());
				auto radius = sphereData->m_radius;
				MyLibrary::DebugDraw::AddDrawSphere(pos, radius, kBeforeColor);
				MyLibrary::DebugDraw::AddDrawSphere(pos, radius, kBeforePlanColor);
			}
			else if (kind == CollidableData::Kind::Rect)
			{
				auto rectData = dynamic_cast<MyLibrary::CollidableDataRect*> (collider.get());
				//auto size = rectData->m_size;
				MyLibrary::DebugDraw::AddDrawRect(pos, size, kBeforeColor);
				MyLibrary::DebugDraw::AddDrawRect(pos, size, kBeforePlanColor);
			}

		}
#endif
		//�\��|�W�V�����ݒ�
		item->rigidbody.SetNextPos(nextPos);

	}

	//�����蔻��`�F�b�N�inextPos�w��j
	CheckCollide();
	CheckCollide1();
	CheckCollide2();
	CheckCollide3();

	CheckUpdate();

	CheckSendOnCollideInfo(m_preCollideInfo, m_newCollideInfo, false);
	CheckSendOnCollideInfo(m_preTriggerInfo, m_newTriggerInfo, true);

	//�ʒu����
	FixPosition();

	for (const auto& info : m_onCollideInfo)
	{
		if (info.kind == eOnCollideInfoKind::CollideEnter)
		{
			info.mySelf->OnCollideEnter(info.person);
		}
		else if (info.kind == eOnCollideInfoKind::CollideStay)
		{
			info.mySelf->OnCollideStay(info.person);
		}
		else if (info.kind == eOnCollideInfoKind::CollideExit)
		{
			info.mySelf->OnCollideExit(info.person);
		}
		else if (info.kind == eOnCollideInfoKind::TriggerEnter)
		{
			info.mySelf->OnTriggerEnter(info.person);
		}
		else if (info.kind == eOnCollideInfoKind::TriggerStay)
		{
			info.mySelf->OnTriggerStay(info.person);
		}
		else if (info.kind == eOnCollideInfoKind::TriggerExit)
		{
			info.mySelf->OnTriggerExit(info.person);
		}
	}

}

void MyLibrary::Physics::CheckUpdate()
{
	for (auto& item : m_collidables)
	{
		if (item->GetTag() == ObjectTag::Player)
		{
			float rad = 0;
			int modelHandle = -1;

			for (auto& col : item->m_colliders)
			{
				rad = dynamic_cast<MyLibrary::CollidableDataCapsule*> (col.get())->m_radius;

				modelHandle = m_stageCollisionHandle;
			}
			//m_hitDim = MV1CollCheck_Sphere(modelHandle, -1, item->rigidbody.GetNextPosVECTOR(), rad);
			m_hitDim = MV1CollCheck_Capsule(modelHandle, -1, VAdd(item->rigidbody.GetNextPosVECTOR(), item->rigidbody.GetVec()),
				VSub(item->rigidbody.GetNextPosVECTOR(), item->rigidbody.GetVec()), rad);
		}
		else
		{
			continue;
		}

		//�ǂƏ��̓����蔻����s��
		CheckWallAndFloor(item);
		//�ǂƂ̓����蔻�菈��
		FixPositionWithWall(item);
		//���Ƃ̓����蔻�菈��
		FixNowPositionWithFloor(item);

		//���o�����v���C���[�̎��͂̃|���S�������������
		MV1CollResultPolyDimTerminate(m_hitDim);

	}
}

void MyLibrary::Physics::CheckEnemyUpdate()
{
	for (auto& item : m_collidables)
	{
		if (item->GetTag() == ObjectTag::Enemy)
		{
			float rad = 0;
			int modelHandle = -1;

			for (auto& col : item->m_colliders)
			{
				rad = dynamic_cast<MyLibrary::CollidableDataCapsule*> (col.get())->m_radius;

				modelHandle = m_stageCollisionHandle;
			}
			//m_hitDim = MV1CollCheck_Sphere(modelHandle, -1, item->rigidbody.GetNextPosVECTOR(), rad);
			m_hitDim = MV1CollCheck_Capsule(modelHandle, -1, VAdd(item->rigidbody.GetNextPosVECTOR(), item->rigidbody.GetVec()),
				VSub(item->rigidbody.GetNextPosVECTOR(), item->rigidbody.GetVec()), rad);
		}
		else
		{
			continue;
		}

		//�G�L����������ԍŏ��ɍ��ꂽ��ȊO�͗����čŌ�ɍ�����L�����͗����Ȃ������ɂȂ��Ă���o�O

		//�ǂƏ��̓����蔻����s��
		CheckWallAndFloorEnemy(item);
		//�ǂƂ̓����蔻�菈��
		FixPositionWithWallEnemy(item);
		//���Ƃ̓����蔻�菈��
		FixNowPositionWithFloorEnemy(item);

		//���o�����v���C���[�̎��͂̃|���S�������������
		MV1CollResultPolyDimTerminate(m_hitDim);
	}
}

/// <summary>
/// �����蔻��`�F�b�N
/// </summary>
void MyLibrary::Physics::CheckCollide()
{
	std::vector<OnCollideInfoData> onCollideInfo;
	//�Փ˒ʒm�A�|�W�V�����␳
	bool doCheck = true;
	int checkCount = 0;     //�`���b�N��
	while (doCheck)
	{
		doCheck = false;
		checkCount++;

		//2�d���[�v�őS�I�u�W�F�N�g�����蔻��
		//FIXME : �d���̂ŋ߂��I�u�W�F�N�g���m�̂ݓ����蔻�肷��ȂǍH�v������
		for (const auto& objA : m_collidables)
		{
			for (const auto& objB : m_collidables)
			{
				//����I�u�W�F�N�g�Ȃ瑁�����^�[��
				if (objA == objB)
					continue;

				for (const auto& colA : objA->m_colliders)
				{
					for (const auto& colB : objB->m_colliders)
					{
						if (!IsCollide(objA->rigidbody, objB->rigidbody, colA.get(), colB.get())) continue;

						bool isTrigger = colA->IsTrigger() || colB->IsTrigger();

						if (isTrigger)
						{
							AddNewCollideInfo(objA, objB, m_newTriggerInfo);
						}
						else
						{
							AddNewCollideInfo(objA, objB, m_newCollideInfo);
						}

						//Trigger�̏ꍇ�͈ʒu�␳�͂��Ȃ�
						if (isTrigger) continue;

						auto primary = objA;
						auto secondary = objB;

						if (primary == secondary)
						{
							break;
						}

						auto primaryCollider = colA;
						auto secondaryCollider = colB;
						if (objA->priority < objB->priority)
						{
							primary = objB;
							secondary = objA;
							primaryCollider = colB;
							secondaryCollider = colA;
						}

						FixNextPosition(primary->rigidbody, secondary->rigidbody, primaryCollider.get(), secondaryCollider.get());
						//�ʒu�␳�������������x���߂���s��
						doCheck = true;
						break;
					}
					if (doCheck) break;
				}
				if (doCheck) break;
			}
			if (doCheck) break;
		}
		if (doCheck && checkCount > 800)
		{
#if _DEBUG
			printfDx("�K��񐔂𒴂��܂���");
#endif
			break;
		}
	}
}

void MyLibrary::Physics::CheckCollide1()
{
	std::vector<OnCollideInfoData> onCollideInfo;
	//�Փ˒ʒm�A�|�W�V�����␳
	bool doCheck = true;
	int checkCount = 0;     //�`���b�N��
	while (doCheck)
	{
		doCheck = false;
		checkCount++;

		//2�d���[�v�őS�I�u�W�F�N�g�����蔻��
		//FIXME : �d���̂ŋ߂��I�u�W�F�N�g���m�̂ݓ����蔻�肷��ȂǍH�v������
		for (const auto& objA : m_collidables)
		{
			for (const auto& objB : m_collidables)
			{
				//����I�u�W�F�N�g�Ȃ瑁�����^�[��
				if (objA == objB)
					continue;

				for (const auto& colA : objA->m_colliders)
				{
					for (const auto& colB : objB->m_colliders)
					{
						if (!IsCollide1(objA->rigidbody, objB->rigidbody, colA.get(), colB.get())) continue;

						bool isTrigger = colA->IsTrigger() || colB->IsTrigger();

						if (isTrigger)
						{
							AddNewCollideInfo(objA, objB, m_newTriggerInfo);
						}
						else
						{
							AddNewCollideInfo(objA, objB, m_newCollideInfo);
						}

						//Trigger�̏ꍇ�͈ʒu�␳�͂��Ȃ�
						if (isTrigger) continue;

						auto primary = objA;
						auto secondary = objB;

						if (primary == secondary)
						{
							break;
						}

						auto primaryCollider = colA;
						auto secondaryCollider = colB;
						if (objA->priority < objB->priority)
						{
							primary = objB;
							secondary = objA;
							primaryCollider = colB;
							secondaryCollider = colA;
						}

						FixNextPosition(primary->rigidbody, secondary->rigidbody, primaryCollider.get(), secondaryCollider.get());
						//�ʒu�␳�������������x���߂���s��
						doCheck = true;
						break;
					}
					if (doCheck) break;
				}
				if (doCheck) break;
			}
			if (doCheck) break;
		}
		if (doCheck && checkCount > 800)
		{
#if _DEBUG
			printfDx("�K��񐔂𒴂��܂���");
#endif
			break;
		}
	}
}

void MyLibrary::Physics::CheckCollide2()
{
	std::vector<OnCollideInfoData> onCollideInfo;
	//�Փ˒ʒm�A�|�W�V�����␳
	bool doCheck = true;
	int checkCount = 0;     //�`���b�N��
	while (doCheck)
	{
		doCheck = false;
		checkCount++;

		//2�d���[�v�őS�I�u�W�F�N�g�����蔻��
		//FIXME : �d���̂ŋ߂��I�u�W�F�N�g���m�̂ݓ����蔻�肷��ȂǍH�v������
		for (const auto& objA : m_collidables)
		{
			for (const auto& objB : m_collidables)
			{
				//����I�u�W�F�N�g�Ȃ瑁�����^�[��
				if (objA == objB)
					continue;

				for (const auto& colA : objA->m_colliders)
				{
					for (const auto& colB : objB->m_colliders)
					{
						if (!IsCollide2(objA->rigidbody, objB->rigidbody, colA.get(), colB.get())) continue;

						bool isTrigger = colA->IsTrigger() || colB->IsTrigger();

						if (isTrigger)
						{
							AddNewCollideInfo(objA, objB, m_newTriggerInfo);
						}
						else
						{
							AddNewCollideInfo(objA, objB, m_newCollideInfo);
						}

						//Trigger�̏ꍇ�͈ʒu�␳�͂��Ȃ�
						if (isTrigger) continue;

						auto primary = objA;
						auto secondary = objB;

						if (primary == secondary)
						{
							break;
						}

						auto primaryCollider = colA;
						auto secondaryCollider = colB;
						if (objA->priority < objB->priority)
						{
							primary = objB;
							secondary = objA;
							primaryCollider = colB;
							secondaryCollider = colA;
						}

						FixNextPosition(primary->rigidbody, secondary->rigidbody, primaryCollider.get(), secondaryCollider.get());
						//�ʒu�␳�������������x���߂���s��
						doCheck = true;
						break;
					}
					if (doCheck) break;
				}
				if (doCheck) break;
			}
			if (doCheck) break;
		}
		if (doCheck && checkCount > 800)
		{
#if _DEBUG
			printfDx("�K��񐔂𒴂��܂���");
#endif
			break;
		}
	}
}

void MyLibrary::Physics::CheckCollide3()
{
	std::vector<OnCollideInfoData> onCollideInfo;
	//�Փ˒ʒm�A�|�W�V�����␳
	bool doCheck = true;
	int checkCount = 0;     //�`���b�N��
	while (doCheck)
	{
		doCheck = false;
		checkCount++;

		//2�d���[�v�őS�I�u�W�F�N�g�����蔻��
		//FIXME : �d���̂ŋ߂��I�u�W�F�N�g���m�̂ݓ����蔻�肷��ȂǍH�v������
		for (const auto& objA : m_collidables)
		{
			for (const auto& objB : m_collidables)
			{
				//����I�u�W�F�N�g�Ȃ瑁�����^�[��
				if (objA == objB)
					continue;

				for (const auto& colA : objA->m_colliders)
				{
					for (const auto& colB : objB->m_colliders)
					{
						if (!IsCollide3(objA->rigidbody, objB->rigidbody, colA.get(), colB.get())) continue;

						bool isTrigger = colA->IsTrigger() || colB->IsTrigger();

						if (isTrigger)
						{
							AddNewCollideInfo(objA, objB, m_newTriggerInfo);
						}
						else
						{
							AddNewCollideInfo(objA, objB, m_newCollideInfo);
						}

						//Trigger�̏ꍇ�͈ʒu�␳�͂��Ȃ�
						if (isTrigger) continue;

						auto primary = objA;
						auto secondary = objB;

						if (primary == secondary)
						{
							break;
						}

						auto primaryCollider = colA;
						auto secondaryCollider = colB;
						if (objA->priority < objB->priority)
						{
							primary = objB;
							secondary = objA;
							primaryCollider = colB;
							secondaryCollider = colA;
						}

						FixNextPosition(primary->rigidbody, secondary->rigidbody, primaryCollider.get(), secondaryCollider.get());
						//�ʒu�␳�������������x���߂���s��
						doCheck = true;
						break;
					}
					if (doCheck) break;
				}
				if (doCheck) break;
			}
			if (doCheck) break;
		}
		if (doCheck && checkCount > 800)
		{
#if _DEBUG
			printfDx("�K��񐔂𒴂��܂���");
#endif
			break;
		}
	}
}

/// <summary>
/// ��̃I�u�W�F�N�g���ڐG���Ă��邩�ǂ���
/// </summary>
/// <param name="rigidA">�I�u�W�F�N�gA�̕����f�[�^</param>
/// <param name="rigidB">�I�u�W�F�N�gB�̕����f�[�^</param>
/// <param name="colliderA">�I�u�W�F�N�gA�̓����蔻��f�[�^</param>
/// <param name="colliderB">�I�u�W�F�N�gB�̓����蔻��f�[�^</param>
/// <returns></returns>
bool MyLibrary::Physics::IsCollide(const Rigidbody& rigidA, const Rigidbody& rigidB, CollidableData* colliderA, CollidableData* colliderB) const
{
	bool isCollide = false;

	auto kindA = colliderA->GetKind();
	auto kindB = colliderB->GetKind();

	//�J�v�Z�����m�̓����蔻��
	if (kindA == MyLibrary::CollidableData::Kind::Capsule && kindB == MyLibrary::CollidableData::Kind::Capsule)
	{
		auto colA = dynamic_cast<MyLibrary::CollidableDataCapsule*>(colliderA);
		auto colB = dynamic_cast<MyLibrary::CollidableDataCapsule*>(colliderB);

		auto atob = rigidA.GetNextPos() - rigidB.GetNextPos();
		auto atobLength = atob.Length();

		//���g�̌����Ă�����ɐL�тĂ���x�N�g�����쐬
		LibVec3 sDirVec = colA->m_vec.GetNormalized() * colA->m_len * 0.5f;
		//�Ώۂ̌����Ă�����ɐL�тĂ���x�N�g�����쐬
		LibVec3 tDirVec = colB->m_vec.GetNormalized() * colB->m_len * 0.5f;

		//���΃x�N�g��
		LibVec3 vec = rigidA.GetPos() - rigidB.GetPos();

		//�@���x�N�g��
		LibVec3 norm = norm.Cross(sDirVec, tDirVec);

		//���s����
		bool isParallel = norm.SqLength() < 0.001f;

		float s, t;
		//���s�łȂ��ꍇ
		if (!isParallel)
		{
			//�P�ʍs��
			LibMatrix3 mat;
			mat.Init();

			//�l�̑��
			mat.SetLine(0, sDirVec);
			mat.SetLine(1, tDirVec.Reverse());
			mat.SetLine(2, norm);

			//�t�s��
			mat = mat.GetInverse();

			s = norm.Dot(mat.GetRow(0), vec);
			t = norm.Dot(mat.GetRow(1), vec);
		}
		//���s�̏ꍇ
		else
		{
			s = norm.Dot(sDirVec, vec) / sDirVec.SqLength();
			t = norm.Dot(tDirVec, vec) / tDirVec.SqLength();
		}

		//�͈͂̐���
		if (s < -1.0f) s = -1.0f;   //����
		if (s > 1.0f) s = 1.0f;     //���
		if (t < -1.0f) t = -1.0f;   //����
		if (t > 1.0f) t = 1.0f;     //���

		//������ł̍ŒZ����
		LibVec3 minPos1 = sDirVec * s + rigidA.GetPos();
		LibVec3 minPos2 = tDirVec * t + rigidB.GetPos();
		//�傫��2��
		float sqLen = (minPos1 - minPos2).SqLength();
		//���ꂼ��̔��a�̍��v��2��
		float ar = colA->m_radius + colB->m_radius;

		ar = ar * ar;

		isCollide = sqLen < ar;
	}
	
	
	

	return isCollide;
}

bool MyLibrary::Physics::IsCollide1(const Rigidbody& rigidA, const Rigidbody& rigidB, CollidableData* colliderA, CollidableData* colliderB) const
{
	bool isCollide = false;

	auto kindA = colliderA->GetKind();
	auto kindB = colliderB->GetKind();

	//�J�v�Z���Ƌ��̂̓����蔻��
	if (kindA == MyLibrary::CollidableData::Kind::Capsule && kindB == MyLibrary::CollidableData::Kind::Sphere)
	{
		auto colA = dynamic_cast<MyLibrary::CollidableDataCapsule*>(colliderA);
		auto colB = dynamic_cast<MyLibrary::CollidableDataSphere*>(colliderB);

		//���g�̌����Ă�����ɐL�тĂ���x�N�g�����쐬
		LibVec3 dirVec = colA->m_vec.GetNormalized() * colA->m_len * 0.5f;

		//���΃x�N�g��
		LibVec3 vec = rigidA.GetPos() - rigidB.GetPos();

		//���΃x�N�g���Ɛ��ʃx�N�g���̓���
		float dot = vec.Dot(vec, dirVec);
		//�����x�N�g���̑傫�����擾(2��)
		float sqLen = dirVec.SqLength();

		//������̂ǂ��ɂ��邩���m���߂�
		float t = dot / sqLen;
		//�͈͂̐���
		if (t < -1.0f) t = -1.0f; //����
		if (t > 1.0f) t = 1.0f;   //���

		//������ł̓_�܂ł̍ŒZ����
		LibVec3::Pos3 minPos = dirVec * t + rigidA.GetPos();

		//�ŒZ���W�Ɖ~�̍��W�̃x�N�g���傫��(2��)���擾
		sqLen = (minPos - rigidB.GetPos()).SqLength();
		//���a�̍��v��2��
		float radius = colA->m_radius + colB->m_radius;
		radius = radius * radius;

		isCollide = sqLen < radius;
	}

	return isCollide;
}

bool MyLibrary::Physics::IsCollide2(const Rigidbody& rigidA, const Rigidbody& rigidB, CollidableData* colliderA, CollidableData* colliderB) const
{
	bool isCollide = false;

	auto kindA = colliderA->GetKind();
	auto kindB = colliderB->GetKind();

	//��`�ƃJ�v�Z���̓����蔻��
	if (kindA == MyLibrary::CollidableData::Kind::Rect && kindB == MyLibrary::CollidableData::Kind::Capsule)
	{
		auto colA = dynamic_cast<MyLibrary::CollidableDataRect*>(colliderA);
		auto colB = dynamic_cast<MyLibrary::CollidableDataCapsule*>(colliderB);

		//���΃x�N�g��
		LibVec3 vec = rigidA.GetPos() - rigidB.GetPos();

		//�l�̐�Ή�
		vec.x = fabs(vec.x);
		vec.y = fabs(vec.y);
		vec.z = fabs(vec.z);

		//float trw = colB->m_radius + (colA->m_size.width * 0.5f);
		//float trh = colB->m_radius + (colA->m_size.height * 0.5f);
		//float trd = colB->m_radius + (colA->m_size.depth * 0.5f);

		float trw = colB->m_radius + (rigidA.GetSize().width * 0.5f);
		float trh = colB->m_radius + (rigidA.GetSize().height * 0.5f);
		float trd = colB->m_radius + (rigidA.GetSize().depth * 0.5f);

		//�e�����̔���
		bool isHitX = vec.x < trw;
		bool isHitY = vec.y < trh;
		bool isHitZ = vec.z < trd;

		//����
		isCollide = isHitX && isHitY && isHitZ;
	}

	return isCollide;
}

bool MyLibrary::Physics::IsCollide3(const Rigidbody& rigidA, const Rigidbody& rigidB, CollidableData* colliderA, CollidableData* colliderB) const
{
	bool isCollide = false;

	auto kindA = colliderA->GetKind();
	auto kindB = colliderB->GetKind();

	//��`�Ƌ��̂̓����蔻��
	if (kindA == MyLibrary::CollidableData::Kind::Rect && kindB == MyLibrary::CollidableData::Kind::Sphere)
	{
		auto colA = dynamic_cast<MyLibrary::CollidableDataRect*>(colliderA);
		auto colB = dynamic_cast<MyLibrary::CollidableDataSphere*>(colliderB);

		//���΃x�N�g��
		LibVec3 vec = rigidA.GetPos() - rigidB.GetPos();

		//�l�̐�Ή�
		vec.x = fabs(vec.x);
		vec.y = fabs(vec.y);
		vec.z = fabs(vec.z);

		//float trw = colB->m_radius + (colA->m_size.width * 0.5f);
		//float trh = colB->m_radius + (colA->m_size.height * 0.5f);
		//float trd = colB->m_radius + (colA->m_size.depth * 0.5f);

		float trw = colB->m_radius + (rigidA.GetSize().width * 0.5f);
		float trh = colB->m_radius + (rigidA.GetSize().height * 0.5f);
		float trd = colB->m_radius + (rigidA.GetSize().depth * 0.5f);

		//�e�����̔���
		bool isHitX = vec.x < trw;
		bool isHitY = vec.y < trh;
		bool isHitZ = vec.z < trd;

		//����
		isCollide = isHitX && isHitY && isHitZ;
	}

	return isCollide;
}

/// <summary>
/// ���������I�u�W�F�N�g�̃y�A��o�^����
/// </summary>
/// <param name="objA">�I�u�W�F�N�gA</param>
/// <param name="objB">�I�u�W�F�N�gB</param>
/// <param name="info">�o�^����z��</param>
void MyLibrary::Physics::AddNewCollideInfo(const std::shared_ptr<Collidable>& objA, const std::shared_ptr<Collidable>& objB, SendCollideInfo& info)
{
	bool isPairA = info.find(objA) != info.end();
	bool isPairB = info.find(objB) != info.end();
	if (isPairA || isPairB)
	{
		std::shared_ptr<Collidable> parent = objA;
		std::shared_ptr<Collidable> child = objB;
		if (isPairB)
		{
			parent = objB;
			child = objA;
		}
		bool isChild = std::find(info[parent].begin(), info[parent].end(), child) != info[parent].end();
		if (!isChild)
		{
			info[parent].emplace_back(child);
		}
	}
	else
	{
		info[objA].emplace_back(objB);
	}
}

/// <summary>
/// �ړ��\��̍��W���C������
/// </summary>
/// <param name="primaryRigid">�D��x�������I�u�W�F�N�g�̕����f�[�^</param>
/// <param name="secondaryRigid">�D��x���Ⴂ�I�u�W�F�N�g�̕����f�[�^</param>
/// <param name="primaryCollider">�D��x�������I�u�W�F�N�g�̓����蔻��f�[�^</param>
/// <param name="secondaryCollider">�D��x���Ⴂ�I�u�W�F�N�g�̓����蔻��f�[�^</param>
void MyLibrary::Physics::FixNextPosition(const Rigidbody& primaryRigid, Rigidbody& secondaryRigid, CollidableData* primaryCollider, CollidableData* secondaryCollider) const
{
	//�����蔻��̎�ʖ��ɕ␳���@��ς���
	auto primaryKind = primaryCollider->GetKind();
	auto secondaryKind = secondaryCollider->GetKind();

	if (primaryKind == MyLibrary::CollidableData::Kind::Capsule && secondaryKind == MyLibrary::CollidableData::Kind::Capsule)
	{
		auto primaryToSecondary = secondaryRigid.GetNextPos() - primaryRigid.GetNextPos();
		auto primaryToSecondaryN = primaryToSecondary.Normalize();

		auto primaryColliderData = dynamic_cast<MyLibrary::CollidableDataCapsule*>(primaryCollider);
		auto secondaryColliderData = dynamic_cast<MyLibrary::CollidableDataCapsule*>(secondaryCollider);
		auto awayDist = primaryColliderData->m_radius + secondaryColliderData->m_radius + 0.0001f; //���̂܂܂��Ƃ�����ʒu�ɂȂ邩��]���ɗ���
		auto primaryToNewSecondaryPos = primaryToSecondaryN * awayDist;
		auto fixedPos = primaryRigid.GetNextPos() + primaryToNewSecondaryPos;
		fixedPos.y = secondaryRigid.GetPos().y;
		secondaryRigid.SetNextPos(fixedPos);

	}
	//��`�ƃJ�v�Z���Ƃ̓����蔻��
	else if (primaryKind == MyLibrary::CollidableData::Kind::Rect && secondaryKind == MyLibrary::CollidableData::Kind::Capsule)
	{
		auto rectCollider = dynamic_cast<MyLibrary::CollidableDataRect*>(primaryCollider);
		auto capsuleCollider = dynamic_cast<MyLibrary::CollidableDataCapsule*>(secondaryCollider);

		//��`�ƒ��S�̃J�v�Z���̒��S�̑��Έʒu���v�Z
		auto rectCenter = primaryRigid.GetNextPos();
		auto capsuleCenter = secondaryRigid.GetNextPos();
		auto relativePos = capsuleCenter - rectCenter;

		//��`�̂̔����̃T�C�Y���擾
		auto halfSize = primaryRigid.GetSize() * 0.5f;

		//�J�v�Z���̔��a���擾
		float capsuleRadius = capsuleCollider->m_radius;

		//�����o������������
		LibVec3 pushDir;
		if (fabs(relativePos.x) > fabs(relativePos.z))
		{
			//X�����ɉ����o��
			pushDir = LibVec3((relativePos.x > 0 ? 1 : -1) * (halfSize.width + capsuleRadius - fabs(relativePos.x)), 0, 0);
		}
		else
		{
			//Z�����ɉ����o��
			pushDir = LibVec3(0, 0, (relativePos.z > 0 ? 1 : -1) * (halfSize.depth + capsuleRadius - fabs(relativePos.z)));
		}

		//�V�����ʒu��ݒ�
		secondaryRigid.SetNextPos(capsuleCenter + pushDir);
	}

}

/// <summary>
/// ��ޖ��ɏՓ˒ʒm���΂��z��ɒǉ�����
/// </summary>
/// <param name="preSendInfo">�Փ˂����I�u�W�F�N�g�̃��X�g�̃��O</param>
/// <param name="newSendInfo">�Փ˂����I�u�W�F�N�g�̃��X�g</param>
/// <param name="isTrigger">�������邩�ǂ���</param>
void MyLibrary::Physics::CheckSendOnCollideInfo(SendCollideInfo& preSendInfo, SendCollideInfo& newSendInfo, bool isTrigger)
{
	for (auto& parent : newSendInfo)
	{
		//�ȑO�̏��ɐe�Ƃ��ēo�^����Ă��邩
		bool isPreParent = preSendInfo.find(parent.first) != preSendInfo.end();
		bool isAllEnter = true;

		for (auto& child : parent.second)
		{
			bool isPreChild = false;
			if (isPreChild)
			{
				//�ȑO�̏��Ɏq�Ƃ��ēo�^����Ă��邩
				auto& preParent = preSendInfo[parent.first];
				isPreChild = std::find(preParent.begin(), preParent.end(), child) != preParent.end();

			}

			//��������Ă����ꍇ��Enter���Ă�(�q�Ƃ��ēo�^����Ă��Ȃ�)
			if (!isPreChild)
			{
				if (isTrigger)
				{
					AddOnCollideInfo(parent.first, child, eOnCollideInfoKind::TriggerEnter);
					AddOnCollideInfo(child, parent.first, eOnCollideInfoKind::TriggerEnter);
				}
				else
				{
					AddOnCollideInfo(parent.first, child, eOnCollideInfoKind::CollideEnter);
					AddOnCollideInfo(child, parent.first, eOnCollideInfoKind::CollideEnter);
				}
			}

			//Stary�͖��x�Ă�
			if (isTrigger)
			{
				AddOnCollideInfo(parent.first, child, eOnCollideInfoKind::TriggerStay);
				AddOnCollideInfo(child, parent.first, eOnCollideInfoKind::TriggerStay);
			}
			else
			{
				AddOnCollideInfo(parent.first, child, eOnCollideInfoKind::CollideStay);
				AddOnCollideInfo(child, parent.first, eOnCollideInfoKind::CollideStay);
			}

			//�o�^����Ă��������폜
			if (isPreChild)
			{
				preSendInfo[parent.first].remove(child);
			}
			//�S�ēo�^����Ă��Ȃ������Ƃ���
			else
			{
				isAllEnter = false;
			}
		}

		//�S�ēo�^����Ă�����e�̏�������
		if (isAllEnter)
		{
			preSendInfo.erase(parent.first);
		}
	}

	//�c���Ă���O��񂩂�Exist���Ă�(�o�^���c���Ă���=���񔲂���)
	for (auto& parent : preSendInfo)
	{
		for (auto& child : parent.second)
		{
			if(isTrigger)
			{
				AddOnCollideInfo(parent.first, child, eOnCollideInfoKind::TriggerExit);
				AddOnCollideInfo(child, parent.first, eOnCollideInfoKind::TriggerExit);
			}
			else
			{
				AddOnCollideInfo(parent.first, child, eOnCollideInfoKind::CollideExit);
				AddOnCollideInfo(child, parent.first, eOnCollideInfoKind::CollideExit);
			}
		}
	}
}

/// <summary>
/// �Փ˒ʒm���΂��z��ɒǉ�����
/// </summary>
/// <param name="mySelf">���g</param>
/// <param name="person">�Փ˂�������</param>
/// <param name="kind">�����蔻��̎��</param>
void MyLibrary::Physics::AddOnCollideInfo(const std::shared_ptr<Collidable>& mySelf, const std::shared_ptr<Collidable>& person, eOnCollideInfoKind kind)
{
	OnCollideInfoData addInfo;
	addInfo.mySelf = mySelf;
	addInfo.person = person;
	addInfo.kind = kind;
	m_onCollideInfo.emplace_back(addInfo);
}

/// <summary>
/// �ŏI�I�Ȉʒu�����肷��
/// </summary>
void MyLibrary::Physics::FixPosition()
{
	for (auto& item : m_collidables)
	{
		//Pos���X�V����̂ŁAvelocity�������Ɉړ�����velocity�ɏC��
		LibVec3 toFixedPos = item->rigidbody.GetNextPos() - item->rigidbody.GetPos();
		item->rigidbody.SetVelocity(toFixedPos);

		//�ʒu�m��
		item->rigidbody.SetPos(item->rigidbody.GetNextPos());
	}
}

/// <summary>
/// �`�F�b�N�����|���S�����ǃ|���S�������|���S�����𔻒f����
/// </summary>
/// <param name="col">�`�F�b�N����I�u�W�F�N�g</param>
void MyLibrary::Physics::CheckWallAndFloor(std::shared_ptr<Collidable>& col)
{
	//�ǃ|���S���Ə��|���S���̐�������������
	m_wallNum = 0;
	m_floorNum = 0;

	//���o���ꂽ�|���S���̐������J��Ԃ�
	for (int i = 0; i < m_hitDim.HitNum; i++)
	{
		//�|���S���̖@����Y�������ǃ|���S���{�[�_�[�ɒB���Ă��邩�ǂ����ŕǃ|���S�������|���S�����𔻒f����
		if (m_hitDim.Dim[i].Normal.y < kWallPolyBorder && m_hitDim.Dim[i].Normal.y > -kWallPolyBorder)
		{
			//�ǃ|���S���Ɣ��f���ꂽ�ꍇ�ł��A�v���C���[��Y���W��荂���|���S���̂ݓ����蔻����s��
			if (m_hitDim.Dim[i].Position[0].y > col->rigidbody.GetPos().y + kWallPolyHeight ||
				m_hitDim.Dim[i].Position[1].y > col->rigidbody.GetPos().y + kWallPolyHeight ||
				m_hitDim.Dim[i].Position[2].y > col->rigidbody.GetPos().y + kWallPolyHeight)
			{
				//�|���S���̐������E���ɒB���Ă��Ȃ�������|���S����z��ɒǉ�
				if (m_wallNum < ColInfo::kMaxColHitPoly)
				{
					//�|���S���̍\���̂̃A�h���X��ǃ|���S���|�C���^�z��ɒǉ�����
					m_pWallPoly[m_wallNum] = &m_hitDim.Dim[i];

					//�ǃ|���S���̐������Z����
					m_wallNum++;
				}
			}
		}
		else
		{
			//�|���S���̐������E���ɒB���Ă��Ȃ�������|���S����z��ɒǉ�
			if (m_floorNum < ColInfo::kMaxColHitPoly)
			{
				//�|���S���̍\���̂̃A�h���X�����|���S���|�C���^�z��ɕۑ�����
				m_pFloorPoly[m_floorNum] = &m_hitDim.Dim[i];

				//���|���S���̐������Z����
				m_floorNum++;
			}
		}
	}
}

/// <summary>
/// �ǃ|���S���Ƃ̓����蔻����`�F�b�N���A�ړ�������
/// </summary>
/// <param name="col">�`�F�b�N����I�u�W�F�N�g</param>
void MyLibrary::Physics::FixPositionWithWall(std::shared_ptr<Collidable>& col)
{
	float radius = 0.0f;
	MyLibrary::LibVec3 vec;
	float len = 0.0f;
	for (auto& col : col->m_colliders)
	{
		radius = dynamic_cast<MyLibrary::CollidableDataCapsule*> (col.get())->m_radius;
		len = dynamic_cast<MyLibrary::CollidableDataCapsule*> (col.get())->m_len;

		vec = vec.GetNormalized() * len * 0.5f;
	}

	//�ǃ|���S�����Ȃ��ꍇ�͉������Ȃ�
	if (m_wallNum == 0) return;

	//�ǃ|���S���Ƃ̓����蔻�菈��
	//�ǂɓ����������ǂ����̃t���O�͏�����Ԃł́u�������Ă��Ȃ��v�ɂ��Ă���
	m_isHitFlag = false;
	m_isHitWallFlag = false;


	//�ړ��������ǂ����ŏ����𕪊�
	if (col->rigidbody.GetDir().Length() != 0.0f)
	{
		//�ǃ|���S���̐������J��Ԃ�
		for (int i = 0; i < m_wallNum; i++)
		{
			//i�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
			m_pPoly = m_pWallPoly[i];

			//�|���S���ƃv���C���[���������Ă��Ȃ������玟�̃J�E���g��
			if (!HitCheck_Capsule_Triangle(VAdd(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), VSub(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), radius,
				m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2])) continue;

			//�����ɂ�����|���S���ƃv���C���[���������Ă���Ƃ������ƂȂ̂ŁA�|���S���ɓ��������t���O�𗧂Ă�
			m_isHitFlag = true;
			m_isHitWallFlag = true;

			//�ǂ��l�������ړ����O�ς��g���ĎZ�o
			MyLibrary::LibVec3 SlideVec;

			VECTOR ret;
			ret = VCross(col->rigidbody.GetVelocityVECTOR(), m_pPoly->Normal);

			m_ret = VScale(m_pPoly->Normal, 3.0f);

			//�i�s�����x�N�g���ƕǃ|���S���̖@���x�N�g���ɐ����ȃx�N�g�����Z�o
			SlideVec = MyLibrary::LibVec3(ret.x, 0, ret.z);

			//������ړ��O�̍��W�ɑ��������̂�V���ȍ��W�Ƃ���
			col->rigidbody.SetNextPos(col->rigidbody.GetPos() + SlideVec);

			//�V���Ȉړ����W�ŕǃ|���S���Ɠ������Ă��Ȃ����ǂ����𔻒肷��
			bool isHitWallPolygon = false;
			for (int j = 0; j < m_wallNum; j++)
			{
				//j�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
				m_pPoly = m_pWallPoly[j];

				//�������Ă����烋�[�v���甲����
				if (HitCheck_Capsule_Triangle(VAdd(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), VSub(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), radius,
					m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2]))
				{
					//true�ɂ���
					isHitWallPolygon = true;
					break;
				}
			}

			//�S�Ẵ|���S���Ɠ������Ă��Ȃ������炱���Ń��[�v�I��
			if (!isHitWallPolygon)
			{
				//�q�b�g�t���O��|��
				m_isHitFlag = false;
				m_isHitWallFlag = false;
				break;
			}
		}

		//�ǂɓ������Ă�����ǂ��牟���o���������s��
		if (m_isHitFlag)
		{
			FixPositionWithWallInternal(col);
		}

	}
	else
	{
		//�ړ����Ă��Ȃ��ꍇ�̏���

		//�ǃ|���S���̐������J��Ԃ�
		for (int i = 0; i < m_wallNum; i++)
		{
			//i�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
			m_pPoly = m_pWallPoly[i];

			//�|���S���ɓ������Ă����瓖�������t���O�𗧂Ă���Ń��[�v���甲����
			if (HitCheck_Capsule_Triangle(VAdd(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), VSub(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), radius,
				m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2]))
			{
				m_isHitFlag = true;
				m_isHitWallFlag = true;
				break;
			}
		}


		//�ǂɓ������Ă�����ǂ��牟���o���������s��
		if (m_isHitFlag)
		{
			FixPositionWithWallInternal(col);
		}
	}
}

/// <summary>
/// �ǂ̒����牟���o��
/// </summary>
/// <param name="col">�`�F�b�N����I�u�W�F�N�g</param>
void MyLibrary::Physics::FixPositionWithWallInternal(std::shared_ptr<Collidable>& col)
{
	float radius = 0.0f;
	MyLibrary::LibVec3 vec;
	float len = 0.0f;
	for (auto& col : col->m_colliders)
	{
		radius = dynamic_cast<MyLibrary::CollidableDataCapsule*> (col.get())->m_radius;
		len = dynamic_cast<MyLibrary::CollidableDataCapsule*> (col.get())->m_len;

		vec = vec.GetNormalized() * len * 0.5f;
	}

	//�ǂ���̉����o�����������݂�ő吔�����J��Ԃ�
	for (int i = 0; i < ColInfo::kMaxColHitTry; i++)
	{
		//������\���̂���ǃ|���S����S�Č���
		bool isHitWall = false;
		//�ǃ|���S���̐������J��Ԃ�
		for (int j = 0; j < m_wallNum; j++)
		{
			//j�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
			m_pPoly = m_pWallPoly[j];

			//�|���S���ƃv���C���[���������Ă��Ȃ������玟�̃J�E���g��
			if (!HitCheck_Capsule_Triangle(VAdd(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), VSub(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), radius,
				m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2])) continue;

			auto ret = VAdd(col->rigidbody.GetNextPosVECTOR(), VScale(m_pPoly->Normal, kColHitSlideLength));

			MyLibrary::LibVec3 set;
			set = MyLibrary::LibVec3(ret.x, ret.y, ret.z);

			//�������Ă�����K�苗�����v���C���[��ǂ̖@�������Ɉړ�������
			col->rigidbody.SetNextPos(set);

			//�ړ����������ŕǃ|���S���Ɛڑ����Ă��邩�ǂ����𔻒�
			for (int k = 0; k < m_wallNum; k++)
			{
				//�������Ă����烋�[�v�𔲂���
				m_pPoly = m_pWallPoly[k];
				if (HitCheck_Capsule_Triangle(VAdd(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), VSub(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), radius,
					m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2]))
				{
					isHitWall = true;
					break;
				}

			}

			//�S�Ẵ|���S���Ɠ������Ă��Ȃ������炱���Ń��[�v�I��
			if (!isHitWall) break;
		}

		//���[�v�I��
		if (!isHitWall) break;
	}
}

/// <summary>
/// ���|���S���Ƃ̓����蔻����`�F�b�N���A�ړ�������
/// </summary>
/// <param name="col">�`�F�b�N����I�u�W�F�N�g</param>
void MyLibrary::Physics::FixNowPositionWithFloor(std::shared_ptr<Collidable>& col)
{
	//���|���S�����Ȃ��ꍇ�͉������Ȃ�
	if (m_floorNum == 0) return;

	float radius = 0.0f;
	MyLibrary::LibVec3 vec;
	float len = 0.0f;
	for (auto& col : col->m_colliders)
	{
		radius = dynamic_cast<MyLibrary::CollidableDataCapsule*> (col.get())->m_radius;
		len = dynamic_cast<MyLibrary::CollidableDataCapsule*> (col.get())->m_len;

		vec = vec.GetNormalized() * len * 0.5f;
	}

	//���|���S���Ƃ̓����蔻�菈��
	//�����������ǂ����̃t���O������
	bool isHitFlag = false;

	//���|���S���Ƃ̓����蔻��
	//��ԍ������|���S���ɂԂ���ׂ̔���p�ϐ���������
	float PolyMaxPosY = 0.0f;

	//���|���S���̐������J��Ԃ�
	for (int i = 0; i < m_floorNum; i++)
	{
		//i�Ԗڂ̏��|���S���̃A�h���X�����|���S���|�C���^�z�񂩂�擾
		m_pPoly = m_pFloorPoly[i];

		//�|���S���ƃv���C���[���������Ă��Ȃ������玟�̃J�E���g��
		if (HitCheck_Capsule_Triangle(VAdd(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), VSub(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), radius,
			m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2])) continue;

		float mostHeightY = m_pPoly->Position[0].y;

		if (mostHeightY < m_pPoly->Position[1].y)
		{
			mostHeightY = m_pPoly->Position[1].y;
		}

		if (mostHeightY < m_pPoly->Position[2].y)
		{
			mostHeightY = m_pPoly->Position[2].y;
		}

		//���ɓ��������|���S��������A�����܂Ō��o�������|���S�����Ⴂ�ꍇ�͉������Ȃ�
		if (m_isHitFlag && PolyMaxPosY > mostHeightY) continue;

		//�|���S���ɓ��������t���O�𗧂Ă�
		m_isHitFlag = true;

		//�ڐG����Y���W��ێ�����
		PolyMaxPosY = mostHeightY;
	}

	//���|���S���̓����蔻�肩�A�W�����v�͂�0����������(���~���̏ꍇ)�ǂ����ŏ����𕪊�
	if (m_isHitFlag)
	{
		//�ڐG�����|���S���ň�ԍ���Y���W���v���C���[��Y���W�ɂ���
		auto set = col->rigidbody.GetNextPos();
		set.y = PolyMaxPosY + radius;
		col->rigidbody.SetNextPos(set);
	}
}

void MyLibrary::Physics::CheckWallAndFloorEnemy(std::shared_ptr<Collidable>& col)
{
	//�ǃ|���S���Ə��|���S���̐�������������
	m_wallNum = 0;
	m_floorNum = 0;

	//���o���ꂽ�|���S���̐������J��Ԃ�
	for (int i = 0; i < m_hitDim.HitNum; i++)
	{
		//�|���S���̖@����Y�������ǃ|���S���{�[�_�[�ɒB���Ă��邩�ǂ����ŕǃ|���S�������|���S�����𔻒f����
		if (m_hitDim.Dim[i].Normal.y < kWallPolyBorder && m_hitDim.Dim[i].Normal.y > -kWallPolyBorder)
		{
			//�ǃ|���S���Ɣ��f���ꂽ�ꍇ�ł��A�v���C���[��Y���W��荂���|���S���̂ݓ����蔻����s��
			if (m_hitDim.Dim[i].Position[0].y > col->rigidbody.GetPos().y + kWallPolyHeight ||
				m_hitDim.Dim[i].Position[1].y > col->rigidbody.GetPos().y + kWallPolyHeight ||
				m_hitDim.Dim[i].Position[2].y > col->rigidbody.GetPos().y + kWallPolyHeight)
			{
				//�|���S���̐������E���ɒB���Ă��Ȃ�������|���S����z��ɒǉ�
				if (m_wallNum < ColInfo::kMaxColHitPoly)
				{
					//�|���S���̍\���̂̃A�h���X��ǃ|���S���|�C���^�z��ɒǉ�����
					m_pWallPoly[m_wallNum] = &m_hitDim.Dim[i];

					//�ǃ|���S���̐������Z����
					m_wallNum++;
				}
			}
		}
		else
		{
			//�|���S���̐������E���ɒB���Ă��Ȃ�������|���S����z��ɒǉ�
			if (m_floorNum < ColInfo::kMaxColHitPoly)
			{
				//�|���S���̍\���̂̃A�h���X�����|���S���|�C���^�z��ɕۑ�����
				m_pFloorPoly[m_floorNum] = &m_hitDim.Dim[i];

				//���|���S���̐������Z����
				m_floorNum++;
			}
		}
	}
}

void MyLibrary::Physics::FixPositionWithWallEnemy(std::shared_ptr<Collidable>& col)
{
	float radius = 0.0f;
	MyLibrary::LibVec3 vec;
	float len = 0.0f;
	for (auto& col : col->m_colliders)
	{
		radius = dynamic_cast<MyLibrary::CollidableDataCapsule*> (col.get())->m_radius;
		len = dynamic_cast<MyLibrary::CollidableDataCapsule*> (col.get())->m_len;

		vec = vec.GetNormalized() * len * 0.5f;
	}

	//�ǃ|���S�����Ȃ��ꍇ�͉������Ȃ�
	if (m_wallNum == 0) return;

	//�ǃ|���S���Ƃ̓����蔻�菈��
	//�ǂɓ����������ǂ����̃t���O�͏�����Ԃł́u�������Ă��Ȃ��v�ɂ��Ă���
	m_isHitFlag = false;
	m_isHitWallFlag = false;


	//�ړ��������ǂ����ŏ����𕪊�
	if (col->rigidbody.GetDir().Length() != 0.0f)
	{
		//�ǃ|���S���̐������J��Ԃ�
		for (int i = 0; i < m_wallNum; i++)
		{
			//i�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
			m_pPoly = m_pWallPoly[i];

			//�|���S���ƃv���C���[���������Ă��Ȃ������玟�̃J�E���g��
			if (!HitCheck_Capsule_Triangle(VAdd(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), VSub(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), radius,
				m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2])) continue;

			//�����ɂ�����|���S���ƃv���C���[���������Ă���Ƃ������ƂȂ̂ŁA�|���S���ɓ��������t���O�𗧂Ă�
			m_isHitFlag = true;
			m_isHitWallFlag = true;

			//�ǂ��l�������ړ����O�ς��g���ĎZ�o
			MyLibrary::LibVec3 SlideVec;

			VECTOR ret;
			ret = VCross(col->rigidbody.GetVelocityVECTOR(), m_pPoly->Normal);

			m_ret = VScale(m_pPoly->Normal, 3.0f);

			//�i�s�����x�N�g���ƕǃ|���S���̖@���x�N�g���ɐ����ȃx�N�g�����Z�o
			SlideVec = MyLibrary::LibVec3(ret.x, 0, ret.z);

			//������ړ��O�̍��W�ɑ��������̂�V���ȍ��W�Ƃ���
			col->rigidbody.SetPos(col->rigidbody.GetPos() + SlideVec);

			//�V���Ȉړ����W�ŕǃ|���S���Ɠ������Ă��Ȃ����ǂ����𔻒肷��
			bool isHitWallPolygon = false;
			for (int j = 0; j < m_wallNum; j++)
			{
				//j�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
				m_pPoly = m_pWallPoly[j];

				//�������Ă����烋�[�v���甲����
				if (HitCheck_Capsule_Triangle(VAdd(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), VSub(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), radius,
					m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2]))
				{
					//true�ɂ���
					isHitWallPolygon = true;
					break;
				}
			}

			//�S�Ẵ|���S���Ɠ������Ă��Ȃ������炱���Ń��[�v�I��
			if (!isHitWallPolygon)
			{
				//�q�b�g�t���O��|��
				m_isHitFlag = false;
				m_isHitWallFlag = false;
				break;
			}
		}

		//�ǂɓ������Ă�����ǂ��牟���o���������s��
		if (m_isHitFlag)
		{
			FixPositionWithWallInternal(col);
		}

	}
	else
	{
		//�ړ����Ă��Ȃ��ꍇ�̏���

		//�ǃ|���S���̐������J��Ԃ�
		for (int i = 0; i < m_wallNum; i++)
		{
			//i�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
			m_pPoly = m_pWallPoly[i];

			//�|���S���ɓ������Ă����瓖�������t���O�𗧂Ă���Ń��[�v���甲����
			if (HitCheck_Capsule_Triangle(VAdd(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), VSub(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), radius,
				m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2]))
			{
				m_isHitFlag = true;
				m_isHitWallFlag = true;
				break;
			}
		}


		//�ǂɓ������Ă�����ǂ��牟���o���������s��
		if (m_isHitFlag)
		{
			FixPositionWithWallInternal(col);
		}
	}
}

void MyLibrary::Physics::FixPositionWithWallInternalEnemy(std::shared_ptr<Collidable>& col)
{
	float radius = 0.0f;
	MyLibrary::LibVec3 vec;
	float len = 0.0f;
	for (auto& col : col->m_colliders)
	{
		radius = dynamic_cast<MyLibrary::CollidableDataCapsule*> (col.get())->m_radius;
		len = dynamic_cast<MyLibrary::CollidableDataCapsule*> (col.get())->m_len;

		vec = vec.GetNormalized() * len * 0.5f;
	}

	//�ǂ���̉����o�����������݂�ő吔�����J��Ԃ�
	for (int i = 0; i < ColInfo::kMaxColHitTry; i++)
	{
		//������\���̂���ǃ|���S����S�Č���
		bool isHitWall = false;
		//�ǃ|���S���̐������J��Ԃ�
		for (int j = 0; j < m_wallNum; j++)
		{
			//j�Ԗڂ̕ǃ|���S���̃A�h���X��ǃ|���S���|�C���^�z�񂩂�擾
			m_pPoly = m_pWallPoly[j];

			//�|���S���ƃv���C���[���������Ă��Ȃ������玟�̃J�E���g��
			if (!HitCheck_Capsule_Triangle(VAdd(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), VSub(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), radius,
				m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2])) continue;

			auto ret = VAdd(col->rigidbody.GetNextPosVECTOR(), VScale(m_pPoly->Normal, kColHitSlideLength));

			MyLibrary::LibVec3 set;
			set = MyLibrary::LibVec3(ret.x, ret.y, ret.z);

			//�������Ă�����K�苗�����v���C���[��ǂ̖@�������Ɉړ�������
			col->rigidbody.SetPos(set);

			//�ړ����������ŕǃ|���S���Ɛڑ����Ă��邩�ǂ����𔻒�
			for (int k = 0; k < m_wallNum; k++)
			{
				//�������Ă����烋�[�v�𔲂���
				m_pPoly = m_pWallPoly[k];
				if (HitCheck_Capsule_Triangle(VAdd(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), VSub(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), radius,
					m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2]))
				{
					isHitWall = true;
					break;
				}

			}

			//�S�Ẵ|���S���Ɠ������Ă��Ȃ������炱���Ń��[�v�I��
			if (!isHitWall) break;
		}

		//���[�v�I��
		if (!isHitWall) break;
	}
}

void MyLibrary::Physics::FixNowPositionWithFloorEnemy(std::shared_ptr<Collidable>& col)
{
	//���|���S�����Ȃ��ꍇ�͉������Ȃ�
	if (m_floorNum == 0) return;

	float radius = 0.0f;
	MyLibrary::LibVec3 vec;
	float len = 0.0f;
	for (auto& col : col->m_colliders)
	{
		radius = dynamic_cast<MyLibrary::CollidableDataCapsule*> (col.get())->m_radius;
		len = dynamic_cast<MyLibrary::CollidableDataCapsule*> (col.get())->m_len;

		vec = vec.GetNormalized() * len * 0.5f;
	}

	//���|���S���Ƃ̓����蔻�菈��
	//�����������ǂ����̃t���O������
	bool isHitFlag = false;

	//���|���S���Ƃ̓����蔻��
	//��ԍ������|���S���ɂԂ���ׂ̔���p�ϐ���������
	float PolyMaxPosY = 0.0f;

	//���|���S���̐������J��Ԃ�
	for (int i = 0; i < m_floorNum; i++)
	{
		//i�Ԗڂ̏��|���S���̃A�h���X�����|���S���|�C���^�z�񂩂�擾
		m_pPoly = m_pFloorPoly[i];

		//�|���S���ƃv���C���[���������Ă��Ȃ������玟�̃J�E���g��
		if (HitCheck_Capsule_Triangle(VAdd(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), VSub(col->rigidbody.GetNextPosVECTOR(), vec.ConversionToVECTOR()), radius,
			m_pPoly->Position[0], m_pPoly->Position[1], m_pPoly->Position[2])) continue;

		float mostHeightY = m_pPoly->Position[0].y;

		if (mostHeightY < m_pPoly->Position[1].y)
		{
			mostHeightY = m_pPoly->Position[1].y;
		}

		if (mostHeightY < m_pPoly->Position[2].y)
		{
			mostHeightY = m_pPoly->Position[2].y;
		}

		//���ɓ��������|���S��������A�����܂Ō��o�������|���S�����Ⴂ�ꍇ�͉������Ȃ�
		if (m_isHitFlag && PolyMaxPosY > mostHeightY) continue;

		//�|���S���ɓ��������t���O�𗧂Ă�
		m_isHitFlag = true;

		//�ڐG����Y���W��ێ�����
		PolyMaxPosY = mostHeightY;
	}

	//���|���S���̓����蔻�肩�A�W�����v�͂�0����������(���~���̏ꍇ)�ǂ����ŏ����𕪊�
	if (m_isHitFlag)
	{
		//�ڐG�����|���S���ň�ԍ���Y���W���v���C���[��Y���W�ɂ���
		auto set = col->rigidbody.GetNextPos();
		set.y = PolyMaxPosY + radius;
		col->rigidbody.SetPos(set);
	}
}
