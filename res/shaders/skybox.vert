#version 400


layout(location = 0) in vec3 position;

out vec3 texCoords;

uniform mat4 p;
uniform mat4 v;

void main() {
    texCoords = position;
    gl_Position = p * v * vec4(position, 1.0);
}
