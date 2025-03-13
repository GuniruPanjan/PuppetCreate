#pragma once
#include "Character/CharacterBase.h"
#include "Manager/EnemyManager.h"
#include "Manager/ItemManager.h"
#include "Manager/MessageManager.h"
#include <string>
#include <memory>

namespace DataLoad
{
	//ステータス情報列挙型
	enum CsvStatus : int
	{
		name,      //キャラクター名
		hp,        //体力
		stamina,   //スタミナ
		attack,    //攻撃力
		attack1,   //攻撃力2
		attack2,   //攻撃力3
		muscle,    //筋力
		skill,     //技量
		defense,   //防御力
		speed,     //速度
		core,      //ドロップコア
	};

	//敵の生成場所の列挙型
	enum CsvEnemyStage : int
	{
		ID,         //敵ID
		map,        //敵の出現マップ
		x,          //敵のX座標
		y,          //敵のY座標
		z,          //敵のZ座標
	};

	//アイテムの生成場所の列挙型
	enum CsvItemStage : int
	{
		ItemID,         //アイテムのID
		Itemmap,        //アイテムの出現マップ
		Itemx,          //アイテムのX座標
		Itemy,          //アイテムのY座標
		Itemz,          //アイテムのZ座標

		//アイテム識別
		ItemSmallCore,   //小型コア
		ItemMediumCore,  //中型コア
		ItemRubbish,     //ゴミ
		ItemBlackSword,  //黒い剣
		ItemDistorted,   //歪んだ盾
		ItemArmorNormal, //普通の鎧
		ItemBat,         //木のバット
		ItemWoodShield,  //木の盾
	};

	//メッセージの生成場所の列挙型
	enum CsvMessageStage : int
	{
		messageMap,          //出現マップ
		messageX,            //x座標
		messageY,            //y座標
		messageZ,            //z座標
		official,     //公式かの判断
		one,          //最初の言葉
		two,          //二つ目の言葉
		three,        //三つ目の言葉
	};
}

/// <summary>
/// Csvを読み込むクラス
/// </summary>
class CsvLoad
{
private:
	//シングルトンにするためコンストラクタはprivateに置く
	CsvLoad() {};

public:
	virtual ~CsvLoad() {};

	//コピーコンストラクタを禁止する
	CsvLoad(const CsvLoad&) = delete;
	CsvLoad& operator = (const CsvLoad&) = delete;
	CsvLoad(CsvLoad&&) = delete;
	CsvLoad& operator = (const CsvLoad&&) = delete;

	/// <summary>
	/// GetInstance()を通した参照からしか利用できない
	/// </summary>
	/// <returns></returns>
	static CsvLoad& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new CsvLoad;
		}

		return *m_instance;
	}

	//メモリリークを防ぐため
	static void Destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:
	//ステータス情報ロード
	void StatusLoad(CharacterBase::Status& data, const char* charaName);

	//敵生成情報ロード
	void EnemyDataLoad(std::list<std::shared_ptr<EnemyManager::EnemyGenerateInfo>>& pGenerateInfo);

	//アニメーション情報ロード
	void AnimDataLoad(std::string charaName, std::map<std::string, int>& anim);

	//アイテム生成情報ロード
	void ItemDataLoad(std::list<std::shared_ptr<ItemManager::ItemGenerateInfo>>& pGenerateInfo);

	//メッセージ生成情報ロード
	void MessageDataLoad(std::list<std::shared_ptr<MessageManager::MessageGenerateInfo>>& pGenerateInfo);

private:
	//Singletonのポインタがプログラム起動時に一つ作られるようにする
	static CsvLoad* m_instance;
};



