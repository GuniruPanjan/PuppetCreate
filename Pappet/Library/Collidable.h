#pragma once
#include "ObjectTag.h"
#include <List>
#include <memory>

namespace MyLibrary
{

	class Rigidbody;
	class Physics;
	class CollidableData;

	class Collidable : public std::enable_shared_from_this<Collidable>
	{
		friend Physics;
	public:

		//位置補正の優先度の判別
		enum class Priority : int
		{
			Low,        //低
			Middle,     //中
			High,       //高
			Static,     //動かない
		};

	public:
		//コンストラクタ
		Collidable(Priority priority, ObjectTag tag);
		//デストラクタ
		virtual ~Collidable();
		//初期化
		virtual void Init(std::shared_ptr<MyLibrary::Physics> physics);
		//終了
		virtual void Finalize(std::shared_ptr<MyLibrary::Physics> physics);

		//当たり判定関数
		virtual void OnCollideEnter(const std::shared_ptr<Collidable>& collider){}
		virtual void OnCollideStay(const std::shared_ptr<Collidable>& collider){}
		virtual void OnCollideExit(const std::shared_ptr<Collidable>& collider){}
		virtual void OnTriggerEnter(const std::shared_ptr<Collidable>& collider){}
		virtual void OnTriggerStay(const std::shared_ptr<Collidable>& collider){}
		virtual void OnTriggerExit(const std::shared_ptr<Collidable>& collider){}

		//タグを所得
		ObjectTag GetTag() const { return tag; }
		//優先度を所得
		Priority GetPriority() const { return priority; }

		//当たり判定を無視するタグの追加と削除
		void AddIgnoreTag(ObjectTag tag);
		void RemoveIgnoreTag(ObjectTag tag);

		//当たり判定を無視する対象かどうか
		bool IsIgnoreTarget(const std::shared_ptr<Collidable> target) const;

	protected:
		//当たり判定を追加
		std::shared_ptr<CollidableData> AddCollider(const CollidableData::Kind& kind, bool isTrigger);

	protected:
		//物理データ
		std::shared_ptr<Rigidbody> rigidbody;
		//当たり判定情報
		std::list<std::shared_ptr<CollidableData>> m_colliders;


	private:
		//タグ
		ObjectTag tag;
		//優先度
		Priority priority;

		//当たり判定を無視するタグのリスト
		std::list<ObjectTag> ignoreTags;
	};

}


