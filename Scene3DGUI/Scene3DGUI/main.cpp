#include <iostream>
#include "base.h"
#include "Shader.h"
#include "ffImage.h"
#include "Camera.h"
#include "Status.h"
#include "PointCloudService.h"
#include "Geometry.h"
#include <Windows.h>

Shader _shader;
Camera _camera;
SimpleCamera _topCamera{1};
SimpleCamera _leftCamera{1};
SimpleCamera _frontCamera{1};

Status _status;
glm::mat4 _projMatrix(1.0f);
int _width = 1920;
int _height = 1080;
float pointSize = 1.0f;
bool altPressed = false;
bool change = false;
const char* pcdPath = R"(C:\Users\47896\Desktop\20230703154911.000087_BlindLidar01.pcd)";
//const char* pcdPath = R"(D:\workplace\scene_001.pcd)";
//const char* pcdPath = R"(D:\pld\HZ\bev_lane_base_reconstruction_1th_for_82_sample\data\2023-04-20-06-22-09\2023-04-20-06-22-09\global_map.pcd)";
Eigen::RowVector3f pos(2.0f, 2.0f, 2.0f);
Eigen::RowVector3f size(4.0f, 4.0f, 4.0f);
Eigen::RowVector3f euler(0.0f, 0.0f, glm::radians(40.0f));
Geometry::AABB aabb(pos, size);
Geometry::OOBB oobb(pos, size, euler);


// 渲染
void rend(PointCloudInfo &_info)
{
    glPointSize(pointSize);
    _camera.update();
    _projMatrix = glm::perspective(glm::radians(45.0f), (float)_width / (float)_height, 0.1f, 500.0f);
    _shader.start();
    _shader.setMatrix("_viewMatrix", _camera.getMatrix());
    _shader.setMatrix("_projMatrix", _projMatrix);
    _shader.setI("_colorScheme", 0);
    glBindVertexArray(_info.VAO);
    glDrawArrays(GL_POINTS, 0, _info.pointNum);
    _shader.end();
}

// 渲染3D框
void rend2(Geometry::OOBB &oobb)
{
    glPointSize(pointSize);
    _camera.update();
    _projMatrix = glm::perspective(glm::radians(45.0f), (float)_width / (float)_height, 0.1f, 500.0f);
    _shader.start();
    _shader.setMatrix("_viewMatrix", _camera.getMatrix());
    _shader.setMatrix("_projMatrix", _projMatrix);
    _shader.setI("_colorScheme", 1);
    oobb.update();
    glBindVertexArray(oobb.vao);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_LINES, 34, GL_UNSIGNED_INT, 0);
    _shader.end();

}

// 渲染视图
void rendView(SimpleCamera &camera_, PointCloudInfo &_info, Geometry::OOBB &oobb, int view, float width, float height)
{
    if (view == 0){camera_.flash(oobb.getCenter(), oobb.getFront()); }
    else if (view == 1){camera_.flash(oobb.getCenter(), oobb.getTop()); }
    else if (view == 2) { camera_.flash(oobb.getCenter(), oobb.getLeft()); }

    
    //_projMatrix = glm::perspective(glm::radians(45.0f), (float)_width / (float)_height, 0.1f, 500.0f);
    float ratio = width / height;
    _projMatrix = glm::ortho(-ratio * 3, ratio * 3, -1.0f * 3, 1.0f * 3, -1.0f, 100.0f);
    _shader.start();
    _shader.setMatrix("_viewMatrix", camera_.getMatrix());
    _shader.setMatrix("_projMatrix", _projMatrix);
    _shader.setI("_colorScheme", 0);
    glBindVertexArray(_info.VAO);
    glDrawArrays(GL_POINTS, 0, _info.pointNum);
    _shader.setI("_colorScheme", 1);
    oobb.update();
    glBindVertexArray(oobb.vao);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_LINES, 34, GL_UNSIGNED_INT, 0);
    _shader.end();
}

// 事件监听
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// 摄像机移动
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
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

