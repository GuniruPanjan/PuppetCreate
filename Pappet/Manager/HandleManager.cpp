#include "HandleManager.h"
#include "DxLib.h"
#include<cassert>

HandleManager* HandleManager::m_instance = nullptr;

HandleManager::HandleManager()
{
}

HandleManager::~HandleManager()
{
	for (auto& h : m_handles)
	{
		MV1DeleteModel(h.handle);
	}

	m_handles.clear();
}

int HandleManager::GetModelHandle(std::string path, bool isEternal)
{
	//すでにロードされていたら複製ハンドルを返す
	for (auto& h : m_handles)
	{
		if (h.path == path)
		{
			return MV1DuplicateModel(h.handle);
		}
	}

	//ロードされていなかったら
	Handle add;
	add.handle = MV1LoadModel(path.c_str());
	add.path = path;
	add.isEternal = isEternal;

	m_handles.emplace_back(add);

	//ロードしたハンドルの複製ハンドルを返す
	return MV1DuplicateModel(add.handle);
}

void HandleManager::Clear()
{
	//isEternalがfalseのハンドルをDeleteする
	for (auto& h : m_handles)
	{
		if (!h.isEternal)
		{
			MV1DeleteModel(h.handle);
			h.handle = 0;
		}
	}

	//不要になったハンドルをここで削除処理する
	auto it = remove_if(m_handles.begin(), m_handles.end(), []
	(auto& v) {
			return v.handle == 0;
		       });

	m_handles.erase(it, m_handles.end());
}
