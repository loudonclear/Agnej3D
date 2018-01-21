#version 330 core
out vec4 fragColor;

uniform vec3 color;

uniform sampler2D tex;
uniform int useTexture = 0;

in vec4 normal_worldSpace;
in vec4 position_worldSpace;
in vec4 eye_worldSpace;
in vec2 texc;

void main(){
    vec3 base_color = vec3(0);
    fragColor = vec4(0);
    if(useTexture > 0){
        base_color = clamp(texture(tex,texc).xyz,0,1);
    }else{
        base_color = color;
    }
    vec4 vertexToLight = normalize(vec4(1,1,1,0));
    float diffuseIntensity = max(0.0, dot(vertexToLight, normal_worldSpace));
    fragColor += vec4(max(vec3(0), vec3(1,1,1) * base_color * diffuseIntensity),0.f);

    // Add specular component
    vec4 lightReflection = normalize(-reflect(vertexToLight, normal_worldSpace));
    vec4 eyeDirection = normalize(eye_worldSpace - position_worldSpace);
    float specIntensity = pow(max(0.0, dot(eyeDirection, lightReflection)), 150);
    fragColor += vec4(max(vec3(0), vec3(1,1,1) * base_color * specIntensity),0.f);

    fragColor += vec4(base_color*.1,0.f);
}
