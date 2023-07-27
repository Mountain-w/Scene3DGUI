#pragma once
#include "base.h"


class ffImage
{
public:
	ffRGBA getColor(int x, int y) const
	{
		if (x < 0 || x > m_width || y < 0 || y > m_height)
		{
			return ffRGBA(0, 0, 0, 0);
		}
		return m_data[y * m_width + x];
	}
	ffImage(int width = 0, int height = 0, int picType = 0, ffRGBA* data = nullptr)
	{
		m_width = width;
		m_height = height;
		m_picType = picType;

		int sumSize = m_width * m_height;
		if (data && sumSize)
		{
			m_data = new ffRGBA[sumSize];
			memcpy(m_data, data, sizeof(ffRGBA) * sumSize);
		}
		else
		{
			m_data = nullptr;
		}
	}
	~ffImage()
	{
		if (m_data)
		{
			delete[]m_data;
		}
	}
	
	int getWidth() const { return m_width; };
	int getHeight() const { return m_height; };
	int getPicType() const { return m_picType; };
	ffRGBA* getData() const { return m_data; };
	static ffImage* readFromFile(const char* fileName);

private:
	int m_width;
	int m_height;
	int m_picType;
	ffRGBA* m_data;
};

