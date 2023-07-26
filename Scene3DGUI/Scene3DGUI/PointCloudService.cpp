#include "PointCloudService.h"

bool PointCloudService::getPcdType(const char* pcdPath, PCDTYPE& _type)
{
	return true;
};

PointCloudInfo* PointCloudService::loadPcd(const char* pcdPath)
{
	PointCloudInfo* info = new PointCloudInfo;
};