#version 400

layout (location = 0) in vec3 position;

uniform mat4 lightSpaceMatrix;
uniform mat4 m;

void main()
{
    gl_Position = lightSpaceMatrix * m * vec4(position, 1.0);
}
