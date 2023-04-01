#include "Boid.hpp"
#include "glm/fwd.hpp"

Boid::Boid(glm::vec2 position, glm::vec2 direction, float vitesse, float taille, float top_limit, float bottom_limit, float left_limit, float right_limit)
    : m_position(glm::vec2(position)), m_direction(glm::vec2(direction)), m_vitesse(vitesse), m_taille(taille), m_limite_haut(top_limit), m_limite_bas(bottom_limit), m_limite_gauche(left_limit), m_limite_droite(right_limit) {}

void Boid::dessin(p6::Context& ctx)
{
    // ctx.square(p6::Center{m_position}, p6::Radius{m_taille}, p6::Rotation{0.0_radians});
    ctx.equilateral_triangle(
        p6::Center{m_position},
        p6::Radius{m_taille},
        p6::Rotation{m_direction}
    );
}

void Boid::mouvement()
{
    m_position += m_direction * m_vitesse;
}

bool Boid::rebondir_si_hors_limite()
{
    bool  hors_limite = true;
    float rebond      = 0.5f;

    if (m_position.y > m_limite_haut - m_taille)
    {
        m_position.y  = m_limite_haut - m_taille;
        m_direction.y = -m_direction.y * rebond;
        m_direction.x += rebond * (1 - abs(m_direction.y));
        hors_limite = false;
    }
    else if (m_position.y < m_limite_bas + m_taille)
    {
        m_position.y  = m_limite_bas + m_taille;
        m_direction.y = -m_direction.y * rebond;
        m_direction.x -= rebond * (1 - abs(m_direction.y));
        hors_limite = false;
    }
    if (m_position.x > m_limite_droite - m_taille)
    {
        m_position.x  = m_limite_droite - m_taille;
        m_direction.x = -m_direction.x * rebond;
        m_direction.y += rebond * (1 - abs(m_direction.x));
        hors_limite = false;
    }
    else if (m_position.x < m_limite_gauche + m_taille)
    {
        m_position.x  = m_limite_gauche + m_taille;
        m_direction.x = -m_direction.x * rebond;
        m_direction.y -= rebond * (1 - abs(m_direction.x));
        hors_limite = false;
    }

    m_direction = glm::normalize(m_direction);
    return hors_limite;
}

std::vector<Boid> Boid::creation_boids(int num_boids, float top_limit, float bottom_limit, float left_limit, float right_limit)
{
    std::vector<Boid> boids;
    for (int i = 0; i < num_boids; i++)
    {
        glm::vec2 position  = glm::vec2(p6::random::number(left_limit, right_limit), p6::random::number(bottom_limit, top_limit));
        float     vitesse   = p6::random::number(0.001f, 0.005f);
        glm::vec2 direction = glm::vec2(p6::random::number(0.5f, 1.00f), p6::random::number(0.01f, 1.0f));
        // float     taille    = p6::random::number(0.1f, 0.2f);
        float taille = 0.05f;
        Boid  boid(position, direction, vitesse, taille, top_limit, bottom_limit, left_limit, right_limit);
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

void Boid::alignement(std::vector<Boid>& boids, const float& alignement_distance, const float& alignement_strength)
{
    glm::vec2 average_direction(0.0f);
    int       count = 0;

    for (const auto& boid : boids)
    {
        if (&boid == this)
        {
            continue;
        }

        float distance = glm::distance(m_position, boid.m_position);
        if (distance < alignement_distance)
        {
            average_direction += boid.m_direction;
            count++;
        }
    }

    if (count > 0)
    {
        average_direction /= static_cast<float>(count);
        average_direction = glm::normalize(average_direction) * alignement_strength;
        m_direction += average_direction;
        m_direction = glm::normalize(m_direction);
    }
}

void Boid::separation(const std::vector<Boid>& boids, float separation_distance, float separation_strength)
{
    glm::vec2 separation_force(0.0f, 0.0f);
    int       neighbor_count = 0;

    for (const auto& other_boid : boids)
    {
        if (&other_boid == this)
        {
            continue;
        }

        float distance = glm::distance(m_position, other_boid.m_position);

        if (distance < separation_distance)
        {
            float separation_factor = (separation_distance - distance) / separation_distance;
            separation_force += (m_position - other_boid.m_position) * separation_factor / (distance * distance);
            neighbor_count++;
        }
    }

    if (neighbor_count > 0)
    {
        separation_force /= static_cast<float>(neighbor_count);
        separation_force = glm::normalize(separation_force) * separation_strength;
    }

    m_direction += separation_force;
    m_direction = glm::normalize(m_direction);
}