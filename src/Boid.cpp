#include "Boid.hpp"

// Boid::Boid(glm::vec2 position, glm::vec2 vitesse, float radius)
//     : m_position(glm::vec2(position)), m_vitesse(glm::vec2(vitesse)), m_radius(radius) {}

Boid::Boid(glm::vec2 position, glm::vec2 vitesse, float radius, float top_limit, float bottom_limit, float left_limit, float right_limit)
    : m_position(glm::vec2(position)), m_vitesse(glm::vec2(vitesse)), m_radius(radius), m_top_limit(top_limit), m_bottom_limit(bottom_limit), m_left_limit(left_limit), m_right_limit(right_limit) {}

// Boid::Boid()
//     : m_position(glm::vec2(0, 0)), m_vitesse(glm::vec2(0.1, 0.1)), m_radius(0.2) {}

// Boid::Boid() {
//     m_position(glm::vec2(0, 0)), m_vitesse(glm::vec2(0.1, 0.1)), m_radius(0.2) {}
// }

void Boid::draw(p6::Context& ctx)
{
    ctx.square(p6::Center{m_position}, p6::Radius{m_radius}, p6::Rotation{0.0_radians});
}

void Boid::position()
{
    m_position += m_vitesse;
}

bool Boid::borders_bool()
{
    bool out_of_border = true;
    if (m_position.y > m_top_limit - m_radius)
    {
        m_position.y  = m_top_limit - m_radius;
        m_vitesse.y   = -m_vitesse.y;
        out_of_border = false;
    }
    else if (m_position.y < m_bottom_limit + m_radius)
    {
        m_position.y  = m_bottom_limit + m_radius;
        m_vitesse.y   = -m_vitesse.y;
        out_of_border = false;
    }
    if (m_position.x > m_right_limit - m_radius)
    {
        m_position.x  = m_right_limit - m_radius;
        m_vitesse.x   = -m_vitesse.x;
        out_of_border = false;
    }
    else if (m_position.x < m_left_limit + m_radius)
    {
        m_position.x  = m_left_limit + m_radius;
        m_vitesse.x   = -m_vitesse.x;
        out_of_border = false;
    }
    return out_of_border;
}