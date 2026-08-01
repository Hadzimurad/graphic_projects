#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <cstdint>
#include <cmath>

struct Vertex {
    glm::vec3 position;
    glm::vec2 texCoords;
};


struct Texture {
    uint32_t id_texture = 0;
};
struct ShaderProg {
    uint32_t id_programm = 0;
};

struct MVP {
    uint32_t modelLoc = 0;
    uint32_t viewLoc = 0;
    uint32_t projectionLoc = 0;
    uint32_t padding = 0;
};

struct Mesh {
    uint32_t vao = 0;
    uint32_t vbo = 0;
    uint32_t ebo = 0;
    uint32_t vertexCount = 0;
};

struct MeshContainer {
    std::vector<Mesh> meshCont;
    std::vector<ShaderProg> shaderCont;
    std::vector<glm::vec3> positions;
    std::vector<MVP> mvp;
    std::vector<Texture> textureCont;
};