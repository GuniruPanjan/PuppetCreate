#pragma once
#include "DxLib.h"
#include "Library/MyLibrary.h"
#include <memory>

/// <summary>
/// オブジェクトベースクラス
/// </summary>
class ObjectBase : public MyLibrary::Collidable
{
public:
	ObjectBase(Priority priority, ObjectTag tag);
	virtual ~ObjectBase() {};

	virtual void Init() {};
	virtual void Update() {};
	virtual void Draw() {};
};

