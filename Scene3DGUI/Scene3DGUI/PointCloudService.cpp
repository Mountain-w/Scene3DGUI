#include "PointCloudService.h"

bool PointCloudService::getPcdType(const char* pcdPath, PCDTYPE& _type)
{
	pcl::PCDReader _reader;
	pcl::PCDHeader _header;

	if (_reader.readHeader(pcdPath) != 0)
	{
		return false;
	}
	std::cout << "Version:" << _header.version << std::endl;
	std::cout << "Fields:" << _header.fields.size() << std::endl;
	std::cout << "Width:" << _header.width << std::endl;
	std::cout << "Height:" << _header.height << std::endl;
	std::cout << "PointNum:" << _header.point_count << std::endl;

	return true;
};

PointCloudInfo* PointCloudService::loadPcd(const char* pcdPath)
{
	PointCloudInfo* info = new PointCloudInfo;
};