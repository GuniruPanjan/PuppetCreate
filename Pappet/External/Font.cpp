#include "Font.h"
#include "DxLib.h"

Font::Font():
	m_fontHandle(0)
{
}

Font::~Font()
{
}

void Font::FontInit(int size)
{
	m_fontHandle = AddDxArchiveFontResource("Data/Font/アプリ明朝.otf", size);
}

int Font::AddDxArchiveFontResource(LPCSTR fontpath, int size)
{
	if (AddFontResourceEx(fontpath, FR_PRIVATE, NULL) > 0)
	{
	}
	else
	{
		//フォント読み込みエラー処理
		MessageBox(NULL, "フォント読み込みエラー", "", MB_OK);
	}

	return CreateFontToHandle("アプリ明朝", size, -1, DX_FONTTYPE_NORMAL);
}
