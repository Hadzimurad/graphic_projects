#version 450 core
in vec2 texCoords;
out vec4 FragColor;

uniform sampler2D textureid;

void main() { FragColor = texture(textureid, texCoords); }