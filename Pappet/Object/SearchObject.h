#pragma once
#include "ObjectBase.h"
class SearchObject : public ObjectBase
{
public:
	SearchObject(float radius);
	~SearchObject();

	void Init(std::shared_ptr<MyLibrary::Physics> physics, MyLibrary::LibVec3 pos, bool isEnemy = false, bool isRest = false, bool isItem = false, bool isBossRoom = false, bool isCore = false);
	void Update(MyLibrary::LibVec3 pos);

	void OnCollideEnter(const std::shared_ptr<Collidable>& collidable) {};

	void Finalize(std::shared_ptr<MyLibrary::Physics> physics) override;

	void OnTriggerEnter(const std::shared_ptr<Collidable>& collidable) override;
	void OnTriggerStay(const std::shared_ptr<Collidable>& collidable) override;
	void OnTriggerExit(const std::shared_ptr<Collidable>& collidable) override;

	bool GetIsTrigger();
	bool GetIsStay()const;
	bool GetIsExit();

	void IsTriggerReset();

private:
	std::shared_ptr<MyLibrary::Physics> m_pPhysics;

	bool m_isEnemy = false;
	bool m_isRest = false;
	bool m_isItem = false;
	bool m_isBossRoom = false;
	bool m_isCore = false;
	bool m_isTriggerEnter = false;
	bool m_isTriggerStay = false;
	bool m_isTriggerExit = false;
};
