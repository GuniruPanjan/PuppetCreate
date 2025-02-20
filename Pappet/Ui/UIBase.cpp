#include "UIBase.h"

/// <summary>
/// コンストラクタ
/// </summary>
UIBase::UIBase() :
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
	m_messageUI(0)
{
	//画像読み込み
	//m_heelStone = MyLoadGraph("Data/UI/HeelStoneMini.png", 6, 6);
	//m_eqFist = MyLoadGraph("Data/UI/FistUi.png", 7, 7);
	//m_sword = MyLoadGraph("Data/UI/SwordUI.png", 3, 3);
	//m_eqSword = MyLoadGraph("Data/UI/SwordUI.png", 7, 7);
	//m_eqShield = MyLoadGraph("Data/UI/Shield.png", 7, 7);

	////装備関係
	//m_equipment = MyLoadGraph("Data/UI/装備画面UI.png", 1, 1);
	//m_itemBox = MyLoadGraph("Data/UI/アイテム画面UI.png", 1, 1);
	//m_selectEquipment = MyLoadGraph("Data/UI/装備選択画面UI.png", 1, 1);
	//m_blackSword = MyLoadGraph("Data/UI/黒い剣UI.png", 3, 3);
	//m_fist = MyLoadGraph("Data/UI/拳UI.png", 3, 3);
	//m_body = MyLoadGraph("Data/UI/裸体UI.png", 3, 3);
	//m_uglyShield = MyLoadGraph("Data/UI/醜い盾UI.png", 4, 4);
	//m_commonArmor = MyLoadGraph("Data/UI/平凡な鎧UI.png", 3, 3);

	////UI関係
	//m_rest = MyLoadGraph("Data/UI/四角の白い長方形の囲い.png", 1, 1);
	//m_dead = MyLoadGraph("Data/UI/YOUDIEDGraph.png", 1, 1);
	//m_selectUi = MyLoadGraph("Data/UI/選択UI.png", 2, 2);
	//m_levelUp = MyLoadGraph("Data/UI/レベルステータスUI.png", 1, 1);
	//m_backRightBar = MyLoadGraph("Data/UI/StatusBar右端.png", 2, 2);
	//m_backLeftBar = MyLoadGraph("Data/UI/StatusBar左端.png", 2, 2);
	//m_backCenterBar = MyLoadGraph("Data/UI/StatusBar中央.png", 2, 2);
	//m_backOutBar = MyLoadGraph("Data/UI/StatusBar外枠.png", 2, 2);
	//m_hpBar = MyLoadGraph("Data/UI/HPBarRed.png", 2, 2);
	//m_staminaBar = MyLoadGraph("Data/UI/StaminaBar.png", 2, 2);
	//m_coreBackBar = MyLoadGraph("Data/UI/CoreBar.png", 1, 1);
	//m_statusIcon = MyLoadGraph("Data/UI/StatusIcon.png", 1, 1);
	//m_equipmentFrame = MyLoadGraph("Data/UI/Frame.png", 1, 1);
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
	int handle = LoadGraph(FileName);  //画像のロード
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


