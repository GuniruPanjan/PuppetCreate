#include "SceneTitle.h"
#include "SceneGame.h"
#include "Manager/HandleManager.h"
#include "Manager/SelectManager.h"
#include "Map/Map.h"
#include "Manager/MapManager.h"
#include "Manager/EffectManager.h"

namespace
{
	int selectDecision = 0;  //�I�����A���肵������

	//float cCameraTargetx = 485.0f;
	//float cCameraTargety = 80.0f;
	//float cCameraTargetz = -550.0f;

	float cCameraTargetx = -250.0f;
	float cCameraTargety = 115.0f;
	float cCameraTargetz = -270.0f;

	bool cCameraTrun = false;

	int cHandY = 600;

	//�V���O���g��
	auto& handle = HandleManager::GetInstance();
	auto& cEffect = EffectManager::GetInstance();
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
SceneTitle::SceneTitle() :
	m_start(0),
	m_setting(0),
	m_end(0),
	m_button(0),
	m_hand(0),
	m_BButton(0),
	m_AButton(0),
	m_one(false),
	m_blend(false),
	m_setButton(false),
	m_waitTime(0),
	m_cameraPos(VGet(0.0f, 0.0f, 0.0f)),
	m_playerHandle(0),
	m_anim(0),
	m_animation(0),
	m_totalAnimationTime(0.0f),
	m_playTime(0.0f),
	m_pos(VGet(0.0f, 0.0f, 0.0f)),
	m_cameraTarget(VGet(0.0f, 0.0f, 0.0f)),
	m_xpad()
{
	for (int i = 0; i < 3; i++)
	{
		m_select[i] = 0;
		m_pal[i] = 0;
	}
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
SceneTitle::~SceneTitle()
{
	//���������
	DeleteGraph(m_backScene);
	DeleteGraph(m_start);
	DeleteGraph(m_setting);
	DeleteGraph(m_end);
	DeleteGraph(m_hand);
	DeleteGraph(m_BButton);
	DeleteGraph(m_AButton);
	MV1DeleteModel(m_playerHandle);
	MV1DeleteModel(m_anim);
	m_pSetting->End();
	m_pBgm->End();
	pse->End();

	handle.Clear();
}

/// <summary>
/// ����������
/// </summary>
void SceneTitle::Init()
{
	//�������ǂݍ���
	m_backScene = m_pUi->MyLoadGraph("Data/SceneBack/PuppetGraves�^�C�g��mini.png", 1, 1);     //144 KB (147,793 �o�C�g)
	m_start = m_pUi->MyLoadGraph("Data/UI/STARTButtonMini.png", 1, 1);                         //27.1 KB (27,851 �o�C�g)
	m_setting = m_pUi->MyLoadGraph("Data/UI/SettingButtonMini.png", 1, 1);                     //29.4 KB (30,170 �o�C�g)
	m_end = m_pUi->MyLoadGraph("Data/UI/EndButtonMini.png", 1, 1);                             //22.5 KB (23,109 �o�C�g)
	m_hand = m_pUi->MyLoadGraph("Data/UI/PuppetHand.png", 2, 2);                         
	m_BButton = m_pUi->MyLoadGraph("Data/UI/BButton.png", 1, 1);
	m_AButton = m_pUi->MyLoadGraph("Data/UI/AButton.png", 1, 1);

	m_playerHandle = handle.GetModelHandle("Data/Player/PuppetPlayerModel.mv1");
	m_anim = handle.GetModelHandle("Data/PlayerAnimation/JumpingDown.mv1");

	m_pMap->DataInit(0);
	cEffect.Init();

	m_pPhysics = std::make_shared<MyLibrary::Physics>(m_pMap->GetCollisionMap());

	//pmap->Init();
	m_pMap->Init(m_pPhysics);

	selectDecision = 0;

	//�A�j���[�V�����A�^�b�`
	m_animation = MV1AttachAnim(m_playerHandle, 1, m_anim, TRUE);

	//����
	m_totalAnimationTime = MV1GetAttachAnimTotalTime(m_playerHandle, m_animation);

	m_playTime = 28.0f;

	m_pos = VGet(-145.0f, 0.0f, -270.0f);

	m_select[0] = 1;
	m_select[1] = 0;
	m_select[2] = 0;

	for (int i = 0; i < 3; i++)
	{
		m_pal[i] = 255;
	}

	cCameraTrun = false;


	//m_cameraPos = VGet(550.0f, 20.0f, -450.0f);
	m_cameraPos = VGet(-80.0f, 35.0f, 80.0f);
	m_cameraTarget = VGet(cCameraTargetx, cCameraTargety, cCameraTargetz);

	//�ݒ�֌W
	m_pSetting->Init();
	m_pBgm->TitleInit();
	m_pBgm->TitleBGM();
	pse->SceneInit();

	m_one = false;
	m_blend = false;
}

/// <summary>
/// �X�V����
/// </summary>
/// <returns>�V�[����Ԃ�</returns>
std::shared_ptr<SceneBase> SceneTitle::Update()
{
	cEffect.Update();
	m_pMap->Update(m_pPhysics, false, false, false);

	//�J�����㉺�ɓ����p
	if (cCameraTargety >= 140.0f)
	{
		cCameraTrun = false;
	}
	else if (cCameraTargety <= 100.0f)
	{
		cCameraTrun = true;
	}

	if (!cCameraTrun)
	{
		cCameraTargety -= 0.2f;
	}
	else if (cCameraTrun)
	{
		cCameraTargety += 0.2f;
	}

	m_cameraTarget = VGet(cCameraTargetx, cCameraTargety, cCameraTargetz);

	if (m_pSetting->GetSettingScene() == false)
	{
		//�p�b�h���͏���
		GetJoypadXInputState(DX_INPUT_KEY_PAD1, &m_xpad);

		//��
		if (m_xpad.ThumbLY > 2000 || m_xpad.Buttons[0] == 1)
		{
			m_button++;
		}
		//��
		else if (m_xpad.ThumbLY < 0 || m_xpad.Buttons[1] == 1)
		{
			m_button--;
		}
		else
		{
			//������
			m_button = 0;

			m_one = false;
		}

		m_playTime += 0.5f;

		//�I������
		if (m_pSetting->GetSettingScene() == false)
		{
			pselect->Menu_Update(m_button, m_one, m_xpad.Buttons[12], selectDecision, pselect->Eight);
		}

		if (pselect->NowSelect == 7)
		{
			cHandY = 600;
		}
		else if (pselect->NowSelect == 8)
		{
			cHandY = 750;
		}
		else if (pselect->NowSelect == 9)
		{
			cHandY = 900;
		}

		if (m_waitTime > 50)
		{
			//A�{�^������������
			if (m_xpad.Buttons[12] == 1 || CheckHitKey(KEY_INPUT_A) == 1)
			{
				PlaySoundMem(pse->GetButtonSE(), DX_PLAYTYPE_BACK, true);

				//�Q�[���X�^�[�g
				if (selectDecision == 8)
				{
					m_pMap->End(m_pPhysics, true);
					m_pSetting->End();

					return std::make_shared<SceneGame>();

					//pmap->End();
				}
				//�ݒ�
				if (selectDecision == 9)
				{
					m_setButton = true;

					m_waitTime = 0;

					m_pSetting->SetSettingScene(m_setButton);
				}
				//�I��
				if (selectDecision == 10)
				{
					SetEnd(true);
				}
			}
			//B�{�^������������
			else if (m_xpad.Buttons[13] == 1)
			{
				SetEnd(true);
			}
		}
		else if (m_pSetting->GetSettingScene() == false)
		{
			m_waitTime++;
		}


		SelectBlend(7, 0, 1, 2);
		SelectBlend(8, 1, 0, 2);
		SelectBlend(9, 2, 1, 0);

	}
	//�ݒ���J��
	if (m_pSetting->GetSettingScene() == true)
	{
		m_pSetting->Update();
	}

	if (m_playTime >= m_totalAnimationTime && m_animation != -1)
	{
		m_playTime = 80.0f;
	}
	if (m_animation != -1)
	{
		MV1SetAttachAnimTime(m_playerHandle, m_animation, m_playTime);
	}

	m_pBgm->Update(m_pSetting->GetVolume());
	pse->Update(m_pSetting->GetVolume());

	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_cameraTarget);

	return shared_from_this();  //���g�̃|�C���^��Ԃ�
}

/// <summary>
/// �I�𒆂̏ꍇ
/// </summary>
/// <param name="select">�񋓌^</param>
/// <param name="now">�I�����Ă����</param>
/// <param name="other1">����ȊO�P</param>
/// <param name="other2">����ȊO�Q</param>
void SceneTitle::SelectBlend(int select, int now, int other1, int other2)
{
	if (pselect->NowSelect == select)
	{
		if (m_blend == false)
		{
			if (m_pal[now] < 256)
			{
				m_pal[now] += 2;
			}
			else
			{
				m_blend = true;
			}

		}
		if (m_blend == true)
		{
			if (m_pal[now] > 125)
			{
				m_pal[now] -= 2;
			}
			else
			{
				m_blend = false;
			}
		}


		m_pal[other1] = 255;
		m_pal[other2] = 255;
	}
}

/// <summary>
/// �`�揈��
/// </summary>
void SceneTitle::Draw()
{
	//pmap->Draw();
	m_pMap->Draw();
	cEffect.Draw();

	MV1SetPosition(m_playerHandle, m_pos);

	MV1DrawModel(m_playerHandle);

	//3D���f���̉�]�n���Z�b�g����
	MV1SetRotationXYZ(m_playerHandle, VGet(0.0f, 160.0f, 0.0f));


	DrawGraph(120, 0, m_backScene, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_pal[0]);
	DrawGraph(500, 350, m_start, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_pal[1]);
	DrawGraph(500, 500, m_setting, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_pal[2]);
	DrawGraph(500, 650, m_end, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//DrawGraph(500, 350, m_hand, true);
	DrawRotaGraph(620, cHandY, 1.0f, DX_PI_F - (DX_PI_F / 4), m_hand, true);
	DrawRotaGraph(1050, cHandY, 1.0f, DX_PI_F + (DX_PI_F / 4), m_hand, true, true);

	SetFontSize(35);

	DrawGraph(1150, 900, m_AButton, true);
	DrawFormatString(1210, 910, 0xffffff, "����");
	DrawGraph(1350, 900, m_BButton, true);
	DrawFormatString(1410, 910, 0xffffff, "�L�����Z��");

	SetFontSize(40);

	//�ݒ��ʂ�`��
	if (m_pSetting->GetSettingScene() == true)
	{
		m_pSetting->Draw();
	}

	m_pSetting->SettingDraw(m_pSetting->GetVolume());

	if (m_pSetting->GetSettingScene() == false)
	{
		pselect->Draw();
	}
}

/// <summary>
/// �I������
/// </summary>
void SceneTitle::End()
{
	//���������
	DeleteGraph(m_backScene);
	DeleteGraph(m_start);
	DeleteGraph(m_setting);
	DeleteGraph(m_end);
	DeleteGraph(m_hand);
	DeleteGraph(m_BButton);
	DeleteGraph(m_AButton);
	MV1DeleteModel(m_playerHandle);
	MV1DeleteModel(m_anim);
	m_pSetting->End();
	m_pBgm->End();
	pse->End();
	m_pMap->End(m_pPhysics, true);
	cEffect.End();

	handle.Clear();
}
