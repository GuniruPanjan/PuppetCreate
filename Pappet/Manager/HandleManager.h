#pragma once
#include<string>
#include<list>

struct Handle
{
	std::string path;
	int handle;
	bool isEternal;
};


/// <summary>
/// 読み込むモデルを管理するクラス(シングルトン)
/// </summary>
class HandleManager
{

private:
	//コンストラクタはprivateに置く
	HandleManager();
	//デストラクタ
	virtual ~HandleManager();

public:
	//コピーコンストラクタを禁止する
	HandleManager(const HandleManager&) = delete;
	HandleManager& operator=(const HandleManager&) = delete;
	HandleManager(HandleManager&&) = delete;
	HandleManager& operator=(const HandleManager&&) = delete;

	//HandleManagerはGetInstance()を通した参照からしか利用できない
	static HandleManager& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new HandleManager;
		}

		return *m_instance;
	}

	//メモリリークを防ぐ
	static void Delete()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:
	//指定したパスのハンドルを取得する
	int GetModelHandle(std::string path, bool isEternal = false);
	//パスのリストを消去する
	void Clear();

private:
	//Singletonのポインタがプログラム起動時に一つ作られるようにする
	static HandleManager* m_instance;

private:
	std::list<Handle> m_handles;

};
