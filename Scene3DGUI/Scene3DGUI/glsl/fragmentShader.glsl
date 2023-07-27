#version 330 core
in vec4 outRGBA;

out vec4 FragColor;


void main()
{
    FragColor = outRGBA / 255.0f;
};