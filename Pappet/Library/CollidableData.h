#pragma once

namespace MyLibrary
{
	/// <summary>
	/// 当たり判定のデータ基底
	/// </summary>
	class CollidableData
	{
	public:
		//当たり判定種別
		enum class Kind
		{
			Capsule,        //カプセル型
			AttackCapsule,  //攻撃型のカプセル型
			Sphere,         //球体型
			Rect,           //四角型
			ShieldRect,     //防御型
		};

		//コンストラクタ
		CollidableData(Kind kind, bool isTrigger)
		{
			this->kind = kind;
			this->m_isTrigger = isTrigger;
		}

		virtual ~CollidableData() {}

		//当たり判定種別所得
		Kind GetKind() const { return kind; }

		//トリガーかどうか
		bool IsTrigger() const { return m_isTrigger; }

	private:
		Kind kind;
		bool m_isTrigger;
	};
}


