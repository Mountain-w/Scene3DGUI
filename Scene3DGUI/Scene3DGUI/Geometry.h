#pragma once
#include "base.h"

namespace Geometry
{
	class TransformMoudle
	{
	public:
		TransformMoudle(){};
		~TransformMoudle()
		{
			for (int i = 0; i < m_matrixList.size(); i++)
			{
				delete m_matrixList.back();
				m_matrixList.pop_back();
			}
		}

		Eigen::Matrix4f getCurMatrix();
		void update(Eigen::Matrix4f* _matrix);
		void undo();

	private:
		std::vector<Eigen::Matrix4f*> m_matrixList;
	};

	class AABB
	{
	public:
		AABB() {};
		AABB(Eigen::RowVector3f _position, Eigen::RowVector3f _size);
		~AABB() {};

	protected:
		Eigen::RowVector3f m_position;	// 中心点
		Eigen::RowVector3f m_size;		// 长宽高
		Eigen::Matrix<float, 8, 4> m_corners;
		TransformMoudle transform;  // 变换模块
	};

	class OOBB: public AABB
	{
	public:
		OOBB(Eigen::RowVector3f _position, Eigen::RowVector3f _size, Eigen::RowVector3f _euler);
		~OOBB() {};

	protected:
		Eigen::RowVector3f m_euler;		// roll，pitch，yaw
	};
};

