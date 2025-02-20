#pragma once
#include "DxLib.h"
#include "LibVec3.h"

namespace MyLibrary
{
	/// <summary>
	/// 物理、衝突判定に必要なデータの塊
	/// </summary>
	class Rigidbody
	{
	public:
		//コンストラクタ
		Rigidbody();

		//初期化
		void Init(bool gravity = false);

		//velocityに力を加える
		void AddForce(const LibVec3& force);

		//現在座標を取得
		const LibVec3& GetPos() const { return m_pos; }
		//大きさを取得
		const LibVec3::Size& GetSize() const { return m_size; }
		//移動予定座標
		const LibVec3& GetNextPos() const { return m_nextPos; }
		//向きを所得
		const LibVec3& GetDir() const { return m_dir; }
		//移動速度を所得
		const LibVec3& GetVelocity() const { return m_velocity; }
		//攻撃用のカプセルのポジションを取得
		const LibVec3& GetAttackPos1() const { return m_pos1; }
		const LibVec3& GetAttackPos2() const { return m_pos2; }

		//現在座標を所得
		const VECTOR GetPosVECTOR() { return m_pos.ConversionToVECTOR(); }
		//移動予定座標を所得
		const VECTOR GetNextPosVECTOR() { return m_nextPos.ConversionToVECTOR(); }
		//ベクターを取得
		const VECTOR GetVec() { return m_vec.ConversionToVECTOR(); }
		//向きを所得
		const VECTOR GetDirVECTOR() { return m_dir.ConversionToVECTOR(); }
		//移動速度を所得
		const VECTOR GetVelocityVECTOR() { return m_velocity.ConversionToVECTOR(); }
		//攻撃用のカプセルのポジションを取得
		const VECTOR GetAttackPos1VECTOR() { return m_pos1.ConversionToVECTOR(); }
		const VECTOR GetAttackPos2VECTOR() { return m_pos2.ConversionToVECTOR(); }
		//重力を与えるかどうかを所得
		bool GetUseGravity() const { return m_useGravity; }


		//現在座標を設定
		void SetPos(const LibVec3& set) { m_pos = set; }
		//移動予定座標を設定
		void SetNextPos(const LibVec3& set) { m_nextPos = set; }
		//ベクターを設定
		void SetVec(const LibVec3& set) { m_vec = set; }
		//大きさを設定
		void SetSize(const LibVec3::Size set) { m_size = set; }
		//向きを設定
		void SetVelocity(LibVec3 set, float mul = 1.0f);
		//移動速度を設定
		void SetUseGravity(bool set) { m_useGravity = set; }
		//攻撃用のカプセルのポジション
		void SetAttackPos1(const LibVec3& set) { m_pos1 = set; }
		void SetAttackPos2(const LibVec3& set) { m_pos2 = set; }

	private:
		//現在座標
		LibVec3 m_pos;
		//移動予定座標
		LibVec3 m_nextPos;
		//ベクター
		LibVec3 m_vec;
		//大きさ
		LibVec3::Size m_size;
		//向き
		LibVec3 m_dir;
		//移動速度
		LibVec3 m_velocity;
		//攻撃用のカプセルのポジション
		LibVec3 m_pos1, m_pos2;
		//重力を与えるか判定する
		bool m_useGravity;
		//高さが合わないため
		LibVec3 m_higth = MyLibrary::LibVec3(0.0f, 50.0f, 0.0f);
	};
}


