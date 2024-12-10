﻿#version 410 core

in float Height;

out vec4 FragColor;

void main()
{

    float h = (Height + 10.0f)/6.0f;

    FragColor = vec4(h/1.3f, h, h/1.9f, 1.0);
}