#pragma once
#include "ObjectBase.h"

/// <summary>
/// 矩形用のトリガークラス
/// </summary>
class RectObjectTrigger : public ObjectBase
{
public:
	RectObjectTrigger(float width, float hight, float depth);
	~RectObjectTrigger();

	void Init(std::shared_ptr<MyLibrary::Physics> physics, MyLibrary::LibVec3 pos, bool isEnter = false);
	void Update(MyLibrary::LibVec3 pos, MyLibrary::LibVec3::Size size);

	void OnCollideEnter(const std::shared_ptr<Collidable>& collidable) {};

	void Finalize(const std::shared_ptr<MyLibrary::Physics> physics) override;

	void CollisionEnd();

	void OnTriggerEnter(const std::shared_ptr<Collidable>& collidable) override;
	void OnTriggerStay(const std::shared_ptr<Collidable>& collidable) override;
	void OnTriggerExit(const std::shared_ptr<Collidable>& collidable) override;

	bool GetIsTrigger();
	bool GetIsStay() const;
	bool GetIsExit();

	void IsTriggerReset();

private:
	std::shared_ptr<MyLibrary::Physics> m_pPhysics;

	bool m_isEnter = false;
	bool m_isTriggerEnter = false;
	bool m_isTriggerStay = false;
	bool m_isTriggerExit = false;
	bool m_isCollisionOn = false;
};

