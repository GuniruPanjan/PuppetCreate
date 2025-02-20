#pragma once
#include "DxLib.h"
#include<memory>

/// <summary>
/// BGM���Ǘ�����N���X
/// </summary>
class BgmManager
{
public:
	//�R���X�g���N�^
	BgmManager();
	//�f�X�g���N�^
	virtual ~BgmManager();

	//�^�C�g���V�[��BGM������
	void TitleInit();
	//�Q�[���V�[���x��BGM������
	void GameRestInit();
	//�Q�[���V�[��1BGM������
	void GameOneInit();
	//�Q�[���V�[��2BGM������
	void GameTwoInit();
	//�Q�[���V�[��3BGM������
	void GameThreeInit();
	//�Q�[���V�[��4BGM������
	void GameFourInit();
	//�Q�[���V�[��5BGM������
	void GameFiveInit();
	//�N���A�V�[��BGM������
	void ClearInit();
	//�X�V����
	void Update(int volume);
	//�^�C�g��BGM�X�V����
	void TitleBGM();
	//�Q�[��BGM�X�V����
	void GameBGM();
	//�{�XBGM�X�V����
	void BossBGM();
	//���S��������BGM���~�߂鏈��
	void BossStopBGM();
	//�N���ABGM�X�V����
	void ClearBGM();
	//�Q�[����BGM����������
	void GameEnd();
	//�I������
	void End();

	//�{�X��BGM�𗬂����̔�����Ƃ�
	int GetBossBGM() { return m_bossBGM; }

private:
	//BGM�֌W
	int m_titleBGM;
	int m_gameBGM;
	int m_bossBGM;
	int m_clearBGM;
	int m_volumePal;
};

