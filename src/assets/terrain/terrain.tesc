﻿/******************************************************************************
* File name: terrain.tesc   File author(s): Jeffrey Paone (LearnOpenGL)       *
*                                           Mod. by Elysium Hosack            *
* File description:                                                           *
* Sets up patches and determines how much tessellation to apply               *
*                                                                             *
* Cloned: 12/3/2024         Edited last: 12/09/2024                           *
* Key Methods: N/A                                                            *
******************************************************************************/

#version 410 core
layout (vertices = 4) out;

uniform mat4 model;
uniform mat4 view;

uniform int uMinTessellation;
uniform int uMaxTessellation;
uniform float uMinDistance;
uniform float uMaxDistance;

in vec2 TexCoord[];
out vec2 TextureCoord[];

void main() {
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
    TextureCoord[gl_InvocationID] = TexCoord[gl_InvocationID];

    if(gl_InvocationID == 0)
    {
        // changed these to uniforms for the ability to manipulate with ImGui
        int MIN_TESS_LEVEL = uMinTessellation;
        int MAX_TESS_LEVEL = uMaxTessellation;
        float MIN_DISTANCE = uMinDistance;
        float MAX_DISTANCE = uMaxDistance;

        vec4 eyeSpacePos00 = view * model * gl_in[0].gl_Position;
        vec4 eyeSpacePos01 = view * model * gl_in[1].gl_Position;
        vec4 eyeSpacePos10 = view * model * gl_in[2].gl_Position;
        vec4 eyeSpacePos11 = view * model * gl_in[3].gl_Position;

        // "distance" from camera scaled between 0 and 1
        float distance00 = clamp( (abs(eyeSpacePos00.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );
        float distance01 = clamp( (abs(eyeSpacePos01.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );
        float distance10 = clamp( (abs(eyeSpacePos10.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );
        float distance11 = clamp( (abs(eyeSpacePos11.z) - MIN_DISTANCE) / (MAX_DISTANCE-MIN_DISTANCE), 0.0, 1.0 );

        // tessellation level is interpolated across the range
        float tessLevel0 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance10, distance00) );
        float tessLevel1 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance00, distance01) );
        float tessLevel2 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance01, distance11) );
        float tessLevel3 = mix( MAX_TESS_LEVEL, MIN_TESS_LEVEL, min(distance11, distance10) );

        gl_TessLevelOuter[0] = tessLevel0;
        gl_TessLevelOuter[1] = tessLevel1;
        gl_TessLevelOuter[2] = tessLevel2;
        gl_TessLevelOuter[3] = tessLevel3;

        gl_TessLevelInner[0] = max(tessLevel1, tessLevel3);
        gl_TessLevelInner[1] = max(tessLevel0, tessLevel2);
    }
}