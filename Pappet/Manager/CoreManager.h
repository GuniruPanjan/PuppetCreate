#pragma once

class CoreManager
{
public:
	CoreManager();
	virtual ~CoreManager();

	void Init();
	void Update(int core);
	void DeadUpdate();
	void End();

	//コア計算
	int Core(int core) { return m_core += core; }
	//コア入手
	int GetCore() { return m_core; }
	//コアを使う場合
	int SetCore(int core) { return m_core = core; }

public:
	//レベル上げに必要なコア数
	int NeedCore(int level);

private:
	int m_core;     //プレイヤーが入手したコアを全て入れる
};

