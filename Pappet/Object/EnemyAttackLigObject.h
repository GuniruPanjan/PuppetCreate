#pragma once
#include "ObjectBase.h"


/// <summary>
/// 敵のリグの当たり判定クラス
/// </summary>
class EnemyAttackLigObject : public ObjectBase
{
public:
	EnemyAttackLigObject(MyLibrary::LibVec3 pos1, MyLibrary::LibVec3 pos2, float radius);
	~EnemyAttackLigObject();

	void Init(std::shared_ptr<MyLibrary::Physics> physics);
	void Update(MyLibrary::LibVec3 pos1, MyLibrary::LibVec3 pos2);

	//当たり判定を削除
	void Finalize(std::shared_ptr<MyLibrary::Physics> physics) override;

	void CollisionEnd();

	void OnTriggerEnter(const std::shared_ptr<Collidable>& collidable) override;
	//他のオブジェクトに当たったかどうか
	bool GetIsTrigger();
	//攻撃力を設定
	int SetAttack(int attack) { return m_attack = attack; }
	//攻撃力を取得
	const int GetAttack() const { return m_attack; }
	//判定をリセット
	void IsTriggerReset();

private:
	std::shared_ptr<MyLibrary::Physics> m_pPhysics;

	//当たり判定の切り替え
	bool m_isCollisionOn = false;

	int m_attack;
};

