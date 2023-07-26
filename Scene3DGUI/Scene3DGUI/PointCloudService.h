#pragma once
#include "base.h"

/*
*点云基础信息
*/
struct PointCloudInfo
{

};

enum class PCDTYPE
{
	POINTXYZ,		// 纯点
	POINTXYZI,		// 带有强度信息的点云
	POINTXYZRGB,	// 带有RGB信息的点云
	POINTXYZRGBA	// 带有RGBA信息的点云
};

class PointCloudService
{
public:
	static bool getPcdType(const char* pcdPath, PCDTYPE &_type);
	static PointCloudInfo* loadPcd(const char* pcdPath);
};

