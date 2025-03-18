#include "FadeManager.h"
#include "DxLib.h"

namespace
{
	//フェードアウト判定
	bool cOut = false;
	//フェードイン判定
	bool cIn = false;
}

FadeManager::~FadeManager()
{
	DeleteGraph(m_fadeHandle);
}

/// <summary>
/// 初期化処理
/// </summary>
void FadeManager::Init()
{
	m_fadeOut = false;
	m_fadeIn = false;

	m_fadeHandle = LoadGraph("Data/SceneBack/BlackMini.png");
}

/// <summary>
/// フェードアウト関数
/// </summary>
void FadeManager::FadeOut(int fade)
{
	//一回だけ実行
	if (!cIn)
	{
		m_fade = 0;

		cIn = true;
		cOut = false;
	}

	if (m_fade < 255)
	{
		m_fade += fade;
	}
	else
	{
		m_fadeOut = true;
	}
}

/// <summary>
/// フェードイン関数
/// </summary>
void FadeManager::FadeIn(int fade)
{
	//一回だけ実行
	if (!cOut)
	{
		m_fade = 255;

		cIn = false;
		cOut = true;
	}

	if (m_fade > 0)
	{
		m_fade -= fade;
	}
	else
	{
		m_fadeIn = true;
	}
}

/// <summary>
/// 描画処理
/// </summary>
void FadeManager::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fade);
	DrawGraph(0, 0, m_fadeHandle, false);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

/// <summary>
/// 終了処理
/// </summary>
void FadeManager::End()
{
	DeleteGraph(m_fadeHandle);
}



