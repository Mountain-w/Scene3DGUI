#include "Status.h"

void Status::activate(MOUSESTATUS _mode, double _xPos, double _yPos)
{
	// ����Ƿ��Ѿ��м���������
	if (m_isMouseLeftClicked || m_isMouseMiddleClicked || m_isMouseRightClicked)
	{
		return;
	}

	// ����ͼ����Ӧ��������¼��ǰ��λ��
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

	// �����ֵ
	_differenceValue.x = _xPos - m_lastPos.x;
	_differenceValue.y = _yPos - m_lastPos.y;

	// ����λ��
	m_lastPos.x = _xPos;
	m_lastPos.y = _yPos;
	return _differenceValue;
};