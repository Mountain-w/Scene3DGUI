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

		Eigen::Vector3f size;						// �����
		Eigen::Vector3f position;					// ���ĵ�
		Eigen::Vector3f euler;						// roll��pitch��yaw
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

		void undo();									// �����޸Ĳ���
		void update();									// ���¿��ӻ�����
		virtual void updateCurInfo();					// ���µ�ǰboxinfo
		void move(float x, float y, float z);			// ���ĵ��ƶ�
		void moveX(float _distance);					// ������������ϵ x �ƶ�
		void moveY(float _distance);					// ������������ϵ y �ƶ�
		void moveZ(float _distance);					// ������������ϵ z �ƶ�
		void resize(RESIZETYPE _type, float _distance); // �޸�3D���С
		void setColor(int r, int g, int b, int a);		// ������ɫ
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
		GLuint vao;										// ���ڿ��ӻ�

	protected:
		BoxInfo* m_curInfo;

		TransformMoudle m_transform;					// �任ģ��

		Eigen::Vector3f m_top{ 0.0f, 0.0f, 1.0f };		// 3D�򶥲�������Ĭ��+z
		Eigen::Vector3f m_left{ 0.0f, 1.0f, 0.0f };		// 3D��������
		Eigen::Vector3f m_front{ 1.0f, 0.0f, 0.0f };	// 3D����,Ĭ��+x
		
		GLuint vbo;										// 3D���VBO
		GLuint ebo;										// 3D���EBO
		Eigen::RowVector4i m_color{255, 0, 255, 255};	// 3D�����ɫ
	};

	class OOBB: public AABB
	{
	public:
		OOBB(Eigen::RowVector3f _position, Eigen::RowVector3f _size, Eigen::RowVector3f _euler);
		~OOBB() {};

		void yaw(float _degree);
		void roll(float _degree);
		void pitch(float _degree);
		
		void updateCurInfo() override;					// ���µ�ǰboxinfo

	protected:
		Geometry::AABB aabb;							// ��С���aabb
	};
};

