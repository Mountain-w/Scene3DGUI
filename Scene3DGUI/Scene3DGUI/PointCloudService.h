#pragma once
#include "base.h"

/*
*点云类型
*/
enum class PCDTYPE
{
	POINTXYZ,		// 纯点
	POINTXYZI,		// 带有强度信息的点云
	POINTXYZRGBA	// 带有RGBA信息的点云
};

/*
*点云基础信息
*/
struct PointCloudInfo
{
	std::uint32_t pointNum; // 点云中的点数
	bool hasIntensity;		// 是否有强度
	GLuint VAO = 0;				// 用于可视化的VAO
};



class PointCloudService
{
public:
	static bool loadPcd(const char* _pcdPath, PointCloudInfo &_info);
};

