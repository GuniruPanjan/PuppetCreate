#pragma once
#include "EffekseerForDXLib.h"
#include <string>
#include <memory>
#include <map>
#include <vector>
#include "DxLib.h"

/// <summary>
/// エフェクトを管理するクラス
/// </summary>
class EffectManager
{
private:
	//エフェクト構造体
	struct Effectstruct
	{
		int handle;
		float x, y, z;
		int frame;
		bool isExist;
	};

	//エフェクト生成構造体
	struct EffectEmitter
	{
		int emitterHandle = -1;
		const char* effectPath = "";
		std::vector<Effectstruct> effects;
		int endFrame = 0;
	};

private:
	//シングルトンパターン
	EffectManager() {};

public:
	virtual ~EffectManager();

	void Init();   //初期化処理

	//コピーコンストラクタから実態の生成ができてしまうため
	//コピーコンストラクタを禁止する
	EffectManager(const EffectManager&) = delete;
	EffectManager& operator=(const EffectManager&) = delete;
	EffectManager(EffectManager&&) = delete;
	EffectManager& operator=(const EffectManager&&) = delete;

	/// <summary>
	/// EffectはGetInstance()を通した参照からしか利用できない
	/// </summary>
	/// <returns></returns>
	static EffectManager& GetInstance()
	{
		if (!m_instance)
		{
			m_instance = new EffectManager;
		}

		return *m_instance;
	}

	//メモリリークを防ぐ
	static void Destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:
	//エフェクトをロードする
	void EffectLoad(std::string name, const char* path, int endFrame, float scale = 1.0f);

	//更新
	void Update();

	//描画
	void Draw();

	//終了処理
	void End();

	//エフェクトを再生させながらポジションを更新する
	void UpdateEffectPosition(std::string name, VECTOR pos);
	//エフェクトを再生させながら角度を更新する
	void UpdateEffectRotation(std::string name, VECTOR rotation);
	//エフェクトを生成する
	void EffectCreate(std::string name, VECTOR pos);
	//エフェクトを削除する
	void RemoveEffect(std::string name, VECTOR pos);

private:
	//Singletonのポインタがプログラム起動時に一つ作られるようにする
	static EffectManager* m_instance;

private:
	//エフェクトのハンドル
	std::map<std::string, std::shared_ptr<EffectEmitter>> m_effect;
};


