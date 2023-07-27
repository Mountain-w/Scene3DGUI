#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in float intensity;
layout (location = 2) in vec4 aRGBA;

out vec2 outUV;
out vec4 outRGBA;

uniform mat4 _viewMatrix;
uniform mat4 _projMatrix;

void main()
{
   gl_Position = _projMatrix * _viewMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
   outRGBA = aRGBA;
};