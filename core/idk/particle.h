//
// Created by elysium.hosack on 11/15/2024.
//
#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>
#include <glm/ext.hpp>
namespace idk {
    class particle {
        glm::vec2 position;
        glm::vec2 velocity;
        glm::vec4 color;
        float size;
        float life;

    public:
        particle() : position(0.0f), velocity(0.0f), color(1.0f), size(1.0f), life(0.0f) {}

        glm::vec2 getPosition() { return position; }
        glm::vec2 getVelocity() { return velocity; }
        glm::vec4 getColor() { return color; }
        float getSize() { return size; }
        float getLife() { return life; }


    };
}


#endif //PARTICLE_H
