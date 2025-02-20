#pragma once
#include "CharacterBase.h"

/// <summary>
/// �����蔻��p��NPC
/// </summary>
class CollidableNpc : public CharacterBase
{
public:
	//�R���X�g���N�^
	CollidableNpc();
	//�f�X�g���N�^
	virtual ~CollidableNpc();

	void Init(std::shared_ptr<MyLibrary::Physics> physics);
	void Update();
	void Draw();

	//�Փ˂����Ƃ�
	void OnCollideEnter(const std::shared_ptr<Collidable>& collidable) override;
	void OnTriggerEnter(const std::shared_ptr<Collidable>& collidable) override;


	void SetModelPos();
};

