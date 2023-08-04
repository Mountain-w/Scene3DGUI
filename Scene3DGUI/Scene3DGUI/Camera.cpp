#include "Camera.h"

void Camera::lookAt(glm::vec3 _pos, glm::vec3 _front, glm::vec3 _up)
{
	m_position = _pos;
	m_front = glm::normalize(_front);
	m_up = _up;
	m_vMatrix = glm::lookAt(m_position, m_front + m_position, m_up);
};

void Camera::update()
{
	// 根据中心点、pitch、yaw、距离求出中心点
	m_position.x = m_centerPosition.x + m_distance * cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
	m_position.y = m_centerPosition.y + m_distance * cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
	m_position.z = m_centerPosition.z + m_distance * sin(glm::radians(m_pitch));
	m_front = glm::normalize(m_centerPosition - m_position);

	m_vMatrix = glm::lookAt(m_position, m_front + m_position, m_up);
};

glm::mat4 Camera::getMatrix()
{
	return m_vMatrix;
};

void Camera::move(CAMERA_MOVE _mode)
{
	switch (_mode)
	{
	case CAMERA_MOVE::MOVE_FRONT:
		if (m_distance < 3)
		{
			return;
		}
		m_position += 2.0f * m_speed * m_front;
		m_distance = glm::length(m_centerPosition - m_position);
		break;
	case CAMERA_MOVE::MOVE_BACK:
		m_position -= 2.0f * m_speed * m_front;
		m_distance = glm::length(m_centerPosition - m_position);
		break;
	default:
		break;
	}
};

void Camera::move(float _xOffset, float _yOffset)
{
	_xOffset = -_xOffset;
	//横向移动
	m_position += glm::normalize(glm::cross(m_front, m_up)) * _xOffset * m_moveSpeed;
	m_centerPosition += glm::normalize(glm::cross(m_front, m_up)) * _xOffset * m_moveSpeed;

	//纵向移动
	m_position += glm::normalize(glm::cross(glm::cross(m_front, m_up), m_front)) * _yOffset * m_moveSpeed;
	m_centerPosition += glm::normalize(glm::cross(glm::cross(m_front, m_up), m_front)) * _yOffset * m_moveSpeed;
};

void Camera::setSpeed(float _speed)
{
	m_speed = _speed;
};

void Camera::setMoveSpeed(float _speed)
{
	m_moveSpeed = _speed;
};

void Camera::pitch(float _yOffset)
{
	// 旋转角度
	m_pitch += _yOffset * m_sentitivity;
	if (m_pitch > 89.0f)
	{
		m_pitch = 89.0f;
	}
	if (m_pitch < -89.0f)
	{
		m_pitch = -89.0f;
	}
};
void Camera::yaw(float _xOffset)
{
	// 旋转角度
	m_yaw += _xOffset * m_sentitivity;
	if (m_yaw >= 360.0f)
	{
		m_yaw -= 360.f;
	}
	if (m_yaw < 0.0f)
	{
		m_yaw += 360.0f;
	}
};
void Camera::setSentitivity(float _s)
{
	m_sentitivity = _s;
};

void Camera::onMouseMove(double _xpos, double _ypos)
{
	pitch(_ypos);
	yaw(-_xpos);
	update();
};