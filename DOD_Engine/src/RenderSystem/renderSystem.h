#pragma once
#include <glad/glad.h>
#include "../Window/window.h"
#include "../RenderTypes/renderTypes.h"
#include "file_manager/file_readings.h"
#include "PrimitiveFactory/PrimitiveFactory.h"


void checkCompileErrors(GLuint shader, std::string type);

namespace render {
    ShaderProg create_shader(const char* vs, const char* fs);
    Texture load_texture(const char* filepath);
    namespace utils {
        MVP getLocMvp(ShaderProg shader); 
    }
}