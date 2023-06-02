#pragma once

#include "glm/fwd.hpp"
#include "p6/p6.h"

struct BoidProperties {
    glm::vec3 position;
    glm::vec3 direction;
    float     speed;
    float     size;
    float     upper_limit;
    float     lower_limit;
    float     left_limit;
    float     right_limit;
    float     foreground_limit;
    float     background_limit;
    glm::vec3 color;
};

class Boid {
private:
    glm::vec3      m_direction;
    float          m_speed;
    float          m_size;
    float          m_upper_limit;
    float          m_lower_limit;
    float          m_left_limit;
    float          m_right_limit;
    float          m_foreground_limit;
    float          m_background_limit;
    float          m_radius;
    glm::vec3      m_color;
    BoidProperties m_properties;

public:
    Boid();
    explicit Boid(const BoidProperties& properties);

    void update(const float& size_boid, p6::Context& ctx, std::vector<Boid>& boids, const float& cohesion_radius, const float& cohesion_strength, const float& alignment_radius, const float& alignment_strength, const float& separation_radius, const float& separation_strength);

    glm::vec3 get_position() const;

private:
    void set_size(float size);

    void separation(std::vector<Boid>& boids, const float& separation_radius, const float& separation_strength);
    void cohesion(std::vector<Boid>& boids, const float& cohesion_radius, const float& cohesion_strength);
    void alignment(std::vector<Boid>& boids, const float& alignment_radius, const float& alignment_strength);

    bool bounce_if_out_of_limits();
    void movement();
};

std::vector<Boid> creation_boids(int num_boids, float upper_limit, float lower_limit, float left_limit, float right_limit, float size_boid, float foreground_limit, float background_limit);