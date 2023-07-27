#include "ffImage.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ffImage* ffImage::readFromFile(const char* fileName)
{
	int picType = 0;
	int width = 0;
	int height = 0;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* bits = stbi_load(fileName, &width, &height, &picType, STBI_rgb_alpha);
	ffImage* image = new ffImage(width, height, picType, (ffRGBA*)bits);

	stbi_image_free(bits);
	return image;
};