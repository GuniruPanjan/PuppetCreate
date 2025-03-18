#pragma once

/// <summary>
/// フェードアウト・フェードインを管理するクラス
/// </summary>
class FadeManager
{
public:
	FadeManager() :
		m_fadeOut(false),
		m_fadeIn(false),
		m_fade(0),
		m_fadeHandle(-1)
	{

	}
	virtual ~FadeManager();

	//初期化処理
	void Init();

	//フェードアウト関数
	void FadeOut(int fade);
	//フェードイン関数
	void FadeIn(int fade);
	//描画処理
	void Draw();
	//終了処理
	void End();

	int SetFade(int set) { return m_fade = set; }

	bool GetOut() { return m_fadeOut; }
	bool SetOut(bool set) { return m_fadeOut = set; }
	bool GetIn() { return m_fadeIn; }
	bool SetIn(bool set) { return m_fadeIn = set; }

private:
	bool m_fadeOut;    //フェードアウト完了
	bool m_fadeIn;     //フェードイン完了

	int m_fade;        //フェード変数
	int m_fadeHandle;  //フェードハンドル
};






