#include "Rigidbody.h"

/// <summary>
/// コンストラクタ
/// </summary>
MyLibrary::Rigidbody::Rigidbody():
	m_pos(),
	m_nextPos(),
	m_vec(),
	m_dir(),
	m_velocity(),
	m_pos1(),
	m_pos2(),
	m_useGravity(true)
{
}

/// <summary>
/// 初期化処理
/// </summary>
/// <param name="gravity">重力</param>
void MyLibrary::Rigidbody::Init(bool gravity)
{
	m_pos = LibVec3();
	m_pos1 = LibVec3();
	m_pos2 = LibVec3();
	m_dir = LibVec3();
	m_velocity = LibVec3();
	this->m_useGravity = gravity;
}

/// <summary>
/// velocityに力を加える
/// </summary>
/// <param name="force">加える力</param>
void MyLibrary::Rigidbody::AddForce(const LibVec3& force)
{
	m_velocity = m_velocity + force;
}

/// <summary>
/// VelocityのSetとDir自動計算
/// </summary>
/// <param name="set">Velocity</param>
/// <param name="mul">向き</param>
void MyLibrary::Rigidbody::SetVelocity(LibVec3 set, float mul)
{
	m_velocity = set * mul;

	if (m_velocity.SqLength() > 0)
	{
		m_dir = m_velocity.Normalize();
	}
}
