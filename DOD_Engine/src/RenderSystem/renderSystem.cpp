#include "renderSystem.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../include/stb_image.h"



void checkCompileErrors(GLuint shader, std::string type) {
    
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
        }
    }
}

ShaderProg render::create_shader(const char* vs, const char* fs) {
    
    ShaderProg shader;
    std::string vsr = read_file(vs);
    std::string fsr = read_file(fs);
    vs = vsr.c_str();
    fs = fsr.c_str();
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vs, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fs, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT");
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    checkCompileErrors(shaderProgram, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    shader.id_programm = shaderProgram;
    if (shader.id_programm == 0) {
        std::cout << "Критическая ошибка: Программа шейдеров не создана!\n";
        return shader;
    }
    std::cout << "Шейдер создан\n";
    return shader;
}

MVP render::utils::getLocMvp(ShaderProg shader) {
    MVP mvp;
    if (shader.id_programm == 0) {
        std::cout << "ОШИБКА: Шейдер пустой\n";
        return mvp;
    } 
    mvp.modelLoc = glGetUniformLocation(shader.id_programm, "model");
    mvp.viewLoc = glGetUniformLocation(shader.id_programm, "view");
    mvp.projectionLoc = glGetUniformLocation(shader.id_programm, "projection");
    return mvp;
}



Texture render::load_texture(const char* filepath) {
    Texture texture;
    glGenTextures(1, &texture.id_texture);

    int width, height, nrChannels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(filepath, &width, &height, &nrChannels, 0);

    if (data) {
        // Определяем формат в зависимости от каналов в картинке (RGB или RGBA для PNG)
        GLenum format = GL_RGB;
        if (nrChannels == 4) format = GL_RGBA;

        // 2. Привязываем текстуру для настройки
        glBindTexture(GL_TEXTURE_2D, texture.id_texture);

        // 3. Копируем массив байт из ОЗУ в память видеокарты (GPU)
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // Создаем уменьшенные копии для экономии памяти вдали

        // 4. Настраиваем фильтрацию (чтобы не было пиксельной каши)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 5. Обязательно освобождаем память на CPU, она больше не нужна!
        stbi_image_free(data);
    }
    else {
        std::cout << "Ошибка: Не удалось загрузить текстуру по пути: " << filepath << std::endl;
        stbi_image_free(data);
    }

    return texture;
}

