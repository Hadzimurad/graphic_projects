#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>

extern float yaw;
extern float pitch;
extern float lastX;
extern float lastY;
extern bool firstMouse;
extern float sensitivity;
extern glm::vec3 camera_front;

class Window {
public:
    float H;
    float W;
    GLFWwindow* window;

    Window(float height, float width, std::string title, int v1 = 4, int v2 = 5);
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
    
    void wait_key(glm::vec3* pos);
    
    void Swap(); 
    void PollEvents();
    ~Window();
};