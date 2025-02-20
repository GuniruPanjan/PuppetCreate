#pragma once
#include "Library/MyLibrary.h"
#include "Map/MapBase.h"
#include <memory>

/// <summary>
/// �}�b�v���Ǘ�����N���X
/// </summary>
class MapManager
{
public:
	//�R���X�g���N�^
	MapManager();
	//�f�X�g���N�^
	virtual ~MapManager();

	//�f�[�^�̏�����
	void DataInit(int map);
	//����������
	void Init(std::shared_ptr<MyLibrary::Physics> physics);
	//�X�V����
	void Update(std::shared_ptr<MyLibrary::Physics> physics, bool warp, bool enter, bool Dead);
	//���[�v�̍X�V����
	void WarpUpdate(std::shared_ptr<MyLibrary::Physics> physics, bool warp, bool title);
	//�����������̔��菈��
	void JudgeUpdate();
	//�R�A�̍X�V
	void CoreUpdate();
	//���������������
	void TriggerReset();
	//�`�揈��
	void Draw();
	//�R�A�̕`��
	void CoreDraw();
	//�I������
	void End(std::shared_ptr<MyLibrary::Physics> physics, bool title);

	//�}�b�v�n���h����Ԃ�
	int GetMap() { return m_pMap->GetMap(); }

	//�R���W�����}�b�v��Ԃ�
	int GetCollisionMap() { return m_pMap->GetCollisionMap(); }

	//�x���n�_��Ԃ�
	bool GetRest() { return m_pMap->GetRest(); }

	//�{�X����
	bool GetBossRoom() { return m_pMap->GetBossRoom(); }

	//�R�A�̓����蔻���Ԃ�
	bool GetCore() { return m_pMap->GetCore(); }

	//�{�X�����ɓ��邽�߂̔����Ԃ�
	bool GetBossEnter() { return m_pMap->GetBossEnter(); }

	//�x���n�_��Ԃ�
	VECTOR GetRestPos() { return m_pMap->GetRestPos(); }

	//�X�e�[�W����Ԃ�
	const char* GetStageName() { return m_pMap->GetStageName(); }
private:
	//�X�}�[�g�|�C���^
	std::shared_ptr<MapBase> m_pMap;
};

