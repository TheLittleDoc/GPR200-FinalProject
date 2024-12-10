#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

out vec2 TexCoord;

void main()
{
    //passthrough shader. No vertex manipulation here :)
    gl_Position = vec4(aPos, 1.0);
    TexCoord = aTex;
}