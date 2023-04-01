#include "Boid.hpp"
#include "glm/fwd.hpp"

Boid::Boid(glm::vec2 position, glm::vec2 direction, glm::vec2 vitesse, float taille, float top_limit, float bottom_limit, float left_limit, float right_limit)
    : m_position(glm::vec2(position)), m_direction(glm::vec2(direction)), m_vitesse(glm::vec2(vitesse)), m_taille(taille), m_top_limit(top_limit), m_bottom_limit(bottom_limit), m_left_limit(left_limit), m_right_limit(right_limit) {}

void Boid::draw(p6::Context& ctx)
{
    ctx.square(p6::Center{m_position}, p6::Radius{m_taille}, p6::Rotation{0.0_radians});
}

void Boid::movement()
{
    m_position += m_direction * m_vitesse;
}

bool Boid::borders_bool()
{
    bool out_of_border = true;
    if (m_position.y > m_top_limit - m_taille)
    {
        m_position.y  = m_top_limit - m_taille;
        m_vitesse.y   = -m_vitesse.y;
        out_of_border = false;
    }
    else if (m_position.y < m_bottom_limit + m_taille)
    {
        m_position.y  = m_bottom_limit + m_taille;
        m_vitesse.y   = -m_vitesse.y;
        out_of_border = false;
    }
    if (m_position.x > m_right_limit - m_taille)
    {
        m_position.x  = m_right_limit - m_taille;
        m_vitesse.x   = -m_vitesse.x;
        out_of_border = false;
    }
    else if (m_position.x < m_left_limit + m_taille)
    {
        m_position.x  = m_left_limit + m_taille;
        m_vitesse.x   = -m_vitesse.x;
        out_of_border = false;
    }
    return out_of_border;
}

std::vector<Boid> Boid::create_boids(int num_boids, float top_limit, float bottom_limit, float left_limit, float right_limit)
{
    std::vector<Boid> boids;
    for (int i = 0; i < num_boids; i++)
    {
        glm::vec2 position  = glm::vec2(p6::random::number(left_limit, right_limit), p6::random::number(bottom_limit, top_limit));
        glm::vec2 vitesse   = glm::vec2(p6::random::number(-0.01f, 0.01f), p6::random::number(-0.01f, 0.01f));
        glm::vec2 direction = glm::vec2(p6::random::number(0.5f, 1.00f), p6::random::number(0.01f, 1.0f));
        float     taille    = p6::random::number(0.1f, 0.2f);
        // float taille = 0.2f;
        Boid boid(position, direction, vitesse, taille, top_limit, bottom_limit, left_limit, right_limit);
        boids.push_back(boid);
    }
    return boids;
}

void Boid::cohesion(std::vector<Boid>& boids, const float& cohesion_distance, const float& cohesion_force)
{
    glm::vec2 centre_boids(0.0f);
    int       count = 0;

    for (const auto& boid : boids)
    {
        if (&boid == this)
        {
            continue;
        }

        float distance = glm::distance(m_position, boid.m_position);
        if (distance < cohesion_distance)
        {
            centre_boids += boid.m_position;
            count++;
        }
    }

    if (count > 0)
    {
        centre_boids /= static_cast<float>(count);
        glm::vec2 cohesion_vector = centre_boids - m_position;
        cohesion_vector           = glm::normalize(cohesion_vector) * cohesion_force;
        m_direction += cohesion_vector;
        m_direction = glm::normalize(m_direction);
    }
}