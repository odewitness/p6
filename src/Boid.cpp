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
        Boid      boid(position, direction, vitesse, taille, top_limit, bottom_limit, left_limit, right_limit);
        boids.push_back(boid);
    }
    return boids;
}

glm::vec2 Boid::get_vitesse() const
{
    return m_vitesse;
}

void Boid::set_vitesse(const glm::vec2& vitesse)
{
    m_vitesse = vitesse;
}

glm::vec2 Boid::get_position() const
{
    return m_position;
}

void Boid::set_position(const glm::vec2& position)
{
    m_position = position;
}

float Boid::get_taille() const
{
    return m_taille;
}
void Boid::set_taille(const float& taille)
{
    m_taille = taille;
}

float Boid::distance_between_boids(Boid boid1, Boid boid2)
{
    return glm::distance(boid1.m_position, boid2.m_position);
}

void Boid::separation(std::vector<Boid>& boids, const float& separation_distance, const float& separation_strength)
{
    glm::vec2 separation(0.0f);
    int       count = 0;
    for (const auto& boid : boids)
    {
        if (&boid == this)
        {
            continue;
        }

        const float distance = glm::distance(m_position, boid.m_position);
        if (distance < separation_distance)
        {
            separation += (m_position - boid.m_position) / (distance * distance);
            // separation += separation_strength * (m_position - boid.m_position);
            count++;
        }
    }
    if (count > 0)
    {
        separation /= static_cast<float>(count);
        separation = glm::normalize(separation) * separation_strength;
    }
    m_direction += separation;
    m_direction = glm::normalize(m_direction);
}
