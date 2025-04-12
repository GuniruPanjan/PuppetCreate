#include "StateBase.h"
#include "CharacterBase.h"
#include <string>
#include <cassert>

StateBase::StateBase(std::shared_ptr<CharacterBase> chara) :
	m_pChara(chara),
	m_nowState(StateKind::Null),
	m_nextState(StateKind::Null)
{
}

StateBase::~StateBase()
{
}

void StateBase::ChangeState(StateKind kind)
{
	//ƒ^ƒO‚ðŽæ“¾
	auto tag = m_pChara.lock()->GetTag();

}

bool StateBase::CheckState()
{
	return false;
}

const StateBase::eDir StateBase::GetDirection(float x, float y) const
{
	auto angle = atan2(y, x);
	if (angle < 0)
	{
		angle = angle + 2 * DX_PI_F;
	}
	angle = floor(angle * 360 / (2 * DX_PI_F));

	if (23 <= angle && angle <= 67)
	{
		return eDir::ForwardRight;
	}
	else if (68 <= angle && angle <= 112)
	{
		return eDir::Forward;
	}
	else if (113 <= angle && angle <= 157)
	{
		return eDir::ForwardLeft;
	}
	else if (158 <= angle && angle <= 202)
	{
		return eDir::Left;
	}
	else if (203 <= angle && angle <= 247)
	{
		return eDir::BackLeft;
	}
	else if (248 <= angle && angle <= 292)
	{
		return eDir::Back;
	}
	else if (293 <= angle && angle <= 337)
	{
		return eDir::BackRight;
	}
	else
	{
		return Back;
	}
}
