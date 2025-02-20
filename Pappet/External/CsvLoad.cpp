#include "CsvLoad.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <cassert>

CsvLoad* CsvLoad::m_instance = nullptr;

namespace
{
	//csvデータの,で文字列を分割する
	std::vector<std::string> Split(std::string& input, char delimiter)
	{
		std::istringstream stream(input);
		std::string field;
		std::vector<std::string> result;
		while (getline(stream, field, delimiter))
		{
			result.push_back(field);
		}
		return result;
	}

	const std::string cFrontPathName = "Data/Csv/";
	const std::string cBackPathName = ".csv";
}

/// <summary>
/// ステータス情報ロード
/// </summary>
/// <param name="data">ステータス情報</param>
/// <param name="charaName">キャラクターの名前</param>
void CsvLoad::StatusLoad(CharacterBase::Status& data, const char* charaName)
{
	//一時保存用string
	std::string strBuf;
	//カンマ分け一時保存用string
	std::vector<std::string> strConmaBuf;

	//ファイル読み込み
	std::ifstream cfs("Data/Csv/status.csv");
	if (!cfs)
	{
		assert(false);
		return;
	}

	//情報を取得できたかどうかのフラグ
	bool isGet = false;

	//対応情報が入っているため無視する
	std::getline(cfs, strBuf);

	while (getline(cfs, strBuf))
	{
		//取得した文字列をカンマ区切りの配列(情報群)にする
		strConmaBuf = Split(strBuf, ',');

		//0 : キャラクター名
		//1 : 体力
		//2 : スタミナ
		//3 : 攻撃力
		//4 : 攻撃力1
		//5 : 攻撃力2
		//4 : 筋力
		//5 : 技量
		//6 : 防御力
		//7 : 速度
		//8 : コア

		//指定したキャラクター名と一致するデータがあれば情報を取得する
		if (strConmaBuf[DataLoad::CsvStatus::name] == charaName)
		{
			isGet = true;
			data.s_hp = std::stoi(strConmaBuf[DataLoad::CsvStatus::hp]);
			data.s_stamina = std::stoi(strConmaBuf[DataLoad::CsvStatus::stamina]);
			data.s_attack = std::stoi(strConmaBuf[DataLoad::CsvStatus::attack]);
			data.s_attack1 = std::stoi(strConmaBuf[DataLoad::CsvStatus::attack1]);
			data.s_attack2 = std::stoi(strConmaBuf[DataLoad::CsvStatus::attack2]);
			data.s_muscle = std::stoi(strConmaBuf[DataLoad::CsvStatus::muscle]);
			data.s_skill = std::stoi(strConmaBuf[DataLoad::CsvStatus::skill]);
			data.s_defense = std::stoi(strConmaBuf[DataLoad::CsvStatus::defense]);
			data.s_speed = std::stoi(strConmaBuf[DataLoad::CsvStatus::speed]);
			data.s_core = std::stoi(strConmaBuf[DataLoad::CsvStatus::core]);
		}
	}

	if (!isGet)
	{
		assert(0 && "指定したキャラクター名の情報を取得できなかった");
	}

	return;
}

