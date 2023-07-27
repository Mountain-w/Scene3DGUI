#pragma once
#include "base.h"

/*
*��������
*/
enum class PCDTYPE
{
	POINTXYZ,		// ����
	POINTXYZI,		// ����ǿ����Ϣ�ĵ���
	POINTXYZRGBA	// ����RGBA��Ϣ�ĵ���
};

/*
*���ƻ�����Ϣ
*/
struct PointCloudInfo
{
	std::uint32_t pointNum; // �����еĵ���
	bool hasIntensity;		// �Ƿ���ǿ��
	GLuint VAO = 0;				// ���ڿ��ӻ���VAO
};



class PointCloudService
{
public:
	static bool loadPcd(const char* _pcdPath, PointCloudInfo &_info);
};

