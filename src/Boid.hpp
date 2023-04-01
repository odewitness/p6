#ifndef BOID_H
#define BOID_H

#include <cmath>
#include "config.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

class Boid {
private:
    glm::vec2 m_position;
    glm::vec2 m_direction;
    float     m_vitesse;
    float     m_taille;
    float     m_top_limit;
    float     m_bottom_limit;
    float     m_left_limit;
    float     m_right_limit;

public:
    Boid();
    Boid(glm::vec2 position, glm::vec2 direction, float vitesse, float taille, float m_top_limit, float m_bottom_limit, float m_left_limit, float m_right_limit);

    void                     draw(p6::Context& ctx);
    void                     movement();
    bool                     borders_bool(); // todo erommer pour rque ce soit clair si ça nous dutdit qu'on est dehor ou dedans
    void                     avoid_walls(const glm::vec2& min_window_size, const glm::vec2& max_window_size, const float& wall_distance);
    static std::vector<Boid> create_boids(int num_boids, float top_limit, float bottom_limit, float left_limit, float right_limit);
    void                     cohesion(std::vector<Boid>& boids, const float& cohesion_distance, const float& cohesion_force);
    void                     alignment(std::vector<Boid>& boids, const float& alignment_distance, const float& alignment_force);
};

#endif