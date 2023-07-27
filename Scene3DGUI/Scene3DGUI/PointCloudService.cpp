#include "PointCloudService.h"

std::string lower(std::string _input)
{
	std::string _output;
	_output.resize(_input.size());
	_input.copy(&_output[0], _input.size(), 0);

	std::transform(_output.begin(), _output.end(), _output.begin(), [](unsigned char c) {return std::tolower(c); });
	return _output;
}

bool PointCloudService::loadPcd(const char* _pcdPath, PointCloudInfo& _info)
{
	// 任意列点云对象
	pcl::PCLPointCloud2::Ptr cloudIn(new pcl::PCLPointCloud2);
	pcl::PCDReader _reader;

	// 读取点云
	if (_reader.read(_pcdPath, *cloudIn) != 0)
	{
		return false;
	}
	
	// 取出头部信息，并判断是哪种类型点云
	bool hasIntensity{ false };

	std::unique_ptr<GLfloat[]> vertexs(new GLfloat[cloudIn->width * cloudIn->height * 8]);
	for (size_t i = 0; i < cloudIn->width * cloudIn->height; i++)
	{
		// 为颜色赋默认值 255，255，0
		vertexs[i * 8 + 4] = 255;
		vertexs[i * 8 + 5] = 255;
		vertexs[i * 8 + 7] = 255;
		
		for (size_t j = 0; j < cloudIn->fields.size(); j++)
		{
			int column{ 0 };
			// 判断是哪一列
			if (lower(cloudIn->fields[j].name) == "x") { column = 0; }			
			else if (lower(cloudIn->fields[j].name) == "y") { column = 1; }
			else if (lower(cloudIn->fields[j].name) == "z") { column = 2; }
			else if (lower(cloudIn->fields[j].name) == "intensity") { column = 3; }
			else if (lower(cloudIn->fields[j].name) == "rgb") { column = 4; }
			else if (lower(cloudIn->fields[j].name) == "rgba") { column = 4; }
			else { continue; }

			// 取出对应值
			int fieldOffset = cloudIn->fields[j].offset;
			int fieldDtype = cloudIn->fields[j].datatype;
			void* fieldPtr = &(cloudIn->data[i * cloudIn->point_step + fieldOffset]);

			// 如果带有颜色信息，优先处理
			std::uint32_t rgba;
			rgba = *static_cast<std::uint32_t*>(fieldPtr);
			if (lower(cloudIn->fields[j].name) == "rgb")
			{
				vertexs[i * 8 + column] = ((rgba & 0x00FF0000) >> 16) & 0xFF;
				vertexs[i * 8 + column + 1] = ((rgba & 0x0000FF00) >> 8) & 0xFF;
				vertexs[i * 8 + column + 2] = (rgba & 0x000000FF) & 0xFF;
				continue;
			}
			else if (lower(cloudIn->fields[j].name) == "rgba")
			{
				vertexs[i * 8 + column] = ((rgba & 0xFF000000) >> 24) & 0xFF;
				vertexs[i * 8 + column + 1] = ((rgba & 0x00FF0000) >> 16) & 0xFF;
				vertexs[i * 8 + column + 2] = ((rgba & 0x0000FF00) >> 8) & 0xFF;
				vertexs[i * 8 + column + 3] = (rgba & 0x000000FF) & 0xFF;
				continue;
			}

			switch (fieldDtype)
			{
			case pcl::PCLPointField::FLOAT32:
				vertexs[i * 8 + column] = *static_cast<std::float_t*>(fieldPtr);
				break;
			case pcl::PCLPointField::FLOAT64:
				vertexs[i * 8 + column] = *static_cast<std::double_t*>(fieldPtr);
				break;
			case pcl::PCLPointField::INT8:
				vertexs[i * 8 + column] = *static_cast<std::int8_t*> (fieldPtr);
				break;
			case pcl::PCLPointField::UINT8:
				vertexs[i * 8 + column] = *static_cast<std::uint8_t*>(fieldPtr);
				break;
			case pcl::PCLPointField::INT16:
				vertexs[i * 8 + column] = *static_cast<std::int16_t*>(fieldPtr);
				break;
			case pcl::PCLPointField::UINT16:
				vertexs[i * 8 + column] = *static_cast<std::uint16_t*>(fieldPtr);
				break;
			case pcl::PCLPointField::INT32:
				vertexs[i * 8 + column] = *static_cast<std::int32_t*>(fieldPtr);
				break;
			case pcl::PCLPointField::UINT32:
				vertexs[i * 8 + column] = *static_cast<std::uint32_t*>(fieldPtr);
				break;
			case pcl::PCLPointField::INT64:
				vertexs[i * 8 + column] = *static_cast<std::int64_t*>(fieldPtr);
				break;
			case pcl::PCLPointField::UINT64:
				vertexs[i * 8 + column] = *static_cast<std::uint64_t*>(fieldPtr);
				break;
			default:
				std::cout << "unknown type,'" << pcl::getFieldType(fieldDtype) << "' switch to default FLOAT32" << std::endl;
				vertexs[i * 8 + column] = *static_cast<float*>(fieldPtr);
				break;
			}	
		}
	}
	/*for (int i = 0; i < 800; i += 8)
	{
		std::cout << vertexs[i] << " ";
		std::cout << vertexs[i+1] << " ";
		std::cout << vertexs[i+2] << " ";
		std::cout << vertexs[i+3] << " ";
		std::cout << vertexs[i+4] << " ";
		std::cout << vertexs[i+5] << " ";
		std::cout << vertexs[i+6] << " ";
		std::cout << vertexs[i+7] << " ";
		std::cout << std::endl;
	}*/
	// 统计点云信息
	_info.hasIntensity = hasIntensity;
	_info.pointNum = cloudIn->width * cloudIn->height;

	// 绑定VAO
	glGenVertexArrays(1, &(_info.VAO));
	glBindVertexArray(_info.VAO);

	// 构建VBO
	GLuint VBO = 0;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, _info.pointNum * sizeof(GLfloat) * 8, vertexs.get(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 3));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(sizeof(GLfloat) * 4));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// 解绑VAO、VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
};