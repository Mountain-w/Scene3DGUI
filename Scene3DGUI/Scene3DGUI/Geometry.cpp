#include "Geometry.h"

Eigen::Matrix4f Geometry::TransformMoudle::getCurMatrix()
{
	if (m_matrixList.size() == 0)
	{
		throw "变换矩阵未初始化。。。";
	}
	return *(m_matrixList.back());
};

void Geometry::TransformMoudle::update(Eigen::Matrix4f* _matrix)
{
	Eigen::Matrix4f* newMatrix;
	if (m_matrixList.size() == 0)
	{
		newMatrix = _matrix;
	}
	else
	{
		newMatrix = new Eigen::Matrix4f((*_matrix) * (*(m_matrixList.back())));
	}
	m_matrixList.push_back(newMatrix);
};

void Geometry::TransformMoudle::undo()
{
	if (m_matrixList.size() <= 1)
	{
		return;
	}
	delete m_matrixList.back();
	m_matrixList.pop_back();
};

void Geometry::AABB::setColor(int r, int g, int b, int a)
{
	m_color(0) = r;
	m_color(1) = g;
	m_color(2) = b;
	m_color(3) = a;
};

Geometry::AABB::AABB(Eigen::RowVector3f _position, Eigen::RowVector3f _size)
{
	m_position = _position;
	m_size = _size;
	m_corners <<
		-1, -1, -1, 1,
		1, -1, -1, 1,
		-1, 1, -1, 1,
		-1, -1, 1, 1,
		1, 1, 1, 1,
		-1, 1, 1, 1,
		1, -1, 1, 1,
		1, 1, -1, 1;

	Eigen::RowVector3f scale = m_size / 2.0f;
	for (int row = 0; row < 8; row++)
	{
		m_corners(row, 0) *= scale[0];
		m_corners(row, 1) *= scale[1];
		m_corners(row, 2) *= scale[2];
	}
	Eigen::Affine3f affine = Eigen::Affine3f::Identity();
	affine.translation() << m_position(0), m_position(1), m_position(2);
	Eigen::Matrix4f* _matrix = new Eigen::Matrix4f(affine.matrix());
	transform.update(_matrix);
};

void Geometry::AABB::move(float x, float y, float z)
{
	Eigen::Affine3f affine = Eigen::Affine3f::Identity();
	affine.translation() << x, y, z;
	Eigen::Matrix4f* _matrix = new Eigen::Matrix4f(affine.matrix());
	transform.update(_matrix);
	update();
};
void Geometry::AABB::undo()
{
	transform.undo();
	update();
};

void Geometry::AABB::update()
{
	Eigen::MatrixXf cornerPoints = (transform.getCurMatrix() * m_corners.transpose()).transpose();

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
		0, 3, 1, 6, 7, 4, 2, 5
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
};

Geometry::OOBB::OOBB(Eigen::RowVector3f _position, Eigen::RowVector3f _size, Eigen::RowVector3f _euler)
{
	m_position = _position;
	m_size = _size;
	m_euler = _euler;
	m_corners <<
		-1, -1, -1, 1,
		1, -1, -1, 1,
		-1, 1, -1, 1,
		-1, -1, 1, 1,
		1, 1, 1, 1,
		-1, 1, 1, 1,
		1, -1, 1, 1,
		1, 1, -1, 1;

	Eigen::RowVector3f scale = m_size / 2.0f;
	for (int row = 0; row < 8; row++)
	{
		m_corners(row, 0) *= scale[0];
		m_corners(row, 1) *= scale[1];
		m_corners(row, 2) *= scale[2];
	}
	Eigen::Affine3f affine = Eigen::Affine3f::Identity();
	affine = Eigen::AngleAxisf(m_euler(2), Eigen::Vector3f::UnitZ()) * Eigen::AngleAxisf(m_euler(1), Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(m_euler(0), Eigen::Vector3f::UnitX());

	affine.translation() << m_position(0), m_position(1), m_position(2);
	Eigen::Matrix4f* _matrix = new Eigen::Matrix4f(affine.matrix());
	transform.update(_matrix);
};