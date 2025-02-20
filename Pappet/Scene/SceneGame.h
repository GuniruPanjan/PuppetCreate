#pragma once
#include "SceneBase.h"
#include "Manager/GameManager.h"

/// <summary>
/// �Q�[���V�[�����Ǘ�����N���X
/// </summary>
class SceneGame : public SceneBase
{
public:
	//�R���X�g���N�^
	SceneGame();

	//�f�X�g���N�^
	virtual ~SceneGame();

	//����������
	virtual void Init();

	//�X�V����
	virtual std::shared_ptr<SceneBase> Update();

	//�`�揈��
	virtual void Draw();

	//�I������
	virtual void End();

private:
	//�X�}�[�g�|�C���^�Ǘ�
	std::shared_ptr<GameManager> game = std::make_shared<GameManager>();
};
