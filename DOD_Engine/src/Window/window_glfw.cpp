#include "window.h"

float yaw   = -90.0f;
float pitch =   0.0f;
float lastX = 400.0f;
float lastY = 300.0f;
bool firstMouse = true;
float sensitivity = 0.1f;
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);

Window::Window(float height, float width, std::string title, int v1, int v2) :
    H(height), W(width) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, v1);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, v2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    window = glfwCreateWindow(height, width, title.c_str(), NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

void Window::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    // При первом входе в окно фиксируем координаты, чтобы избежать рывка
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    // Считаем смещение мыши относительно предыдущего кадра
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Инвертируем, так как в GLFW Y-координаты идут сверху вниз

    lastX = xpos;
    lastY = ypos;

    // Применяем чувствительность
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    // Изменяем углы поворота
    yaw   += xoffset;
    pitch += yoffset;

    // Ограничиваем угол Pitch, чтобы камера не переворачивалась "вверх ногами"
    if (pitch > 89.0f)  pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    
    camera_front = glm::normalize(front);

}

void Window::wait_key(glm::vec3* pos) {
    float speed = 0.1f;
    glm::vec3 camera_right = glm::normalize(glm::cross(camera_front, glm::vec3(0, 1, 0)));

    if (glfwGetKey(window, GLFW_KEY_W)) *pos += camera_front * speed;
    if (glfwGetKey(window, GLFW_KEY_S)) *pos -= camera_front * speed;
    if (glfwGetKey(window, GLFW_KEY_D)) *pos += camera_right * speed;
    if (glfwGetKey(window, GLFW_KEY_A)) *pos -= camera_right * speed;
}

void Window::Swap() {
    glfwSwapBuffers(window);
}
void Window::PollEvents() {
    glfwPollEvents();
}
Window::~Window() {
    glfwTerminate();
}
