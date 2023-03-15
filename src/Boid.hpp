#ifndef BOID_H
#define BOID_H

#include "glm/fwd.hpp"
#include "p6/p6.h"

class Boid {
private:
    glm::vec2 m_position;
    glm::vec2 m_vitesse;
    float     m_radius;
    float     m_top_limit;
    float     m_bottom_limit;
    float     m_left_limit;
    float     m_right_limit;

public:
    Boid();
    Boid(glm::vec2 position, glm::vec2 vitesse, float radius, float m_top_limit, float m_bottom_limit, float m_left_limit, float m_right_limit);
    // ~Boid();

    void                     draw(p6::Context& ctx);
    void                     position();
    bool                     borders_bool();
    static std::vector<Boid> create_boids(int num_boids, float top_limit, float bottom_limit, float left_limit, float right_limit);
};

#endif