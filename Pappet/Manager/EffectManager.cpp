#include "EffectManager.h"
#include <cassert>

EffectManager* EffectManager::m_instance = nullptr;

/// <summary>
/// デストラクタ
/// </summary>
EffectManager::~EffectManager()
{
	//同じパスのエフェクトがロードされていないか確認する
	for (auto& effect : m_effect)
	{
		DeleteEffekseerEffect(effect.second->emitterHandle);
	}
}

/// <summary>
/// 初期化処理
/// </summary>
void EffectManager::Init()
{
	//回復エフェクト
	EffectLoad("Heel", "Data/Effect/AnotherEffect/Sylph13.efkefc", 160, 20.0f);
	//アイテムエフェクト
	EffectLoad("Item", "Data/Effect/Item.efkefc", 60, 5.0f);
	//休息エフェクト
	EffectLoad("Rest", "Data/Effect/Benediction.efkefc", 210, 10.0f);
	//熊の必殺エフェクト
	EffectLoad("BearLance", "Data/Effect/BloodLance.efkefc", 350, 10.0f);
	//煙エフェクト
	EffectLoad("Smoke", "Data/Effect/Smoke.efkefc", 50, 30.0f);
	//歪みエフェクト
	EffectLoad("Distortion", "Data/Effect/effect2.efkefc", 120, 10.0f);
	//メッセージエフェクト
	EffectLoad("Message", "Data/Effect/message.efkefc", 50, 10.0f);
	//Hitエフェクト
	EffectLoad("Hit", "Data/Effect/HitEffect.efkefc", 30, 15.0f);
}

/// <summary>
/// エフェクトロード
/// </summary>
/// <param name="name">エフェクトの名前</param>
/// <param name="path">ロードするためのパス</param>
/// <param name="endFrame">終了フレーム</param>
/// <param name="scale">拡大率</param>
void EffectManager::EffectLoad(std::string name, const char* path, int endFrame, float scale)
{
	//同じパスのエフェクトがロードされていないか確認する
	for (auto& effect : m_effect)
	{
		if (effect.second->effectPath == path)
		{
			return;
		}
	}

	//ここまで来たらロードする
	std::shared_ptr<EffectEmitter> add = std::make_shared<EffectEmitter>();
	add->emitterHandle = LoadEffekseerEffect(path, scale);
	assert(add->emitterHandle != -1 && "エフェクトロード失敗");
	add->endFrame = endFrame;

	m_effect[name] = add;
}

/// <summary>
/// 更新
/// </summary>
void EffectManager::Update()
{

	Effekseer_Sync3DSetting();


	UpdateEffekseer3D();

	//エフェクトの更新
	for (auto& effects : m_effect)
	{
		for (auto& ef : effects.second->effects)
		{
			if (ef.frame > effects.second->endFrame)
			{
				StopEffekseer3DEffect(ef.handle);
				ef.isExist = false;
			}

			ef.frame++;
		}

		//isExistがfalseのオブジェクトを削除
		auto it = std::remove_if(effects.second->effects.begin(), effects.second->effects.end(), [](auto& v)
			{
				return v.isExist = false;
			});
		effects.second->effects.erase(it, effects.second->effects.end());
	}
}

/// <summary>
/// 描画
/// </summary>
void EffectManager::Draw()
{
	DrawEffekseer3D();
}

/// <summary>
/// 終了処理
/// </summary>
void EffectManager::End()
{
	for (auto& effect : m_effect)
	{
		DeleteEffekseerEffect(effect.second->emitterHandle);
	}
}

/// <summary>
/// エフェクトを生成
/// </summary>
/// <param name="name">エフェクトの名前</param>
/// <param name="pos">描画座標</param>
void EffectManager::EffectCreate(std::string name, VECTOR pos)
{
	int handle = -1;
	for (auto& effects : m_effect)
	{
		if (effects.first == name)
		{
			handle = effects.second->emitterHandle;
		}
	}

	//追加しようとしたエフェクトの名前のハンドルがなければ終わる
	if (handle == -1)
	{
		return;
	}

	Effectstruct add;
	add.handle = PlayEffekseer3DEffect(handle);
	add.frame = 0;
	add.x = pos.x;
	add.y = pos.y;
	add.z = pos.z;
	add.isExist = true;

	SetPosPlayingEffekseer3DEffect(add.handle, add.x, add.y, add.z);

	m_effect[name]->effects.emplace_back(add);

	return;
}

/// <summary>
/// エフェクトを削除
/// </summary>
/// <param name="name"></param>
/// <param name="pos"></param>
void EffectManager::RemoveEffect(std::string name, VECTOR pos)
{
	auto it = m_effect.find(name);
	if (it != m_effect.end())
	{
		auto& effects = it->second->effects;
		effects.erase(std::remove_if(effects.begin(), effects.end(),
			[&pos](const Effectstruct& effect) {
				return effect.x == pos.x && effect.y == pos.y && effect.z == pos.z;
			}), effects.end());
	}
}
