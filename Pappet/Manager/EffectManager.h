#pragma once
#include "EffekseerForDXLib.h"
#include <string>
#include <memory>
#include <map>
#include <vector>
#include "DxLib.h"

/// <summary>
/// �G�t�F�N�g���Ǘ�����N���X
/// </summary>
class EffectManager
{
private:
	//�G�t�F�N�g�\����
	struct Effectstruct
	{
		int handle;
		float x, y, z;
		int frame;
		bool isExist;
	};

	//�G�t�F�N�g�����\����
	struct EffectEmitter
	{
		int emitterHandle = -1;
		const char* effectPath = "";
		std::vector<Effectstruct> effects;
		int endFrame;
	};

private:
	//�V���O���g���p�^�[��
	EffectManager() {};

public:
	virtual ~EffectManager();

	void Init();   //����������

	//�R�s�[�R���X�g���N�^������Ԃ̐������ł��Ă��܂�����
	//�R�s�[�R���X�g���N�^���֎~����
	EffectManager(const EffectManager&) = delete;
	EffectManager& operator=(const EffectManager&) = delete;
	EffectManager(EffectManager&&) = delete;
	EffectManager& operator=(const EffectManager&&) = delete;

	/// <summary>
	/// Effect��GetInstance()��ʂ����Q�Ƃ��炵�����p�ł��Ȃ�
	/// </summary>
	/// <returns></returns>
	static EffectManager& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new EffectManager;
		}

		return *m_instance;
	}

	//���������[�N��h��
	static void Destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:
	//�G�t�F�N�g�����[�h����
	void EffectLoad(std::string name, const char* path, int endFrame, float scale = 1.0f);

	//�X�V
	void Update();

	//�`��
	void Draw();

	//�I������
	void End();

	//�G�t�F�N�g�𐶐�����
	void EffectCreate(std::string name, VECTOR pos);
	//�G�t�F�N�g���폜����
	void RemoveEffect(std::string name, VECTOR pos);

private:
	//Singleton�̃|�C���^���v���O�����N�����Ɉ�����悤�ɂ���
	static EffectManager* m_instance;

private:
	//�G�t�F�N�g�̃n���h��
	std::map<std::string, std::shared_ptr<EffectEmitter>> m_effect;
};


