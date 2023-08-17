#pragma once
#include "base.h"

namespace Geometry
{
	struct BoxInfo
	{
		BoxInfo(Eigen::RowVector3f _size, Eigen::RowVector3f _position, Eigen::RowVector3f _euler=Eigen::RowVector3f(0.0f, 0.0f, 0.0f))
		{
			size = _size;
			position = _position;
			euler = _euler;
			Eigen::Affine3f affine = Eigen::Affine3f::Identity();
			affine = Eigen::AngleAxisf(glm::radians(euler(2)), Eigen::Vector3f::UnitZ()) * Eigen::AngleAxisf(glm::radians(euler(1)), Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(glm::radians(euler(0)), Eigen::Vector3f::UnitX());
			affine.translation() << position(0), position(1), position(2);
			matrix = affine.matrix();
		}
		~BoxInfo(){}

		Eigen::Vector3f size;						// 长宽高
		Eigen::Vector3f position;					// 中心点
		Eigen::Vector3f euler;						// roll，pitch，yaw
		Eigen::Matrix4f matrix;
	};
	

	class TransformMoudle
	{
	public:
		TransformMoudle(){};
		~TransformMoudle()
		{
			for (int i = 0; i < m_boxInfos.size(); i++)
			{
				delete m_boxInfos.back();
				m_boxInfos.pop_back();
			}
		}

		BoxInfo* getCurInfo();
		void update(BoxInfo* _info);
		void undo();

	private:
		std::vector<BoxInfo*> m_boxInfos;
	};

	enum class RESIZETYPE
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		FRONT,
		BACK
	};

	class AABB
	{
	public:
		AABB() {};
		AABB(Eigen::RowVector3f _position, Eigen::RowVector3f _size);
		~AABB() {};

		void undo();									// 撤销修改操作
		void update();									// 更新可视化对象
		virtual void updateCurInfo();					// 更新当前boxinfo
		void move(float x, float y, float z);			// 中心点移动
		void moveX(float _distance);					// 朝向自身坐标系 x 移动
		void moveY(float _distance);					// 朝向自身坐标系 y 移动
		void moveZ(float _distance);					// 朝向自身坐标系 z 移动
		void resize(RESIZETYPE _type, float _distance); // 修改3D框大小
		void setColor(int r, int g, int b, int a);		// 设置颜色
		glm::vec3 getTop(){ return glm::normalize(glm::vec3(m_top[0], m_top[1], m_top[2])); }
		glm::vec3 getLeft() { return glm::normalize(glm::vec3(m_left[0], m_left[1], m_left[2])); }
		glm::vec3 getFront() { return glm::normalize(glm::vec3(m_front[0], m_front[1], m_front[2])); }
		glm::vec3 getCenter() 
		{
			return glm::vec3(
				m_curInfo->position[0],
				m_curInfo->position[1],
				m_curInfo->position[2]
			);
		}

	public:
		GLuint vao;										// 用于可视化

	protected:
		BoxInfo* m_curInfo;

		TransformMoudle m_transform;					// 变换模块

		Eigen::Vector3f m_top{ 0.0f, 0.0f, 1.0f };		// 3D框顶部向量，默认+z
		Eigen::Vector3f m_left{ 0.0f, 1.0f, 0.0f };		// 3D框左向量
		Eigen::Vector3f m_front{ 1.0f, 0.0f, 0.0f };	// 3D框朝向,默认+x
		
		GLuint vbo;										// 3D框的VBO
		GLuint ebo;										// 3D框的EBO
		Eigen::RowVector4i m_color{255, 0, 255, 255};	// 3D框的颜色
	};

	class OOBB: public AABB
	{
	public:
		OOBB(Eigen::RowVector3f _position, Eigen::RowVector3f _size, Eigen::RowVector3f _euler);
		~OOBB() {};

		void yaw(float _degree);
		void roll(float _degree);
		void pitch(float _degree);
		
		void updateCurInfo() override;					// 更新当前boxinfo

	protected:
		Geometry::AABB aabb;							// 最小外接aabb
	};
};

