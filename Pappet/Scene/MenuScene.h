#pragma once

/// <summary>
/// ���j���[��ʂ̂��߂̃V�[���N���X
/// </summary>
class MenuScene
{
public:
	//�R���X�g���N�^
	MenuScene();
	//�f�X�g���N�^
	virtual ~MenuScene();

	//����������
	void Init();
	//�X�V����
	void Update();
	//�`�揈��
	void Draw();
	//�I������
	void End();
};

