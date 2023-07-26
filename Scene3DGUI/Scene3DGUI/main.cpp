#include "base.h"
#include "PointCloudService.h"

int main()
{
	const char* pcdPath = R"(D:\SVN\MindFlow\需求列表\20230721-717\output\data\2023-04-20-06-22-09\global_map.pcd)";
	PCDTYPE _type = PCDTYPE::POINTXYZ;
	std::cout << (int)(PointCloudService::getPcdType(pcdPath, _type)) << std::endl;
}
