#include "UIBase.h"

/// <summary>
/// コンストラクタ
/// </summary>
UIBase::UIBase() :
	m_pItem(),
	m_heelStone(0),
	m_eqFist(0),
	m_sword(0),
	m_eqSword(0),
	m_eqShield(0),
	m_equipment(0),
	m_itemBox(0),
	m_selectEquipment(0),
	m_blackSword(0),
	m_fist(0),
	m_body(0),
	m_uglyShield(0),
	m_commonArmor(0),
	m_bat(0),
	m_woodShield(0),
	m_rest(0),
	m_dead(0),
	m_selectUi(0),
	m_levelUp(0),
	m_backRightBar(0),
	m_backLeftBar(0),
	m_backCenterBar(0),
	m_backOutBar(0),
	m_hpBar(0),
	m_staminaBar(0),
	m_coreBackBar(0),
	m_statusIcon(0),
	m_equipmentFrame(0),
	m_actionUI(0),
	m_itemTaking(0),
	m_yButton(0),
	m_bButton(0),
	m_aButton(0),
	m_xButton(0),
	m_lStick(0),
	m_rStick(0),
	m_rbButton(0),
	m_rtButton(0),
	m_lbButton(0),
	m_startButton(0),
	m_messageUI(0),
	m_tagetLock(0)
{
	
}

/// <summary>
/// デストラクタ
/// </summary>
UIBase::~UIBase()
{
	//メモリ解放
	DeleteGraph(m_heelStone);
	DeleteGraph(m_sword);
	DeleteGraph(m_eqFist);
	DeleteGraph(m_eqSword);
	DeleteGraph(m_eqShield);
	DeleteGraph(m_equipment);
	DeleteGraph(m_itemBox);
	DeleteGraph(m_selectEquipment);
	DeleteGraph(m_blackSword);
	DeleteGraph(m_fist);
	DeleteGraph(m_body);
	DeleteGraph(m_uglyShield);
	DeleteGraph(m_commonArmor);
	DeleteGraph(m_dead);
	DeleteGraph(m_rest);
	DeleteGraph(m_selectUi);
	DeleteGraph(m_levelUp);
	DeleteGraph(m_coreBackBar);
	DeleteGraph(m_statusIcon);
	DeleteGraph(m_equipmentFrame);
	DeleteGraph(m_hpBar);
	DeleteGraph(m_backOutBar);
	DeleteGraph(m_yButton);
	DeleteGraph(m_bButton);
	DeleteGraph(m_lStick);
	DeleteGraph(m_messageUI);
	DeleteGraph(m_tagetLock);
}

/// <summary>
/// 画像のサイズを変えてロードする関数
/// </summary>
/// <param name="FileName">画像のFileName</param>
/// <param name="XSize">画像の横幅を割る数</param>
/// <param name="YSize">画像の縦幅を割る数</param>
/// <returns>サイズを変えた画像を返す</returns>
int UIBase::MyLoadGraph(const char* FileName, int XSize, int YSize)
{
	SetUseASyncLoadFlag(TRUE);     //非同期読み込み有効
	int handle = LoadGraph(FileName);  //画像のロード
	SetUseASyncLoadFlag(FALSE);    //以降非同期読み込み判定してはいけないため解除

	if (handle != -1)  //画像のロードに成功した場合
	{
		int SizeX, SizeY;  //画像サイズを格納するための変数を用意

		GetGraphSize(handle, &SizeX, &SizeY);  //ロードした画像のサイズ取得

		int NowScreen = GetDrawScreen();    //現在の描画対象画面を一時保存

		SizeX /= XSize;
		SizeY /= YSize;

		int miniHandle = MakeScreen(SizeX, SizeY, TRUE);  //サイズ変更後のグラフィックハンドルを作成
		if (miniHandle == -1)   //ハンドルの作成に失敗した場合
		{
			DeleteGraph(handle);   //メモリリークしないように、本来のグラフィックハンドルを削除
		}
		SetDrawScreen(miniHandle);   //描画対象画面を先ほど作ったサイズのグラフィックハンドルに変更
		DrawExtendGraph(0, 0, SizeX, SizeY, handle, TRUE);   //ロードした画像を描画
		DeleteGraph(handle);   //不要になった本来のサイズのグラフィックハンドルを削除
		SetDrawScreen(NowScreen);  //現在の描画対象画面を元の画面に戻す

		return miniHandle;  //縮めた画像のグラフィックハンドルを返す
	}
	else   //画像のロードに失敗した場合
	{
		return -1;   //エラー発生
	}
}


