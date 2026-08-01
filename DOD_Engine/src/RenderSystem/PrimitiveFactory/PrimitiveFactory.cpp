#include "PrimitiveFactory.h"

Mesh create_cilinder(int segment, float radius) {
    Mesh mesh;
    float centerX = 0.0f;
    float centerY = 0.0f;
    float centerZ = -1.0f;
    int segmentCircle = 50;
    std::vector<float> points;
    std::vector<unsigned int> indices;

    for(int j = 0; j < 50; ++j) {
        float angle1 = j * (2 * M_PI / segment);
        float angle2 = (j + 1) * (2 * M_PI / segment);
        points.push_back(centerX);
        points.push_back(centerY);
        points.push_back(centerZ);

        points.push_back(centerX + radius * std::cos(angle1));
        points.push_back(centerY + radius * std::sin(angle1));
        points.push_back(centerZ);

        points.push_back(centerX + radius * std::cos(angle2));
        points.push_back(centerY + radius * std::sin(angle2));
        points.push_back(centerZ);
    }

    for(int i = 0; i < segment - 2; ++i) {
        centerZ += 2.0f / segment;
        
        for(int j = 0; j < segmentCircle; ++j) {
            float angle = j * (2 * M_PI / segmentCircle);
            points.push_back(centerX + radius * std::cos(angle));
            points.push_back(centerY + radius * std::sin(angle));
            points.push_back(centerZ); 
        }
    }
    
    for(int j = 0; j < 50; ++j) {
        float angle1 = j * (2 * M_PI / segment);
        float angle2 = (j + 1) * (2 * M_PI / segment);
        points.push_back(centerX);
        points.push_back(centerY);
        points.push_back(centerZ);

        points.push_back(centerX + radius * std::cos(angle1));
        points.push_back(centerY + radius * std::sin(angle1));
        points.push_back(centerZ);

        points.push_back(centerX + radius * std::cos(angle2));
        points.push_back(centerY + radius * std::sin(angle2));
        points.push_back(centerZ);
    }

    size_t totalVertices = points.size() / 3;
    for(size_t i = 0; i < totalVertices; ++i) {
    
        if (i + segmentCircle >= totalVertices) break;

        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + segmentCircle);
        indices.push_back(i + segmentCircle);
        indices.push_back(i + 1);
        indices.push_back(i + 1 + segmentCircle);
    }

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(float), points.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * 4, indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    mesh.vertexCount = indices.size();

    return mesh;
}

Mesh create_square() {
    Mesh mesh;

    float vertex[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 
        -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 
    };

    GLuint indices[] = {
        0, 2, 4,
        4, 6, 0
    };
    mesh.vertexCount = 6;

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);

    // привязка буфера вершин
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    // привязка буфера индексов
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1); 
    // отвязка буферов вершин и индексов
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    // отвязка vao обязательно последним!
    glBindVertexArray(0);

    return mesh;
}

Mesh create_sphere(int segmentCircle, int segmentSphere, float radius) {
    Mesh mesh;
    float centerX = 0.0f;
    float centerY = 0.0f;
    float centerZ = 0.0f;
    float currentRadius;
    float currentZ;
    std::vector<Vertex> points;
    points.reserve((segmentCircle * segmentSphere) * 3);
    std::vector<unsigned int> indices;

    for(int i = 0; i < segmentSphere; ++i) {
        float ringAngle = -M_PI / 2.0f + i * (M_PI / segmentSphere);
        currentRadius = radius * std::cos(ringAngle);
        currentZ = radius * std::sin(ringAngle);

        for(int j = 0; j < segmentCircle; ++j) {
            float angle = j * (2 * M_PI / segmentCircle);
            Vertex vertex;
            vertex.position[0] = centerX + currentRadius * std::cos(angle);
            vertex.position[1] = centerY + currentRadius * std::sin(angle);
            vertex.position[2] = centerZ + currentZ;

            vertex.texCoords[0] = (float)j / (float)(segmentCircle - 1);
            vertex.texCoords[1] = (float)i / (float)(segmentSphere - 1);

            points.push_back(vertex);
        }
    }
    size_t totalVertices = points.size();
    for(size_t i = 0; i < totalVertices; ++i) {
    
        if (i + segmentCircle >= totalVertices) break;

        indices.push_back(i);
        indices.push_back(i + 1);
        indices.push_back(i + segmentCircle);
        indices.push_back(i + segmentCircle);
        indices.push_back(i + 1);
        indices.push_back(i + 1 + segmentCircle);
    }

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(Vertex), points.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * 4, indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    mesh.vertexCount = indices.size();

    return mesh;
}

Mesh create_circle(int segment, float radius) {
    Mesh mesh;
    float centerX = 0.0f;
    float centerY = 0.0f;
    std::vector<float> vertex;
    vertex.reserve(segment * 6);
    for(int j = 0; j < segment; ++j) {
        float angle1 = j * (2 * M_PI / segment);
        float angle2 = (j + 1) * (2 * M_PI / segment);
        vertex.push_back(centerX);
        vertex.push_back(centerY);

        vertex.push_back(centerX + radius * std::cos(angle1));
        vertex.push_back(centerY + radius * std::sin(angle1));

        vertex.push_back(centerX + radius * std::cos(angle2));
        vertex.push_back(centerY + radius * std::sin(angle2));
    }

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(float), vertex.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    mesh.vertexCount = vertex.size() / 2;

    return mesh;
}

Mesh create_cube() {
    Mesh mesh;

    float vertex[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f
    };

    unsigned int indices[] = {
        0, 1, 2, 2, 3, 0, // Задняя грань
        4, 5, 6, 6, 7, 4, // Передняя
        4, 7, 3, 3, 0, 4, // Левая
        1, 5, 6, 6, 2, 1, // Правая
        3, 2, 6, 6, 7, 3, // Верхняя
        4, 5, 1, 1, 0, 4  // Нижняя
    };
    mesh.vertexCount = 36;

    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBindVertexArray(0);
    return mesh;
}

Mesh create_triangle() {
    Mesh mesh;
    
    float vertex[] = {
        0.0f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };
    
    unsigned int indices[] = {0, 2, 4};

    mesh.vertexCount = 3;
    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    // Привязка буфера (Все последующие настройки запишутся внутрь этого VAO!)
    glBindVertexArray(mesh.vao);

    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBindVertexArray(0);
    return mesh;
}