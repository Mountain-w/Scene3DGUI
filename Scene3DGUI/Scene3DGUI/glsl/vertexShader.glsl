#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float intensity;
layout (location = 2) in vec4 aRGBA;

out vec2 outUV;
out vec4 outRGBA;

uniform mat4 _viewMatrix;
uniform mat4 _projMatrix;
uniform int _colorScheme;

void main()
{
   gl_Position = _projMatrix * _viewMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   if (_colorScheme == 0)
   {
		if (aPos.z < 0) outRGBA = vec4(255, 0, 255, 255);
		if (aPos.z < 1 && aPos.z >= 0) outRGBA = vec4(0, 0, 255, 255);
		if (aPos.z < 2 && aPos.z >= 1) outRGBA = vec4(0, 255, 255, 255);
		if (aPos.z < 3 && aPos.z >= 2) outRGBA = vec4(0, 255, 0, 255);
		if (aPos.z < 4 && aPos.z >= 3) outRGBA = vec4(255, 255, 0, 255);
		if (aPos.z >= 4) outRGBA = vec4(255, 0, 0, 255);
   }
   else if (_colorScheme == 1)
   {
		outRGBA = aRGBA;
   }
	   
};