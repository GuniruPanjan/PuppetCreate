#pragma once


/// <summary>
///選択肢を選ぶためのマネージャー
/// </summary>
class SelectManager
{
public:
	//コンストラクタ
	SelectManager();
	//デストラクタ
	virtual ~SelectManager();

	//選択するための列挙型
	typedef enum
	{

		One,       //選択0
		Two,       //選択1
		Three,     //選択2
		Four,      //選択3
		Five,      //選択4
		Six,       //選択5
		Seven,     //選択6
		Eight,     //選択7
		Nine,      //選択8
		Ten,       //選択9
		SELECT_SIZE

	}SelectName;

	int NowSelect;    //選んでいるもの

	//メニューなどの選択用関数
	void Menu_Update(int stick, bool& one, int button, int& decision, int now);

	//アイテムなどの選択用関数
	void Item_Update();

	//描画処理
	void Draw();
private:
	bool m_menuOne;    //一回だけ行う
};

