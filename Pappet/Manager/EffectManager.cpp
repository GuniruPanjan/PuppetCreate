#include "EffectManager.h"
#include <cassert>

EffectManager* EffectManager::m_instance = nullptr;

/// <summary>
/// �f�X�g���N�^
/// </summary>
EffectManager::~EffectManager()
{
	//�����p�X�̃G�t�F�N�g�����[�h����Ă��Ȃ����m�F����
	for (auto& effect : m_effect)
	{
		DeleteEffekseerEffect(effect.second->emitterHandle);
	}
}

/// <summary>
/// ����������
/// </summary>
void EffectManager::Init()
{
	//�񕜃G�t�F�N�g
	EffectLoad("Heel", "Data/Effect/AnotherEffect/Sylph13.efkefc", 160, 20.0f);
	//�A�C�e���G�t�F�N�g
	EffectLoad("Item", "Data/Effect/Item.efkefc", 60, 5.0f);
	//�x���G�t�F�N�g
	EffectLoad("Rest", "Data/Effect/Benediction.efkefc", 210, 10.0f);
	//�F�̕K�E�G�t�F�N�g
	EffectLoad("BearLance", "Data/Effect/BloodLance.efkefc", 350, 10.0f);
	//���G�t�F�N�g
	EffectLoad("Smoke", "Data/Effect/Smoke.efkefc", 50, 30.0f);
	//�c�݃G�t�F�N�g
	EffectLoad("Distortion", "Data/Effect/effect2.efkefc", 120, 10.0f);
	//���b�Z�[�W�G�t�F�N�g
	EffectLoad("Message", "Data/Effect/message.efkefc", 50, 10.0f);
	//Hit�G�t�F�N�g
	EffectLoad("Hit", "Data/Effect/HitEffect.efkefc", 30, 15.0f);
}

/// <summary>
/// �G�t�F�N�g���[�h
/// </summary>
/// <param name="name">�G�t�F�N�g�̖��O</param>
/// <param name="path">���[�h���邽�߂̃p�X</param>
/// <param name="endFrame">�I���t���[��</param>
/// <param name="scale">�g�嗦</param>
void EffectManager::EffectLoad(std::string name, const char* path, int endFrame, float scale)
{
	//�����p�X�̃G�t�F�N�g�����[�h����Ă��Ȃ����m�F����
	for (auto& effect : m_effect)
	{
		if (effect.second->effectPath == path)
		{
			return;
		}
	}

	//�����܂ŗ����烍�[�h����
	std::shared_ptr<EffectEmitter> add = std::make_shared<EffectEmitter>();
	add->emitterHandle = LoadEffekseerEffect(path, scale);
	assert(add->emitterHandle != -1 && "�G�t�F�N�g���[�h���s");
	add->endFrame = endFrame;

	m_effect[name] = add;
}

/// <summary>
/// �X�V
/// </summary>
void EffectManager::Update()
{

	Effekseer_Sync3DSetting();


	UpdateEffekseer3D();

	//�G�t�F�N�g�̍X�V
	for (auto& effects : m_effect)
	{
		for (auto& ef : effects.second->effects)
		{
			if (ef.frame > effects.second->endFrame)
			{
				StopEffekseer3DEffect(ef.handle);
				ef.isExist = false;
			}

			ef.frame++;
		}

		//isExist��false�̃I�u�W�F�N�g���폜
		auto it = std::remove_if(effects.second->effects.begin(), effects.second->effects.end(), [](auto& v)
			{
				return v.isExist = false;
			});
		effects.second->effects.erase(it, effects.second->effects.end());
	}
}

/// <summary>
/// �`��
/// </summary>
void EffectManager::Draw()
{
	DrawEffekseer3D();
}

/// <summary>
/// �I������
/// </summary>
void EffectManager::End()
{
	for (auto& effect : m_effect)
	{
		DeleteEffekseerEffect(effect.second->emitterHandle);
	}
}

/// <summary>
/// �G�t�F�N�g�𐶐�
/// </summary>
/// <param name="name">�G�t�F�N�g�̖��O</param>
/// <param name="pos">�`����W</param>
void EffectManager::EffectCreate(std::string name, VECTOR pos)
{
	int handle = -1;
	for (auto& effects : m_effect)
	{
		if (effects.first == name)
		{
			handle = effects.second->emitterHandle;
		}
	}

	//�ǉ����悤�Ƃ����G�t�F�N�g�̖��O�̃n���h�����Ȃ���ΏI���
	if (handle == -1)
	{
		return;
	}

	Effectstruct add;
	add.handle = PlayEffekseer3DEffect(handle);
	add.frame = 0;
	add.x = pos.x;
	add.y = pos.y;
	add.z = pos.z;
	add.isExist = true;

	SetPosPlayingEffekseer3DEffect(add.handle, add.x, add.y, add.z);

	m_effect[name]->effects.emplace_back(add);

	return;
}

/// <summary>
/// �G�t�F�N�g���폜
/// </summary>
/// <param name="name"></param>
/// <param name="pos"></param>
void EffectManager::RemoveEffect(std::string name, VECTOR pos)
{
	auto it = m_effect.find(name);
	if (it != m_effect.end())
	{
		auto& effects = it->second->effects;
		effects.erase(std::remove_if(effects.begin(), effects.end(),
			[&pos](const Effectstruct& effect) {
				return effect.x == pos.x && effect.y == pos.y && effect.z == pos.z;
			}), effects.end());
	}
}
