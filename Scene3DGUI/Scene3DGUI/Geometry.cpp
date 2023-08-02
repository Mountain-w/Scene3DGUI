#include "Geometry.h"

Eigen::Matrix4f Geometry::TransformMoudle::getCurMatrix()
{
	if (m_matrixList.size() == 0)
	{
		throw "±‰ªªæÿ’ÛŒ¥≥ı ºªØ°£°£°£";
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

void Geometry::AABB::update()
{
	Eigen::MatrixXf cornerPoints = transform.getCurMatrix() * m_corners.transpose();
	std::cout << cornerPoints;
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