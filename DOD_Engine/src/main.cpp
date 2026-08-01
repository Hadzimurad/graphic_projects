#include "RenderSystem/renderSystem.h"
#include <glm/gtc/type_ptr.hpp>


int main() {
    MeshContainer meshObject;
    // 1. Инициализация GLFW 
    Window main_window(800.0f, 600.0f, "Square rotation");
    // 2. Инициализация Вершинного и Фрагментного шейдера
    ShaderProg shader = render::create_shader("vertex.glsl", "fragment.glsl");
    // 3. Поиск локаций переменных в шейдере
    MVP mvp = render::utils::getLocMvp(shader);
    // 4. Инициализация структуры моделей
    /*meshObject.textureCont.push_back(render::load_texture("../textures/earth.jpg"));
    meshObject.textureCont.push_back(render::load_texture("../textures/jupiter.jpg"));*/

    int count = 1;

    for(int i = 0; i < count; ++i) {
        meshObject.meshCont.push_back(create_sphere()); 
        meshObject.shaderCont.push_back(shader);
        meshObject.mvp.push_back(mvp);
        meshObject.positions.push_back(glm::vec3(-3.0f + i * 3, 0.0f, 0.0f));
        meshObject.textureCont.push_back(render::load_texture("../textures/mars.jpg"));
    }
    

    glm::vec3 camera_pos(0.0f, 0.0f, 0.0f);
    float rotation_angle = 0.0f;
    glm::vec3 entity_pos;
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    float lastTime = (float)glfwGetTime();
    
    // --- ЦИКЛ РЕНДЕРА ---
    while (!glfwWindowShouldClose(main_window.window)) {
        float currentTime = (float)glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime; 

        glfwSetInputMode(main_window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
        glfwSetCursorPosCallback(main_window.window, Window::mouse_callback);

        main_window.wait_key(&camera_pos);
        glm::mat4 view = glm::lookAt(camera_pos, camera_pos + camera_front, glm::vec3(0, 1, 0));
        
        rotation_angle += 1.0f * deltaTime;

        glm::mat4 R = glm::rotate(glm::mat4(1.0f), rotation_angle, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 R1 = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        R *= R1;
        glm::mat4 S = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        for(int i = 0; i < meshObject.meshCont.size(); ++i) { 
            glm::mat4 T = glm::translate(glm::mat4(1.0f), meshObject.positions[i]);
            
            glm::mat4 model = T * R * S;

            glUseProgram(meshObject.shaderCont[i].id_programm);

            // 1. Делаем активным текстурный юнит 0
            glActiveTexture(GL_TEXTURE0); 
            // 2. Привязываем текстуру текущего (i-го) объекта к этому юниту
            glBindTexture(GL_TEXTURE_2D, meshObject.textureCont[i].id_texture);

            glUniformMatrix4fv(meshObject.mvp[i].modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniformMatrix4fv(meshObject.mvp[i].viewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(meshObject.mvp[i].projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

            glBindVertexArray(meshObject.meshCont[i].vao);
            glDrawElements(GL_TRIANGLES, meshObject.meshCont[i].vertexCount, GL_UNSIGNED_INT, 0);
            
            //glDrawArrays(GL_TRIANGLES, 0, meshObject.meshCont.at(i).vertexCount);
            glBindVertexArray(0);
        }
        
        main_window.Swap();
        main_window.PollEvents();
    }
}
