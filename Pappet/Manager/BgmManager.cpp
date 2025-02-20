#include "BgmManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
BgmManager::BgmManager():
	m_titleBGM(0),
	m_gameBGM(0),
	m_bossBGM(0),
	m_clearBGM(0),
	m_volumePal(0)
{
	
}

/// <summary>
/// デストラクタ
/// </summary>
BgmManager::~BgmManager()
{
	//メモリ解放
	//BGM
	DeleteSoundMem(m_titleBGM);
	DeleteSoundMem(m_gameBGM);
	DeleteSoundMem(m_bossBGM);
	DeleteSoundMem(m_clearBGM);

	
}

/// <summary>
/// タイトルBGMの初期化処理
/// </summary>
void BgmManager::TitleInit()
{
	//BGM読み込み
	m_titleBGM = LoadSoundMem("Data/BGM/TitleBGM.mp3");
}

/// <summary>
/// 休息地点のBGMの初期化処理
/// </summary>
void BgmManager::GameRestInit()
{
	//BGM読み込み
	m_gameBGM = LoadSoundMem("Data/BGM/RestBGM.mp3");
	m_bossBGM = LoadSoundMem("Data/BGM/RestBGM.mp3");
}

/// <summary>
/// ゲームBGMの初期化処理
/// </summary>
void BgmManager::GameOneInit()
{
	//BGM読み込み
	m_gameBGM = LoadSoundMem("Data/BGM/GameBGM.mp3");
	m_bossBGM = LoadSoundMem("Data/BGM/BossBGM.mp3");
}

/// <summary>
/// クリアBGMの初期化処理
/// </summary>
void BgmManager::ClearInit()
{
	//BGM読み込み
	m_clearBGM = LoadSoundMem("Data/BGM/ClearBGM.mp3");
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="volume">音量</param>
void BgmManager::Update(int volume)
{
	m_volumePal = volume;

	//音量を変える
	//BGM
	ChangeVolumeSoundMem(m_volumePal, m_titleBGM);
	ChangeVolumeSoundMem(m_volumePal, m_gameBGM);
	ChangeVolumeSoundMem(m_volumePal, m_bossBGM);
	ChangeVolumeSoundMem(m_volumePal, m_clearBGM);

}

/// <summary>
/// タイトルBGM更新処理
/// </summary>
void BgmManager::TitleBGM()
{
	PlaySoundMem(m_titleBGM, DX_PLAYTYPE_LOOP, true);
}

/// <summary>
/// ゲームBGM更新処理
/// </summary>
void BgmManager::GameBGM()
{
	PlaySoundMem(m_gameBGM, DX_PLAYTYPE_LOOP, true);
}

/// <summary>
/// ボスBGM更新処理
/// </summary>
void BgmManager::BossBGM()
{
	StopSoundMem(m_gameBGM);   //サウンドを止める

	PlaySoundMem(m_bossBGM, DX_PLAYTYPE_LOOP, true);
}

/// <summary>
/// ボスBGMストップ処理
/// </summary>
void BgmManager::BossStopBGM()
{
	StopSoundMem(m_bossBGM);
}

/// <summary>
/// クリアBGM更新処理
/// </summary>
void BgmManager::ClearBGM()
{
	PlaySoundMem(m_clearBGM, DX_PLAYTYPE_LOOP, true);
}

/// <summary>
/// ゲーム内BGM終了処理
/// </summary>
void BgmManager::GameEnd()
{
	DeleteSoundMem(m_gameBGM);
	DeleteSoundMem(m_bossBGM);
}

/// <summary>
/// 終了処理
/// </summary>
void BgmManager::End()
{
	//メモリ解放
	//BGM
	DeleteSoundMem(m_titleBGM);
	DeleteSoundMem(m_gameBGM);
	DeleteSoundMem(m_bossBGM);
	DeleteSoundMem(m_clearBGM);
}
