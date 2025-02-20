#pragma once
#include "Library/MyLibrary.h"
#include "Object/SearchObject.h"
#include "Object/RectObject.h"
#include "Object/RectObjectTrigger.h"
#include <string>

/// <summary>
/// �}�b�v�̊��N���X
/// </summary>
class MapBase : public std::enable_shared_from_this<MapBase>
{
public:
	//�R���X�g���N�^
	MapBase();
	//�f�X�g���N�^
	virtual ~MapBase();

	//�f�[�^������
	virtual void DataInit() = 0;
	//������
	virtual void Init(std::shared_ptr<MyLibrary::Physics> physics) = 0;
	//�I��
	virtual void Finalize(std::shared_ptr<MyLibrary::Physics> physics);
	//�X�V
	virtual std::shared_ptr<MapBase> Update(bool warp, bool enter, bool Dead) = 0;
	//���[�v�X�V
	virtual std::shared_ptr<MapBase> WarpUpdate(bool warp) = 0;
	//�����������̔���X�V
	virtual void JudgeUpdate() = 0;
	//�{�X�����񂾂�o��
	virtual void CoreUpdate() = 0;
	//���������������
	void TriggerReset() { m_pBossRoom->IsTriggerReset(); }
	//�`��
	virtual void Draw() = 0;
	//�R�A�̕`��
	virtual void CoreDraw() = 0;
	//�I��
	virtual void End(std::shared_ptr<MyLibrary::Physics> physics, bool title) = 0;

	//�I������
	void MapEnd(std::shared_ptr<MyLibrary::Physics> physics);

	//�}�b�v�n���h����Ԃ�
	int GetMap() { return m_handle; }
	//�R���W�����̃}�b�v�n���h����Ԃ�
	int GetCollisionMap() { return m_collisionHandle; }
	//�x���ł���悤�ɓ������Ă��邩��Ԃ�
	bool GetRest() { return m_pSearch->GetIsStay(); }
	//�{�X�����̓����
	bool GetBossRoom() { return m_pBossRoom->GetIsTrigger(); }
	//�R�A�̓����蔻���Ԃ�
	bool GetCore() { return m_pCore->GetIsStay(); }
	//�}�b�v�̃{�X���������Ԃ�
	bool GetBossEnter() { return m_pRectTrigger->GetIsStay(); }
	//�}�b�v�̋x���n�_��Ԃ�
	VECTOR GetRestPos() { return m_mapRestPos.ConversionToVECTOR(); }
	//�X�e�[�W����Ԃ�
	const char* GetStageName() { return m_stageName; }
protected:
	//���f����ǂݍ���
	void LoadData(std::string mapPath, std::string collisionPath, std::string corePath);
	//���G��������铖���蔻����쐬
	void InitSearch(float radius, MyLibrary::LibVec3 pos);
	//�{�X�����̓����
	void InitBossRoom(float radius, MyLibrary::LibVec3 pos);
	//�{�X�����̓���
	void InitRect(float width, float hight, float depth, MyLibrary::LibVec3 pos);
	//�R�A�̓����蔻��
	void InitCore(float radius, MyLibrary::LibVec3 pos);
	//�{�X�����ɓ��邽�߂̔���
	void InitBossEnter(float width, float hight, float depth, MyLibrary::LibVec3 pos);
protected:
	//�����N���X�̃|�C���^
	std::shared_ptr<MyLibrary::Physics> m_pPhysics;

	std::shared_ptr<SearchObject> m_pSearch;             //���G����
	std::shared_ptr<SearchObject> m_pBossRoom;           //�{�X�����̓��������
	std::shared_ptr<SearchObject> m_pCore;               //�R�A�̓����蔻��
	std::shared_ptr<RectObject> m_pRect;                 //��`����
	std::shared_ptr<RectObjectTrigger> m_pRectTrigger;   //��`�̃g���K�[����

	int m_light;                            //���C�g

	int m_handle;                           //���f���̃n���h��
	int m_collisionHandle;                  //���f���̓����蔻��p�̃n���h��
	int m_coreHandle;                       //�R�A�̃n���h��
	float m_size;                           //�}�b�v�̃T�C�Y
	float m_width;                          //��
	float m_hight;                          //����
	float m_depth;                          //���s
	float m_angle;                          //��]������A���O��
	const char* m_stageName;                //�X�e�[�W��

	VECTOR m_mapPos;                                  //�}�b�v�̃|�W�V����
	VECTOR m_mapCollisionPos;                         //�}�b�v�̃R���W�����̃|�W�V����
	VECTOR m_mapCorePos;                              //�}�b�v�̃R�A�|�W�V����
	MyLibrary::LibVec3 m_mapRestPos;                  //�x���|�C���g�̃|�W�V����
	MyLibrary::LibVec3 m_mapBossRoomPos;              //�{�X����������̃|�W�V����
	MyLibrary::LibVec3 m_mapBossEnterPos;             //�{�X����������̃|�W�V����
	MyLibrary::LibVec3 m_mapCoreCollisionePos;        //�R�A�̃|�W�V����
	MyLibrary::LibVec3 m_mapBossEnterTriggerPos;      //�{�X�����ɓ��邽�߂̃|�W�V����
};

