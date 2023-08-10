#include "Geometry.h"

Geometry::BoxInfo* Geometry::TransformMoudle::getCurInfo()
{
	if (m_boxInfos.size() == 0)
	{
		throw "未初始化。。。";
	}
	return m_boxInfos.back();
};

void Geometry::TransformMoudle::update(BoxInfo* _info)
{
	m_boxInfos.push_back(_info);
};

void Geometry::TransformMoudle::undo()
{
	if (m_boxInfos.size() <= 1)
	{
		return;
	}
	delete m_boxInfos.back();
	m_boxInfos.pop_back();
};

/*
* 编辑3D框体：
* 1. 移动
* 2. 旋转（OOBB）
* 3. 框体大小
*/

Geometry::AABB::AABB(Eigen::RowVector3f _position, Eigen::RowVector3f _size)
{
	m_curInfo = new Geometry::BoxInfo(_size, _position);
};

void Geometry::AABB::setColor(int r, int g, int b, int a)
{
	m_color(0) = r;
	m_color(1) = g;
	m_color(2) = b;
	m_color(3) = a;
	update();
};

void Geometry::AABB::updateCurInfo()
{
	m_curInfo = new Geometry::BoxInfo(m_curInfo->size, m_curInfo->position);
};

void Geometry::AABB::move(float x, float y, float z)
{
	m_transform.update(m_curInfo);
	m_curInfo->position[0] += x;
	m_curInfo->position[1] += y;
	m_curInfo->position[2] += z;
	updateCurInfo();
	update();
};

void Geometry::AABB::moveX(float _distance)
{
	m_transform.update(m_curInfo);
	m_curInfo->position += m_front.normalized() * _distance;
	updateCurInfo();
	update();
};

void Geometry::AABB::moveY(float _distance)
{
	m_transform.update(m_curInfo);
	m_curInfo->position += m_left.normalized() * _distance;
	updateCurInfo();
	update();
};

void Geometry::AABB::moveZ(float _distance)
{
	m_transform.update(m_curInfo);
	m_curInfo->position += m_top.normalized() * _distance;
	updateCurInfo();
	update();
};

void Geometry::AABB::resize(RESIZETYPE _type, float _distance)
{
	m_transform.update(m_curInfo);
	switch (_type)
	{
	case RESIZETYPE::UP:
		if (m_curInfo->size[2] + _distance < 0.01f) break;
		m_curInfo->size[2] += _distance;
		m_curInfo->position += _distance / 2.0f * m_top.normalized();
		break;
	case RESIZETYPE::DOWN:
		if (m_curInfo->size[2] + _distance < 0.01f) break;
		m_curInfo->size[2] += _distance;
		m_curInfo->position -= _distance / 2.0f * m_top.normalized();
		break;
	case RESIZETYPE::LEFT:
		if (m_curInfo->size[1] + _distance < 0.01f) break;
		m_curInfo->size[1] += _distance;
		m_curInfo->position += _distance / 2.0f * m_left.normalized();
		break;
	case RESIZETYPE::RIGHT:
		if (m_curInfo->size[1] + _distance < 0.01f) break;
		m_curInfo->size[1] += _distance;
		m_curInfo->position -= _distance / 2.0f * m_left.normalized();
		break;
	case RESIZETYPE::FRONT:
		if (m_curInfo->size[0] + _distance < 0.01f) break;
		m_curInfo->size[0] += _distance;
		m_curInfo->position += _distance / 2.0f * m_front.normalized();
		break;
	case RESIZETYPE::BACK:
		if (m_curInfo->size[0] + _distance < 0.01f) break;
		m_curInfo->size[0] += _distance;
		m_curInfo->position -= _distance / 2.0f * m_front.normalized();
		break;
	default:
		break;
	}
	updateCurInfo();
	update();
};

void Geometry::AABB::undo()
{
	m_transform.undo();
	m_curInfo = m_transform.getCurInfo();
	updateCurInfo();
	update();
};

