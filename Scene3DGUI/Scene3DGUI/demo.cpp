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
//    //增加try catch的扩展功能
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
//    // 初始化GLFW
//    glfwInit();
//    GLFWwindow* window = glfwCreateWindow(800, 600, "ImGui Example", NULL, NULL);
//    glfwMakeContextCurrent(window);
//    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback_1);
//
//
//    // 初始化GLAD
//    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
//
//    // 初始化ImGui
//    IMGUI_CHECKVERSION();
//    ImGui::CreateContext();
//    ImGui_ImplGlfw_InitForOpenGL(window, true);
//    ImGui_ImplOpenGL3_Init("#version 330");
//
//    // 渲染循环
//    while (!glfwWindowShouldClose(window))
//    {
//        glfwPollEvents();
//
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplGlfw_NewFrame();
//        ImGui::NewFrame();
//
//        // 开始一个ImGui窗口
//        ImGui::Begin("Hello, ImGui!");
//
//        // 添加一个标签
//        ImGui::Text("Hello, World!");
//
//        // 添加一个按钮
//        if (ImGui::Button("Click Me"))
//        {
//            // 当按钮被点击时执行的代码
//            // 可以添加你的逻辑
//            std::cout << "click" << std::endl;
//        }
//
//        // 结束ImGui窗口
//        ImGui::End();
//
//        // 渲染ImGui绘制
//        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//        ImGui::Render();
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//
//        glfwSwapBuffers(window);
//    }
//
//    // 清理
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();
//
//    glfwDestroyWindow(window);
//    glfwTerminate();
//
//    return 0;
//}