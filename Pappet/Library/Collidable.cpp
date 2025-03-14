#include "MyLibrary.h"
#include "DxLib.h"
#include <cassert>

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="priority">優先度</param>
/// <param name="tag">タグ</param>
MyLibrary::Collidable::Collidable(Priority priority, ObjectTag tag):
	priority(priority),
	tag(tag)
{
}

/// <summary>
/// デストラクタ
/// </summary>
MyLibrary::Collidable::~Collidable()
{
}

/// <summary>
/// 初期化(自身に当たり判定を追加)
/// </summary>
/// <param name="physics">物理クラス</param>
void MyLibrary::Collidable::Init(std::shared_ptr<MyLibrary::Physics> physics)
{
	physics->Entry(shared_from_this());   //物理情報に自身を返す
}

/// <summary>
/// 終了(自身の当たり判定を削除)
/// </summary>
/// <param name="physics">物理クラス</param>
void MyLibrary::Collidable::Finalize(std::shared_ptr<MyLibrary::Physics> physics)
{
	physics->Exit(shared_from_this());    //物理情報登録解除
}

/// <summary>
/// 当たり判定を無視するタグの追加
/// </summary>
/// <param name="tag">追加するタグ</param>
void MyLibrary::Collidable::AddIgnoreTag(ObjectTag tag)
{
	//当たり判定を無視するタグのリストに追加予定のタグが存在するかを確認
	bool found = (std::find(ignoreTags.begin(), ignoreTags.end(), tag) != ignoreTags.end());
	//リストの中に既に存在していたら警告を出す
	if (found)
	{
		assert(0 && "指定タグは既に追加されています");
	}
	else     //存在していなかったら追加する
	{
		ignoreTags.emplace_back(tag);
	}
}

/// <summary>
/// 当たり判定を無視するタグの削除
/// </summary>
/// <param name="tag">削除するタグ</param>
void MyLibrary::Collidable::RemoveIgnoreTag(ObjectTag tag)
{
	//当たり判定を無視するタグのリストに追加予定のタグが存在するかを確認
	bool found = (std::find(ignoreTags.begin(), ignoreTags.end(), tag) != ignoreTags.end());
	//リストの中に既に存在していたら警告を出す
	if (found)
	{
		assert(0 && "指定タグは既に存在していません");
	}
	else
	{
		ignoreTags.emplace_back(tag);
	}
}

/// <summary>
/// 当たり判定を無視する対象かどうか
/// </summary>
/// <param name="target">無視する対象か確認したい当たり判定</param>
/// <returns></returns>
bool MyLibrary::Collidable::IsIgnoreTarget(const std::shared_ptr<Collidable> target) const
{
	//確認したい当たり判定のタグが無視するタグのリストに含まれているかどうか調べる
	bool found = (std::find(ignoreTags.begin(), ignoreTags.end(), target->GetTag()) != ignoreTags.end());
	return found;
}

/// <summary>
/// 当たり判定を追加
/// </summary>
/// <param name="kind">当たり判定の種類</param>
/// <param name="isTrigger">押し出し判定をするかどうか</param>
/// <returns></returns>
std::shared_ptr<MyLibrary::CollidableData> MyLibrary::Collidable::AddCollider(const CollidableData::Kind& kind, bool isTrigger)
{
	std::shared_ptr<CollidableData> add;

	//種類によって追加する当たり判定の派生先を変更する
	if (kind == CollidableData::Kind::Capsule)
	{
		add = std::make_shared<CollidableDataCapsule>(isTrigger);
	}
	else if (kind == CollidableData::Kind::AttackCapsule)
	{
		add = std::make_shared<CollidableDataAttackCapsule>(isTrigger);
	}
	else if (kind == CollidableData::Kind::Sphere)
	{
		add = std::make_shared<CollidableDataSphere>(isTrigger);
	}
	else if (kind == CollidableData::Kind::Rect)
	{
		add = std::make_shared<CollidableDataRect>(isTrigger);
	}
	else if (kind == CollidableData::Kind::ShieldRect)
	{
		add = std::make_shared<CollidableDataShield>(isTrigger);
	}
	else
	{
		assert(0 && "指定された種類の当たり判定を追加できませんでした");
	}

	//当たり判定を追加する
	m_colliders.emplace_back(add);

	return add;
}
