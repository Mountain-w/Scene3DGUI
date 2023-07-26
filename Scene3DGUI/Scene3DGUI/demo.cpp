//#include "base.h"
//
//void framebuffer_size_callback_1(GLFWwindow* window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//}
//
//void loadShader()
//{
//    const char* _vertexPath = "Scene3DGUI.vertexShader.glsl";
//    const char* _fragPath = "Scene3DGUI.fragmentShader.glsl";
//
//    std::string _vertexCode("");
//    std::string _fragCode("");
//
//    std::ifstream _vShaderFile;
//    std::ifstream _fShaderFile;
//
//    //����try catch����չ����
//    _vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//    _fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//
//    try
//    {
//        _vShaderFile.open(_vertexPath);
//        _fShaderFile.open(_fragPath);
//
//        std::stringstream _vShaderStream, _fShaderStream;
//        _vShaderStream << _vShaderFile.rdbuf();
//        _fShaderStream << _fShaderFile.rdbuf();
//
//        _vShaderFile.close();
//        _fShaderFile.close();
//
//        _vertexCode = _vShaderStream.str();
//        _fragCode = _fShaderStream.str();
//    }
//    catch (std::ifstream::failure e)
//    {
//        std::string errStr = "ffShader constructor fail to read filePath";
//        std::cout << errStr << std::endl;
//        return;
//    }
//
//    const char* _vShaderStr = _vertexCode.c_str();
//    const char* _fShaderStr = _fragCode.c_str();
//
//    std::cout << _vShaderStr << std::endl;
//}
//
//int main1()
//{
//    loadShader();
//    // ��ʼ��GLFW
//    glfwInit();
//    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui Example", NULL, NULL);
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_1);
//
//
//    // ��ʼ��GLAD
//    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
//
//    // ��ʼ��ImGui
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGui_ImplGlfw_InitForOpenGL(window, true);
//    ImGui_ImplOpenGL3_Init("#version 330");
//
//    // ��Ⱦѭ��
//    while (!glfwWindowShouldClose(window))
//    {
//        glfwPollEvents();
//
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplGlfw_NewFrame();
//        ImGui::NewFrame();
//
//        // ��ʼһ��ImGui����
//        ImGui::Begin("Hello, ImGui!");
//
//        // ���һ����ǩ
//        ImGui::Text("Hello, World!");
//
//        // ���һ����ť
//        if (ImGui::Button("Click Me"))
//        {
//            // ����ť�����ʱִ�еĴ���
//            // �����������߼�
//            std::cout << "click" << std::endl;
//        }
//
//        // ����ImGui����
//        ImGui::End();
//
//        // ��ȾImGui����
//        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//        ImGui::Render();
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//        glfwSwapBuffers(window);
//    }
//
//    // ����
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();
//
//    glfwDestroyWindow(window);
//    glfwTerminate();
//
//    return 0;
//}