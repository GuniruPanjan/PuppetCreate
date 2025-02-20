#pragma once
#include "SceneBase.h"
#include <memory>
#include <string>
#include "DxLib.h"

//class SceneBase;

/// <summary>
/// �V�[�����Ǘ�����N���X
/// </summary>
class SceneManager
{
public:
	//�R���X�g���N�^
	SceneManager();
	//�f�X�g���N�^
	virtual ~SceneManager();

	//����������
	void Init();

	//�X�V����
	void Update();

	//�`�揈��
	void Draw();

	//�I������
	void End();

	//�X�e�[�W����ݒ肷��
	void SetStageName(std::string name) { m_stageName = name; }
	//�X�e�[�W�����擾����
	const std::string GetStageName() const { return m_stageName; }

	//�v���O�������������锻���Ԃ�
	bool GetEnd() { return m_pScene->GetEnd(); }

private:

	//�X�}�[�g�|�C���^
	std::shared_ptr<SceneBase> m_pScene;

	//�X�e�[�W��
	std::string m_stageName;
};




