#pragma once
#include "DxLib.h"

class Font
{
public:
	Font();
	~Font();

	void FontInit(int size);
	int AddDxArchiveFontResource(LPCSTR fontpath, int size);

public:
	//フォントハンドルを返す
	int GetHandle() { return m_fontHandle; }

private:
	int m_fontHandle;
};

