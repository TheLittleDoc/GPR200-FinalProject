#version 410 core
layout (quads, fractional_odd_spacing, ccw) in;

uniform sampler2D heightMap;  // the texture corresponding to our height map
uniform mat4 model;           // the model matrix
uniform mat4 view;            // the view matrix
uniform mat4 projection;      // the projection matrix

in vec2 TextureCoord[];

out float Height;

void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    vec2 t00 = TextureCoord[0];
    vec2 t01 = TextureCoord[1];
    vec2 t02 = TextureCoord[2];
    vec2 t03 = TextureCoord[3];

    vec2 t0 = (t01 - t00) * u + t00;
    vec2 t1 = (t03 - t02) * u + t02;
    vec2 texCoord = (t1 - t0) * v + t0;

    Height = texture(heightMap, texCoord).y * 64.0 - 16.0;

}