#pragma once
#include <memory>
#include <map>
#include <string>
#include <List>
#include "Library/MyLibrary.h"
#include "Ui/Message.h"

class GameManager;
class Player;

/// <summary>
/// メッセージを管理するクラス
/// </summary>
class MessageManager
{
public:
	//メッセージの生成情報
	struct MessageGenerateInfo
	{
		int mapNumber;      //マップの番号
		int posx;           //x座標
		int posy;           //y座標
		int posz;           //z座標

		bool official;      //公式メッセージか

		int one;            //一つ目の言葉
		int two;            //二つ目の言葉
		int three;          //三つ目の言葉

		bool isCreated = false;     //生成されたかどうか    
	};

public:
	//コンストラクタ
	MessageManager();
	//デストラクタ
	virtual ~MessageManager();
	//初期化処理
	void Init(const char* stageName);
	//ゲーム内での初期化処理
	void GameInit(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager);
	//更新処理
	void Update(std::shared_ptr<MyLibrary::Physics> physics, GameManager* gameManager, Player& player);

	//描画処理
	void Draw();

	//終了処理
	void End();

	//メッセージを読めるか返す
	bool GetMessagePick() { return m_messagePick; }
	//メッセージを表示中か返す
	bool GetDraw();

private:
	//メッセージ生成
	void CreateMessage(float posx, float posy, float posz, bool official, int one, int two, int three, std::shared_ptr<MyLibrary::Physics> physics);
private:
	//メッセージの管理
	std::shared_ptr<Message> m_pMessage;
	//メッセージの生成情報
	std::list<std::shared_ptr<MessageGenerateInfo>> m_pGenerateInfo;

	//ステージ名
	const char* m_stageName;

	//ステージ毎のメッセージの生成数
	std::unordered_map<int, int> m_MessageGenerationCountPerOneMap;

	bool m_messagePick;         //メッセージを読む
};

