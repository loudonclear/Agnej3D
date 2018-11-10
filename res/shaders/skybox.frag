#version 400

in vec2 uv;

out vec4 fragColor;

in vec3 texCoords;

uniform samplerCube skybox;

void main() {
    fragColor = texture(skybox, texCoords);
}

