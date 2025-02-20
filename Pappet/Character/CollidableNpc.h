#pragma once
#include "CharacterBase.h"

/// <summary>
/// 当たり判定用のNPC
/// </summary>
class CollidableNpc : public CharacterBase
{
public:
	//コンストラクタ
	CollidableNpc();
	//デストラクタ
	virtual ~CollidableNpc();

	void Init(std::shared_ptr<MyLibrary::Physics> physics);
	void Update();
	void Draw();

	//衝突したとき
	void OnCollideEnter(const std::shared_ptr<Collidable>& collidable) override;
	void OnTriggerEnter(const std::shared_ptr<Collidable>& collidable) override;


	void SetModelPos();
};

