#pragma once
#include "base.h"

enum class CAMERA_MOVE
{
	MOVE_LEFT,
	MOVE_RIGHT,
	MOVE_FRONT,
	MOVE_BACK,
	MOVE_UP,
	MOVE_DOWN
};

class Camera
{
public:
	Camera(glm::vec3 _centerPosition = glm::vec3(0.0f))
	{
		m_centerPosition = _centerPosition;
		m_up = glm::vec3(0.0f, 0.0f, 1.0f);

		m_vMatrix = glm::mat4(1.0f);
		update();
	};
	~Camera() {};

	void lookAt(glm::vec3 _pos, glm::vec3 _front, glm::vec3 _up);
	void update();

	glm::mat4 getMatrix();
	void move(CAMERA_MOVE _mode);
	void move(float xOffset, float yOffset);
	void setSpeed(float _speed);
	void setMoveSpeed(float _speed);

	void pitch(float _yOffset);
	void yaw(float _xOffset);
	void setSentitivity(float _s);
	void onMouseMove(double _xpos, double _ypos);

private:
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	float m_speed = 0.1f;		// �����ֿ������ǰ�������ٶ�
	float m_moveSpeed = 0.03f; // ����м����������������ƶ��ٶ�
	float m_distance = 15.0f;

	float m_pitch = 45.0f;
	float m_yaw = 180.0f;
	float m_sentitivity = 0.1f;

	glm::mat4 m_vMatrix;

	glm::vec3 m_centerPosition;  // �����ת�����ĵ�
};

class SimpleCamera
{
public:
	SimpleCamera(float _distance = 5.0f) :m_distance(_distance) {};

	void update()
	{
		glm::vec3 _position = m_centerPosition + m_vector * m_distance;
		m_vMatrix = glm::lookAt(_position, m_centerPosition, m_up);
	};

	void flash(glm::vec3 _center, glm::vec3 _vector)
	{
		m_centerPosition = _center;
		m_vector = _vector;
		update();
	};

	glm::mat4 getMatrix() { return m_vMatrix; }
	void setDistance(float _distance) 
	{ 
		m_distance = _distance; 
		update();
	}
	void setUp(glm::vec3 _up) { m_up = _up; }

private:
	float m_distance = 5.0f;				// ���λ�������ĵ�ľ���
	glm::mat4 m_vMatrix{1.0f};				// lookat����
	glm::vec3 m_vector{ 0.0f };				// ���ĵ㿴�����������
	glm::vec3 m_centerPosition{ 0.0f };		// ���ĵ�λ��
	glm::vec3 m_up{ 0.0f, 0.0f, 1.0f };		// 񷶥����
};
