#include <iostream>
#include "base.h"
#include "Shader.h"
#include "ffImage.h"
#include "Camera.h"
#include "Status.h"
#include "PointCloudService.h"

Shader _shader;
Camera _camera;
Status _status;
glm::mat4 _projMatrix(1.0f);
int _width = 1920;
int _height = 1080;
float pointSize = 1.0f;
const char* pcdPath = R"(D:\SVN\MindFlow\需求列表\20230721-717\output\data\2023-04-20-06-22-09\global_map.pcd)";
//const char* pcdPath = R"(D:\pld\HZ\input\合众标注2023.02.28\localmap\1677121714733085184.pcd)";

// 渲染
void rend(PointCloudInfo &_info)
{
    glPointSize(pointSize);
    _camera.update();
    _projMatrix = glm::perspective(glm::radians(45.0f), (float)_width / (float)_height, 0.1f, 500.0f);
    _shader.start();
    _shader.setMatrix("_viewMatrix", _camera.getMatrix());
    _shader.setMatrix("_projMatrix", _projMatrix);
    glBindVertexArray(_info.VAO);
    glDrawArrays(GL_POINTS, 0, _info.pointNum);
    _shader.end();

}

// 事件监听
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        _camera.move(CAMERA_MOVE::MOVE_UP);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        _camera.move(CAMERA_MOVE::MOVE_DOWN);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        _camera.move(CAMERA_MOVE::MOVE_LEFT);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        _camera.move(CAMERA_MOVE::MOVE_RIGHT);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        pointSize += 0.5;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        if (pointSize == 0)
        {
            return;
        }
        pointSize -= 0.5;
    }
}

void mouse_scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    if (yOffset > 0.0f)
    {
        _camera.move(CAMERA_MOVE::MOVE_FRONT);
    }
    else
    {
        _camera.move(CAMERA_MOVE::MOVE_BACK);
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    glm::vec2 _difference = _status.difference(xpos, ypos);
    switch (_status.curMode())
    {
    case MOUSESTATUS::LEFTCLICKED:
        std::cout << "差值计算，x: " << _difference.x << ", y: " << _difference.y << std::endl;
        _camera.onMouseMove(_difference.x, _difference.y);
        break;
    case MOUSESTATUS::MIDDLECLICKED:

        break;
    case MOUSESTATUS::RIGHTCLICKED:
        break;
    default:
        break;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    MOUSESTATUS _clickMode = MOUSESTATUS::NOTHING;
    MOUSESTATUS _releaseMode = MOUSESTATUS::NOTHING;
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        _clickMode = MOUSESTATUS::LEFTCLICKED;
        _releaseMode = MOUSESTATUS::LEFTRELEASE;
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        _clickMode = MOUSESTATUS::MIDDLECLICKED;
        _releaseMode = MOUSESTATUS::MIDDLERELEASE;
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        _clickMode = MOUSESTATUS::RIGHTCLICKED;
        _releaseMode = MOUSESTATUS::RIGHTRELEASE;
        break;
    default:
        break;
    }
    if (action == GLFW_PRESS)
    {
        double _xPos = 0.0f;
        double _yPos = 0.0f;
        glfwGetCursorPos(window, &_xPos, &_yPos);
        _status.activate(_clickMode, _xPos, _yPos);
    }
    else if (action == GLFW_RELEASE)
    {
        _status.release(_releaseMode);
    }
}

int main()
{
    // 初始化 glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    GLFWwindow* window = glfwCreateWindow(_width, _height, "Scene3D", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    _camera.setSpeed(1.0f);
    _shader.initShader(R"(glsl\vertexShader.glsl)", R"(glsl\fragmentShader.glsl)");
    PointCloudInfo info;
    PointCloudService::loadPcd(pcdPath, info);
    std::cout << "读取到：" << info.pointNum << " 个点" << std::endl;

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, _width, _height);

        rend(info);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