void Geometry::AABB::update()
{
	// 更新3D框角点位置
	Eigen::MatrixXf corners(8, 4);
    corners <<
		-1, -1, -1, 1,
		1, -1, -1, 1,
		-1, 1, -1, 1,
		-1, -1, 1, 1,
		1, 1, 1, 1,
		-1, 1, 1, 1,
		1, -1, 1, 1,
		1, 1, -1, 1;
	Eigen::RowVector3f scale = m_curInfo->size / 2.0f;
	for (int row = 0; row < 8; row++)
	{
		corners(row, 0) *= scale[0];
		corners(row, 1) *= scale[1];
		corners(row, 2) *= scale[2];
	}
	Eigen::MatrixXf cornerPoints = (m_curInfo->matrix * corners.transpose()).transpose();

	std::vector<GLfloat> vertexs;
	for (int row = 0; row < cornerPoints.rows(); row++)
	{
		vertexs.push_back(cornerPoints(row, 0));
		vertexs.push_back(cornerPoints(row, 1));
		vertexs.push_back(cornerPoints(row, 2));
		vertexs.push_back(0);
		vertexs.push_back(m_color(0));
		vertexs.push_back(m_color(1));
		vertexs.push_back(m_color(2));
		vertexs.push_back(m_color(3));
	}

	GLuint indices[] = {
		0, 1, 1, 7, 7, 2, 2, 0,
		3, 6, 6, 4, 4, 5, 5, 3,
		0, 3, 1, 6, 7, 4, 2, 5,
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glBufferData(GL_ARRAY_BUFFER, vertexs.size() * sizeof(GLfloat), vertexs.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 4));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// EBO
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 解绑VAO、VBO、EBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// 更新前、左、上向量
	Eigen::Vector4f top_homogeneous(0.0f, 0.0f, 1.0f, 1.0f);
	Eigen::Vector4f left_homogeneous(0.0f, 1.0f, 0.0f, 1.0f);
	Eigen::Vector4f front_homogeneous(1.0f, 0.0f, 0.0f, 1.0f);

	m_top = (m_curInfo->matrix * top_homogeneous).head<3>() - m_curInfo->position;
	m_left = (m_curInfo->matrix * left_homogeneous).head<3>() - m_curInfo->position;
	m_front = (m_curInfo->matrix * front_homogeneous).head<3>() - m_curInfo->position;
};

Geometry::OOBB::OOBB(Eigen::RowVector3f _position, Eigen::RowVector3f _size, Eigen::RowVector3f _euler)
{
	m_curInfo = new Geometry::BoxInfo(_size, _position, _euler);
};

void Geometry::OOBB::updateCurInfo()
{
	m_curInfo = new Geometry::BoxInfo(m_curInfo->size, m_curInfo->position, m_curInfo->euler);
};

void Geometry::OOBB::yaw(float _degree)
{
	m_transform.update(m_curInfo);
	// 范围[-180, 180]
	float degree = m_curInfo->euler[2] + _degree;
	if (degree < -180.0f)
	{
		m_curInfo->euler[2] = -180.0f;
	}
	else if (degree > 180.0f)
	{
		m_curInfo->euler[2] = 180.0f;
	}
	else
	{
		m_curInfo->euler[2] = degree;
	}
	updateCurInfo();
};

void Geometry::OOBB::roll(float _degree)
{
	m_transform.update(m_curInfo);
	// 范围[-89.0, 89.0]
	float degree = m_curInfo->euler[0] + _degree;
	if (degree < -89.0f)
	{
		m_curInfo->euler[0] = -89.0f;
	}
	else if (degree > 89.0f)
	{
		m_curInfo->euler[0] = 89.0f;
	}
	else
	{
		m_curInfo->euler[0] = degree;
	}
	updateCurInfo();
};

void Geometry::OOBB::pitch(float _degree)
{
	m_transform.update(m_curInfo);
	// 范围[-89.0, 89.0]
	float degree = m_curInfo->euler[1] + _degree;
	std::cout << degree << std::endl;
	if (degree < -89.0f)
	{
		m_curInfo->euler[1] = -89.0f;
	}
	else if (degree > 89.0f)
	{
		m_curInfo->euler[1] = 89.0f;
	}
	else
	{
		m_curInfo->euler[1] = degree;
	}
	std::cout << (m_curInfo->euler[1]) << std::endl;
	updateCurInfo();
};