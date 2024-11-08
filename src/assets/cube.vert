#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
in vec4 aColor;

out vec4 vertexColor;
out vec2 textureCoords;
out vec3 normal;
out vec3 FragPos;

uniform float uTime;
uniform float uSpeed;
uniform float uHeight;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // /* tan color */  vec4 colorT = vec4(aColor.x + abs(tan(uTime)/2.0), aColor.y + abs(tan((uTime) + (aPos.x * 2)))/2.0, aColor.z + abs(tan((uTime) + (aPos.y * 2)))/2.0, 1);
    /* static color */  vec4 colorT = vec4(aColor.x, aColor.y, aColor.z, 1);
    // /* mime pos */   vec3 aPosT = vec3(aPos.x + abs(sin(uTime * uSpeed))/6.0, aPos.y + abs(cos((uTime * uSpeed) + (aPos.x * 2)))/6.0, 0);
    ///* bounce pos */    vec3 aPosT = vec3(aPos.x, aPos.y + (abs(sin(uTime * uSpeed))/20.0) * uHeight, 0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vertexColor = aColor * colorT;

    textureCoords = aTexCoords;
    normal = aNormal;

    FragPos = vec3(model * vec4(aPos, 1.0));

}