/// <summary>
/// ステージの情報ロード
/// </summary>
/// <param name="stageName">ステージの名前</param>
/// <param name="pGenerateInfo">キャラクターの名前</param>
void CsvLoad::EnemyDataLoad(const char* stageName, std::list<std::shared_ptr<EnemyManager::EnemyGenerateInfo>>& pGenerateInfo)
{
	//一時保存用string
	std::string strBuf;
	//カンマ分け一時保存用string
	std::vector<std::string> strConmaBuf;

	std::string csvName = cFrontPathName + stageName + cBackPathName;

	//ファイル読み込み
	std::ifstream ifs(csvName);
	if (!ifs)
	{
		assert(false);
		return;
	}

	//最初は対応表情報が入っているだけなので無視する
	std::getline(ifs, strBuf);

	while (getline(ifs, strBuf))
	{
		//所得した文字列をカンマ区切りの配列(情報群)にする
		strConmaBuf = Split(strBuf, ',');

		//0 : 生成する敵のID
		//1 : 生成されるマップ
		//2 : X座標
		//3 : Y座標
		//4 : Z座標

		std::shared_ptr<EnemyManager::EnemyGenerateInfo> add = std::make_shared<EnemyManager::EnemyGenerateInfo>();
		add->enemyName = strConmaBuf[DataLoad::CsvEnemyStage::ID];
		add->mapNumber = std::stoi(strConmaBuf[DataLoad::CsvEnemyStage::map]);
		add->posX = std::stoi(strConmaBuf[DataLoad::CsvEnemyStage::x]);
		add->posY = std::stoi(strConmaBuf[DataLoad::CsvEnemyStage::y]);
		add->posZ = std::stoi(strConmaBuf[DataLoad::CsvEnemyStage::z]);

		pGenerateInfo.emplace_back(add);
	}

	return;
}

/// <summary>
/// アニメーション情報ロード
/// </summary>
/// <param name="charaName">読み込みたいキャラクター名</param>
/// <param name="anim">格納するアニメーション情報</param>
void CsvLoad::AnimDataLoad(std::string charaName, std::map<std::string, int>& anim)
{
	//一時保存用string
	std::string strBuf;
	//カンマ分け一時保存用string
	std::vector<std::string> strConmaBuf;

	//ファイル読み込み
	std::ifstream ifs("Data/Csv/animation.csv");
	if (!ifs)
	{
		assert(false);
		return;
	}

	//情報を取得できたかどうかのフラグ
	bool isGet = false;

	//アニメーション名保存用string
	std::vector<std::string> animationNameBuf;

	//アニメーションの名前を保存する
	std::getline(ifs, strBuf);
	animationNameBuf = Split(strBuf, ',');

	while (std::getline(ifs, strBuf))
	{
		//前のループですでに取得できていたらループを終わる
		if (isGet)
		{
			break;
		}

		//取得した文字列をカンマ区切りの配列(情報群)にする
		strConmaBuf = Split(strBuf, ',');

		//読み込みたいキャラクター名と読み込んだキャラクター名が同じなら
		if (strConmaBuf[0] == charaName)
		{
			//アニメーション情報を格納していく
			isGet = true;
			for (int i = 1; i < strConmaBuf.size(); i++)
			{
				//アニメーションIdxが-1以下だった場合存在しないため追加しない
				if (std::stoi(strConmaBuf[i]) != -1)
				{
					//ここがバグ
					anim[animationNameBuf[i]] = std::stoi(strConmaBuf[i]);
				}
			}
		}
	}

	if (!isGet)
	{
		assert(0 && "指定したキャラクター名のアニメーション情報を取得できない");
	}

	return;
}

