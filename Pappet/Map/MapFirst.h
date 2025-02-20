#pragma once
#include "MapBase.h"

/// <summary>
/// �ŏ��̃}�b�v�N���X
/// </summary>
class MapFirst : public MapBase
{
public:
	//�R���X�g���N�^
	MapFirst();
	//�f�X�g���N�^
	virtual ~MapFirst();

	//�f�[�^�̏�����
	virtual void DataInit();
	//������
	virtual void Init(std::shared_ptr<MyLibrary::Physics> physics);
	//�X�V
	virtual std::shared_ptr<MapBase> Update(bool warp, bool enter, bool Dead = false);
	//���[�v�̍X�V
	virtual std::shared_ptr<MapBase> WarpUpdate(bool warp);
	//�����������̔���
	virtual void JudgeUpdate();
	//�{�X�����񂾂�o��
	virtual void CoreUpdate();
	//�`��
	virtual void Draw();
	//�R�A�`��
	virtual void CoreDraw();
	//�I��
	virtual void End(std::shared_ptr<MyLibrary::Physics> physics, bool title);

};

