#pragma once
#include "ObjectBase.h"

/// <summary>
/// プレイヤーの索敵クラス
/// </summary>
class PlayerSearchObject : public ObjectBase
{
public:
	PlayerSearchObject(float radius);
	~PlayerSearchObject();

	void Init(std::shared_ptr<MyLibrary::Physics> physics, MyLibrary::LibVec3 pos);
	void Update(MyLibrary::LibVec3 pos);

	void OnCollideEnter(const std::shared_ptr<Collidable>& collidable) {};

	void Finalize(std::shared_ptr<MyLibrary::Physics> physics) override;

	void CollisionEnd();

	void OnTriggerEnter(const std::shared_ptr<Collidable>& collidable) override;
	void OnTriggerStay(const std::shared_ptr<Collidable>& collidable) override;
	void OnTriggerExit(const std::shared_ptr<Collidable>& collidable) override;

	bool GetIsTrigger();
	bool GetIsStay()const;
	bool GetIsExit();

	void IsTriggerReset();

private:
	std::shared_ptr<MyLibrary::Physics> m_pPhysics;

	bool m_isTriggerEnter = false;
	bool m_isTriggerStay = false;
	bool m_isTriggerExit = false;
	bool m_isCollisionOn = false;
};