/// <summary>
/// アイテム情報ロード
/// </summary>
/// <param name="stageName">ステージの名前</param>
/// <param name="pItemInfo"></param>
void CsvLoad::ItemDataLoad(const char* stageName, std::list<std::shared_ptr<ItemManager::ItemGenerateInfo>>& pGenerateInfo)
{
	//一時保存用string
	std::string strBuf;
	//カンマ分け一時保存用string
	std::vector<std::string> strConmaBuf;

	std::string csvName = cFrontPathName + stageName + "Item" + cBackPathName;

	//ファイル読み込み
	std::ifstream ifs(csvName);
	if (!ifs)
	{
		assert(false);
		return;
	}

	//最初は対応表情報が入っているだけなので無視する
	std::getline(ifs, strBuf);

	while (getline(ifs, strBuf))
	{
		//所得した文字列をカンマ区切りの配列(情報群)にする
		strConmaBuf = Split(strBuf, ',');

		//0 : 生成するアイテムのID
		//1 : 生成されるマップ
		//2 : X座標
		//3 : Y座標
		//4 : Z座標
		//5 : 小型コア
		//6 : 中型コア
		//7 : ゴミ
		//8 : 黒い剣
		//9 : 歪んだ盾
		//10 : 普通の鎧

		std::shared_ptr<ItemManager::ItemGenerateInfo> add = std::make_shared<ItemManager::ItemGenerateInfo>();
		add->itemName = strConmaBuf[DataLoad::CsvItemStage::ItemID];
		add->mapNumber = std::stoi(strConmaBuf[DataLoad::CsvItemStage::Itemmap]);
		add->posX = std::stoi(strConmaBuf[DataLoad::CsvItemStage::Itemx]);
		add->posY = std::stoi(strConmaBuf[DataLoad::CsvItemStage::Itemy]);
		add->posZ = std::stoi(strConmaBuf[DataLoad::CsvItemStage::Itemz]);
		add->SmallCore = std::stoi(strConmaBuf[DataLoad::CsvItemStage::ItemSmallCore]);
		add->MediumCore = std::stoi(strConmaBuf[DataLoad::CsvItemStage::ItemMediumCore]);
		add->Rubbish = std::stoi(strConmaBuf[DataLoad::CsvItemStage::ItemRubbish]);
		add->BlackSword = std::stoi(strConmaBuf[DataLoad::CsvItemStage::ItemBlackSword]);
		add->Distorted = std::stoi(strConmaBuf[DataLoad::CsvItemStage::ItemDistorted]);
		add->ArmorNormal = std::stoi(strConmaBuf[DataLoad::CsvItemStage::ItemArmorNormal]);

		pGenerateInfo.emplace_back(add);
	}

	return;
}

/// <summary>
/// メッセージの情報ロード
/// </summary>
/// <param name="stageName"></param>
/// <param name="pGenerateInfo"></param>
void CsvLoad::MessageDataLoad(const char* stageName, std::list<std::shared_ptr<MessageManager::MessageGenerateInfo>>& pGenerateInfo)
{
	//一時保存用string
	std::string strBuf;
	//カンマ分け一時保存用string
	std::vector<std::string> strConmaBuf;

	std::string csvName = cFrontPathName + "Message" + cBackPathName;

	//ファイル読み込み
	std::ifstream ifs(csvName);
	if (!ifs)
	{
		assert(false);
		return;
	}

	//最初は対応表情報が入っているだけなので無視する
	std::getline(ifs, strBuf);

	while (getline(ifs, strBuf))
	{
		//所得した文字列をカンマ区切りの配列(情報群)にする
		strConmaBuf = Split(strBuf, ',');

		//0 : 生成されるマップ
		//1 : X座標
		//2 : Y座標
		//3 : Z座標
		//4 : 公式かの判断
		//5 : 一つ目の言葉
		//6 : 二つ目の言葉
		//7 : 三つ目の言葉

		std::shared_ptr<MessageManager::MessageGenerateInfo> add = std::make_shared<MessageManager::MessageGenerateInfo>();
		add->mapNumber = std::stoi(strConmaBuf[DataLoad::CsvMessageStage::messageMap]);
		add->posx = std::stoi(strConmaBuf[DataLoad::CsvMessageStage::messageX]);
		add->posy = std::stoi(strConmaBuf[DataLoad::CsvMessageStage::messageY]);
		add->posz = std::stoi(strConmaBuf[DataLoad::CsvMessageStage::messageZ]);
		add->official = std::stoi(strConmaBuf[DataLoad::CsvMessageStage::official]);
		add->one = std::stoi(strConmaBuf[DataLoad::CsvMessageStage::one]);
		add->two = std::stoi(strConmaBuf[DataLoad::CsvMessageStage::two]);
		add->three = std::stoi(strConmaBuf[DataLoad::CsvMessageStage::three]);

		pGenerateInfo.emplace_back(add);
	}

	return;
}

