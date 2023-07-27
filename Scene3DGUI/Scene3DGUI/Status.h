#pragma once
#include "base.h"

enum class MOUSESTATUS
{
	LEFTCLICKED,
	MIDDLECLICKED,
	RIGHTCLICKED,
	LEFTRELEASE,
	MIDDLERELEASE,
	RIGHTRELEASE,
	NOTHING
};

class Status
{
public:
	void activate(MOUSESTATUS _mode, double _xPos, double _yPos);
	void release(MOUSESTATUS _mode);
	MOUSESTATUS curMode();
	glm::vec2 difference(double _xPos, double _yPos);

private:
	bool m_isMouseLeftClicked = false;
	bool m_isMouseMiddleClicked = false;
	bool m_isMouseRightClicked = false;

	glm::vec2 m_lastPos{ 1.0f };
};
