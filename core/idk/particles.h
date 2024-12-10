#ifndef PARTICLES_H
#define PARTICLES_H

#include "../ew/external/glad.h"
#include <vector>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture2d.h"
//basis for this file aquired from opengl tutorial: https://learnopengl.com/In-Practice/2D-Game/Particles
//edits made by joshua scalia
// 
namespace idk
{
// Represents a single particle and its state
struct Particle {
    glm::vec3 Position, Velocity;
    glm::vec4 Color;
    float     Life;

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};


//This class makes the particles while using the previous struct (Particle)
class ParticleGenerator
{
public:
    //constructor
    ParticleGenerator(idk::Shader shader, idk::Texture2D texture, unsigned int amount);
    //update all particles' positions and data
    void Update(float dt, glm::vec3 spawnPos, unsigned int newParticles, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));
    //render all particles
    void Draw();
private:
    // state
    std::vector<Particle> particles;
    unsigned int amount;
    // render state
    idk::Shader shader;
    idk::Texture2D texture;
    unsigned int VAO;
    // initializes buffer and vertex attributes
    void init();
    // returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
    unsigned int firstUnusedParticle();
    // respawns particle, used in effect to keep the particles flowing.
    void respawnParticle(Particle& particle, glm::vec3 respawnPos , glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));
};
}


#endif