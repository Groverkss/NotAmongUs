#include "WindowHandler.h"

void WindowHandler::framebufferSizeCallback(GLFWwindow *window,
                                            int width,
                                            int height) {
    /* Keep window size square */
    int minimumVar = std::min(width, height);
    glViewport(0, 0, minimumVar, minimumVar);
}

WindowHandler::WindowHandler(int width, int height, char *title) {
    /* Set Window hints */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Initialize GLFW window */
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);

    /* Initialize Glad */
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    /* Initial dimensions to OpenGL */
    glViewport(0, 0, width, height);

    /* Resize window on window size change */
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}