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
			affine = Eigen::AngleAxisf(euler(2), Eigen::Vector3f::UnitZ()) * Eigen::AngleAxisf(euler(1), Eigen::Vector3f::UnitY()) * Eigen::AngleAxisf(euler(0), Eigen::Vector3f::UnitX());
			affine.translation() << position(0), position(1), position(2);
			matrix = affine.matrix();
		}
		~BoxInfo(){}

		Eigen::RowVector3f size;						// �����
		Eigen::RowVector3f position;					// ���ĵ�
		Eigen::RowVector3f euler;						// roll��pitch��yaw
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
		void move(float x, float y, float z);			// �ƶ�
		void resize(RESIZETYPE _type, float _distance); // �޸�3D���С
		void setColor(int r, int g, int b, int a);		// ������ɫ

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

	protected:
		Geometry::AABB aabb;							// ��С���aabb
	};
};

