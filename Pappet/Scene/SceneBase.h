#pragma once
#include "DxLib.h"
#include "Manager/BgmManager.h"
#include "Ui/Setting.h"
#include "Ui/UI.h"
#include "Library/MyLibrary.h"
#include<memory>

/// <summary>
/// �V�[���̃x�[�X
/// </summary>
class SceneBase : public std::enable_shared_from_this<SceneBase>
{
public:
	//�R���X�g���N�^
	SceneBase() :
		m_end(false)
	{}

	//�f�X�g���N�^
	virtual ~SceneBase() {}

	//virtual�Ōp������Ăяo��

	//����������
	virtual void Init() = 0;

	//�X�V����
	virtual std::shared_ptr<SceneBase> Update() = 0;

	//�`�揈��
	virtual void Draw() = 0;

	//�I������
	virtual void End() = 0;

	/// <summary>
	/// ������Ƃ�
	/// </summary>
	/// <returns>�I�������Ԃ�</returns>
	bool GetEnd() { return m_end; }

	/// <summary>
	/// �I����������߂�
	/// </summary>
	/// <param name="end">������Ƃ�</param>
	/// <returns>���ʂ�Ԃ�</returns>
	bool SetEnd(bool end) { return m_end = end; }

protected:
	//�ϐ�������
	int m_backScene = 0;
	bool m_end;   //�Q�[���I���ϐ�

	//�X�}�[�g�|�C���^
	std::shared_ptr<Setting> m_pSetting = std::make_shared<Setting>();
	std::shared_ptr<BgmManager> m_pBgm = std::make_shared<BgmManager>();
	std::shared_ptr<UI> m_pUi = std::make_shared<UI>();
	std::shared_ptr<MyLibrary::Physics> m_pPhysics;

};