// 3D框移动
void processInput2(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        oobb.moveX(1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        oobb.moveX(-1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        oobb.moveY(1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        oobb.moveY(-1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        oobb.resize(Geometry::RESIZETYPE::UP, 0.03);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        oobb.resize(Geometry::RESIZETYPE::DOWN, -0.03);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        oobb.resize(Geometry::RESIZETYPE::LEFT, 0.03);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        oobb.resize(Geometry::RESIZETYPE::RIGHT, -0.03);
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        oobb.undo();
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        altPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE)
    {
        altPressed = false;
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
        if (altPressed) _camera.onMouseMove(_difference.x, _difference.y);
        else oobb.roll((float)_difference.x);
        break;
    case MOUSESTATUS::MIDDLECLICKED:
        if (altPressed) _camera.move((float)_difference.x, (float)_difference.y);
        else oobb.pitch((float)_difference.x);
        break;
    case MOUSESTATUS::RIGHTCLICKED:
        oobb.yaw((float)_difference.x);
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

int pcdVis()
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

int testImGui()
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext(NULL);
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->AddFontFromFileTTF(R"(fronts\kaiu.ttf)", 18, NULL, io.Fonts->GetGlyphRangesChineseFull());


    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiViewportFlags_NoDecoration;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigFlags |= ImGuiCol_DockingEmptyBg;

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.17f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.37f, 0.36f, 0.36f, 102.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.10f, 0.10f, 0.10f, 171.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.20f, 0.20f, 0.20f, 255.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.64f, 0.64f, 0.64f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.22f, 0.22f, 0.22f, 0.40f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.45f, 0.45f, 0.45f, 0.31f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.55f, 0.55f, 0.55f, 0.80f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.46f, 0.46f, 0.46f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.17f, 0.17f, 0.17f, 0.95f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.50f, 0.50f, 0.50f, 0.78f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.45f, 0.45f, 0.45f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.51f, 0.51f, 0.51f, 0.70f);
    colors[ImGuiCol_Tab] = ImVec4(0.21f, 0.21f, 0.21f, 0.86f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.15f, 0.15f, 0.97f);
    colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.40f, 0.13f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.45f, 1.00f, 0.85f, 0.35f);


    style.WindowRounding = 4;
    style.FrameRounding = 4;
    style.GrabRounding = 3;
    style.ScrollbarSize = 7;
    style.ScrollbarRounding = 0;


    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, _width, _height);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::DockSpaceOverViewport();

        ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
    
}
void renderMenu(std::string &pcdPath)
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImVec2 menuBarPos = viewport->Pos;
    ImVec2 menuBarSize = ImVec2(viewport->Size.x, 30.0f);

    ImGui::SetNextWindowPos(menuBarPos);
    ImGui::SetNextWindowSize(menuBarSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::Begin("MenuBar", nullptr,
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar);
    
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N")) {}
            if (ImGui::MenuItem("Open", "Ctrl+O")) {
                OPENFILENAMEA ofn;
                CHAR szFile[MAX_PATH] = { 0 };

                ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = NULL;
                ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
                ofn.lpstrFile = szFile;
                ofn.nMaxFile = sizeof(szFile);
                ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
                
                if (GetOpenFileNameA(&ofn))
                {
                    pcdPath = ofn.lpstrFile;
                    change = true;
                }
            }
            if (ImGui::MenuItem("Save", "Ctrl+S")) {}
            if (ImGui::MenuItem("Exit", "Alt+F4")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
            if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
            if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();
    ImGui::PopStyleVar(2);
}
int testMoveObj()
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
    glEnable(GL_DEPTH_TEST);

    // 初始化Dear ImGui
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    ImGuiStyle& style = ImGui::GetStyle();
    style.FramePadding = ImVec2(10, 10);
    style.ItemSpacing = ImVec2(10, 5);


    _camera.setSpeed(1.0f);
    _topCamera.setUp(glm::vec3(0.0f, 1.0f, 0.0f));
    _shader.initShader(R"(glsl\vertexShader.glsl)", R"(glsl\fragmentShader.glsl)");
    oobb.setColor(255, 255, 0, 255);
    oobb.update();
    
    PointCloudInfo info;
    std::string _pcdPath = "";
    /*PointCloudService::loadPcd(pcdPath, info);
    std::cout << "读取到：" << info.pointNum << " 个点" << std::endl;*/
    
    int curWidth, curHeight;
    int xpos, ypos;
    bool loadPath = false;
    while (!glfwWindowShouldClose(window))
    {
        glfwGetWindowSize(window, &curWidth, &curHeight);
        glfwGetWindowPos(window, &xpos, &ypos);
        processInput2(window);
        // 渲染UI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // 菜单栏
        renderMenu(_pcdPath);

        ImGui::SetNextWindowPos(ImVec2(0, 30));
        ImGui::SetNextWindowSize(ImVec2(curWidth / 4, curHeight));
        ImGui::Begin("Tools", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
        if (ImGui::CollapsingHeader("tool1"))
        {
            ImGui::Button("3D框");
        }
        if (ImGui::CollapsingHeader("tool2"))
        {
            ImGui::Button("load...");
        }
        ImGui::End();
        //ImGui::Begin("Bottom");
        //ImGui::End();
        if (change)
        {
            loadPath = true;
            PointCloudService::loadPcd(_pcdPath.c_str(), info);
            std::cout << "读取到：" << info.pointNum << " 个点" << std::endl;
            change = false;
            loadPath = true;
        }
        if (loadPath)
        {
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glViewport(curWidth / 4, curHeight / 16, curWidth / 4, curHeight * 3 / 16);
            rendView(_topCamera, info, oobb, 1, curWidth / 4, curHeight * 3 / 16);
            glViewport(curWidth / 2, curHeight / 16, curWidth / 4, curHeight * 3 / 16);
            rendView(_leftCamera, info, oobb, 2, curWidth / 4, curHeight * 3 / 16);
            glViewport(curWidth * 3 / 4, curHeight / 16, curWidth / 4, curHeight * 3 / 16);
            rendView(_frontCamera, info, oobb, 0, curWidth / 4, curHeight * 3 / 16);
            glViewport(curWidth / 4, curHeight / 4, curWidth * 3 / 4, curHeight * 3 / 4);
            rend(info);
            glLineWidth(2.0f);
            rend2(oobb);
        }
        

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // 清理并关闭窗口
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void RenderUI()
{
    if (ImGui::Button("Open File"))
    {
        OPENFILENAMEA ofn;
        CHAR szFile[MAX_PATH] = { 0 };

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL;
        ofn.lpstrFilter = "All Files (*.*)\0*.*\0";
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

        if (GetOpenFileNameA(&ofn))
        {
            printf("Selected file: %s", ofn.lpstrFile);
        }
    }
}

int testFileDialog()
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

    // 初始化Dear ImGui
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    // 主循环
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // 渲染UI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        RenderUI();

        ImGuiIO& io = ImGui::GetIO();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // 清理并关闭窗口
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}


int main()
{
    //testImGui();
    testMoveObj();
    //testFileDialog();
    //testScene();
    return 0;
}