#pragma once
#define _USE_MATH_DEFINES // Важно: строго до подключения библиотек!
#include <glad/glad.h>
#include "../../RenderTypes/renderTypes.h"
#include <cmath>

Mesh create_circle(int segment = 50, float radius = 0.5f);
Mesh create_cilinder(int segment = 50, float radius = 0.5f);
Mesh create_sphere(int segmentCircle = 50, int segmentSphere = 30, float radius = 1.0f);
Mesh create_triangle();
Mesh create_square();
Mesh create_cube();