#pragma once
#include "base.h"

/*
*���ƻ�����Ϣ
*/
struct PointCloudInfo
{

};

enum class PCDTYPE
{
	POINTXYZ,		// ����
	POINTXYZI,		// ����ǿ����Ϣ�ĵ���
	POINTXYZRGB,	// ����RGB��Ϣ�ĵ���
	POINTXYZRGBA	// ����RGBA��Ϣ�ĵ���
};

class PointCloudService
{
public:
	static bool getPcdType(const char* pcdPath, PCDTYPE &_type);
	static PointCloudInfo* loadPcd(const char* pcdPath);
};

