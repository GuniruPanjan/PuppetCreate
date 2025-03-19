#include "SelectManager.h"
#include "DxLib.h"

/// <summary>
/// コンストラクタ
/// </summary>
SelectManager::SelectManager():
	m_menuOne(false),
	NowSelect()
{
}

/// <summary>
///デストラクタ
/// </summary>
SelectManager::~SelectManager()
{
}

/// <summary>
/// メニューなどのセレクト
/// </summary>
/// <param name="stick">スティック入力</param>
/// <param name="one">一回だけ行う</param>
/// <param name="button">ボタン入力</param>
/// <param name="decision">決定したときに返す変数</param>
/// <param name="now">選択の数</param>
void SelectManager::Menu_Update(int stick, bool& one, int button, int& decision, int now)
{
	//一度だけ初期化
	if (m_menuOne == false)
	{
		NowSelect = now;   //現在の選択肢
		decision = 0;

		m_menuOne = true;
	}

	//上選択or左選択
	if (stick > 0 && one == false)
	{
		//NowSelectがNowだった場合
		if (NowSelect == now)
		{
			NowSelect = (NowSelect = Ten);    //選択肢を一番下にする
		}
		//NowSelectがNow以上だった場合
		else if (NowSelect > now)
		{
			NowSelect = (NowSelect - 1);   //選択肢を一つ上げる
		}
		
		
		one = true;
	}

	//下選択or右選択
	if (stick < 0 && one == false)
	{
		
		//NowSelectがTenだった場合
		if (NowSelect == Ten)
		{
			NowSelect = (NowSelect = now);    //選択肢を一番上にする
		}
		//NowSelectがTen以下だった場合
		else if (NowSelect < SELECT_SIZE)
		{
			NowSelect = (NowSelect + 1);   //選択肢を一つ下げる
		}

		one = true;
	}

	//ボタンが押されたら
	if (button == 1)
	{
		switch (NowSelect)     //現在選択中の状態によって処理を分岐
		{
		case One:
			decision = 1;
			m_menuOne = false;
			break;
		case Two:
			decision = 2;
			m_menuOne = false;
			break;
		case Three:
			decision = 3;
			m_menuOne = false;
			break;
		case Four:
			decision = 4;
			m_menuOne = false;
			break;
		case Five:
			decision = 5;
			m_menuOne = false;
			break;
		case Six:
			decision = 6;
			m_menuOne = false;
			break;
		case Seven:
			decision = 7;
			m_menuOne = false;
			break;
		case Eight:
			decision = 8;
			m_menuOne = false;
			break;
		case Nine:
			decision = 9;
			m_menuOne = false;
			break;
		case Ten:
			decision = 10;
			m_menuOne = false;
			break;
		}
	}
	
}

void SelectManager::Item_Update()
{
}

/// <summary>
/// 描画処理
/// </summary>
void SelectManager::Draw()
{
#if _DEBUG
	DrawFormatString(100, 100, 0xffffff, "%d", NowSelect);
#endif
}
