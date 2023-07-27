#include "Status.h"

void Status::activate(MOUSESTATUS _mode, double _xPos, double _yPos)
{
	// 检测是否已经有键被激活了
	if (m_isMouseLeftClicked || m_isMouseMiddleClicked || m_isMouseRightClicked)
	{
		return;
	}

	// 否则就激活对应按键，记录当前点位置
	switch (_mode)
	{
	case MOUSESTATUS::LEFTCLICKED:
		m_isMouseLeftClicked = true;
		break;
	case MOUSESTATUS::MIDDLECLICKED:
		m_isMouseMiddleClicked = true;
		break;
	case MOUSESTATUS::RIGHTCLICKED:
		m_isMouseRightClicked = true;
		break;
	default:
		break;
	}
	m_lastPos.x = _xPos;
	m_lastPos.y = _yPos;
};

void Status::release(MOUSESTATUS _mode)
{
	switch (_mode)
	{
	case MOUSESTATUS::LEFTRELEASE:
		m_isMouseLeftClicked = false;
		break;
	case MOUSESTATUS::MIDDLERELEASE:
		m_isMouseMiddleClicked = false;
		break;
	case MOUSESTATUS::RIGHTRELEASE:
		m_isMouseRightClicked = false;
		break;
	default:
		break;
	}
};

MOUSESTATUS Status::curMode()
{
	if (m_isMouseLeftClicked)
	{
		return MOUSESTATUS::LEFTCLICKED;
	}
	else if (m_isMouseMiddleClicked)
	{
		return MOUSESTATUS::MIDDLECLICKED;
	}
	else if (m_isMouseRightClicked)
	{
		return MOUSESTATUS::RIGHTCLICKED;
	}
	else
	{
		return MOUSESTATUS::NOTHING;
	}
};

glm::vec2 Status::difference(double _xPos, double _yPos)
{
	glm::vec2 _differenceValue{ 0.0f };
	if (curMode() == MOUSESTATUS::NOTHING)
	{
		return _differenceValue;
	}

	// 计算差值
	_differenceValue.x = _xPos - m_lastPos.x;
	_differenceValue.y = _yPos - m_lastPos.y;

	// 更新位置
	m_lastPos.x = _xPos;
	m_lastPos.y = _yPos;
	return _differenceValue;
};