#include "Boid.hpp"

Boid::Boid(const BoidProperties& properties)
    : m_properties(properties) {}

void Boid::set_size(float size)
{
    m_properties.size = size;
}

glm::vec3 Boid::get_position() const
{
    return m_properties.position;
}

void Boid::movement()
{
    m_properties.position += m_properties.direction * m_properties.speed;
}

bool Boid::bounce_if_out_of_limits()
{
    bool  hors_limite = true;
    float rebond      = 0.5f;

    if (m_properties.position.y > m_properties.upper_limit - m_properties.size)
    {
        m_properties.position.y  = m_properties.upper_limit - m_properties.size;
        m_properties.direction.y = -m_properties.direction.y * rebond;
        m_properties.direction.x += rebond * (1 - std::abs(m_properties.direction.y));
        m_properties.direction.z += rebond * (1 - std::abs(m_properties.direction.y));
        hors_limite = false;
    }
    else if (m_properties.position.y < m_properties.lower_limit + m_properties.size)
    {
        m_properties.position.y  = m_properties.lower_limit + m_properties.size;
        m_properties.direction.y = -m_properties.direction.y * rebond;
        m_properties.direction.x -= rebond * (1 - std::abs(m_properties.direction.y));
        m_properties.direction.z -= rebond * (1 - std::abs(m_properties.direction.y));
        hors_limite = false;
    }
    if (m_properties.position.x > m_properties.right_limit - m_properties.size)
    {
        m_properties.position.x  = m_properties.right_limit - m_properties.size;
        m_properties.direction.x = -m_properties.direction.x * rebond;
        m_properties.direction.y += rebond * (1 - std::abs(m_properties.direction.x));
        m_properties.direction.z += rebond * (1 - std::abs(m_properties.direction.x));
        hors_limite = false;
    }
    else if (m_properties.position.x < m_properties.left_limit + m_properties.size)
    {
        m_properties.position.x  = m_properties.left_limit + m_properties.size;
        m_properties.direction.x = -m_properties.direction.x * rebond;
        m_properties.direction.y -= rebond * (1 - std::abs(m_properties.direction.x));
        m_properties.direction.z -= rebond * (1 - std::abs(m_properties.direction.x));
        hors_limite = false;
    }
    if (m_properties.position.z > m_properties.foreground_limit - m_properties.size)
    {
        m_properties.position.z  = m_properties.foreground_limit - m_properties.size;
        m_properties.direction.z = -m_properties.direction.z * rebond;
        m_properties.direction.y += rebond * (1 - std::abs(m_properties.direction.x));
        m_properties.direction.x += rebond * (1 - std::abs(m_properties.direction.x));
        hors_limite = false;
    }
    else if (m_properties.position.z < m_properties.left_limit + m_properties.size)
    {
        m_properties.position.z  = m_properties.left_limit + m_properties.size;
        m_properties.direction.z = -m_properties.direction.z * rebond;
        m_properties.direction.y -= rebond * (1 - std::abs(m_properties.direction.x));
        m_properties.direction.x -= rebond * (1 - std::abs(m_properties.direction.x));
        hors_limite = false;
    }

    m_properties.direction = glm::normalize(m_properties.direction);
    return hors_limite;
}

std::vector<Boid> creation_boids(int num_boids, float upper_limit, float lower_limit, float left_limit, float right_limit, float size_boid, float foreground_limit, float background_limit)
{
    std::vector<Boid> boids;
    for (int i = 0; i < num_boids; i++)
    {
        glm::vec3 position  = glm::vec3(p6::random::number(left_limit, right_limit), p6::random::number(lower_limit, upper_limit), p6::random::number(background_limit, foreground_limit));
        float     speed     = p6::random::number(0.001f, 0.005f);
        glm::vec3 direction = glm::vec3(p6::random::number(0.5f, 1.00f), p6::random::number(0.01f, 1.0f), p6::random::number(0.01f, 1.0f));
        glm::vec3 color     = glm::vec3(p6::random::number(0.0f, 1.0f), p6::random::number(0.0f, 1.0f), p6::random::number(0.0f, 1.0f));

        BoidProperties properties = {position, direction, speed, size_boid, upper_limit, lower_limit, left_limit, right_limit, foreground_limit, background_limit, color};
        Boid           boid(properties);
        boids.push_back(boid);
    }
    return boids;
}

void Boid::cohesion(std::vector<Boid>& boids, const float& cohesion_radius, const float& cohesion_strength)
{
    glm::vec3 center_boids(0.0f);
    int       nearby_neighbour_counter = 0;

    for (const auto& boid : boids)
    {
        if (&boid == this)
        {
            continue;
        }

        float distance = glm::distance(m_properties.position, boid.m_properties.position);
        if (distance < cohesion_radius)
        {
            center_boids += boid.m_properties.position;
            nearby_neighbour_counter++;
        }
    }

    if (nearby_neighbour_counter > 0)
    {
        center_boids /= static_cast<float>(nearby_neighbour_counter);
        glm::vec3 vecteur_cohesion = center_boids - m_properties.position;
        vecteur_cohesion           = glm::normalize(vecteur_cohesion) * cohesion_strength;

        m_properties.direction += vecteur_cohesion;
        m_properties.direction = glm::normalize(m_properties.direction);
    }
}

void Boid::alignment(std::vector<Boid>& boids, const float& alignment_radius, const float& alignment_strength)
{
    glm::vec3 direction_average(0.0f);
    int       nearby_neighbour_counter = 0;

    for (const auto& boid : boids)
    {
        if (&boid == this)
        {
            continue;
        }

        float distance = glm::distance(m_properties.position, boid.m_properties.position);

        if (distance < alignment_radius)
        {
            direction_average += boid.m_properties.direction;
            nearby_neighbour_counter++;
        }
    }

    if (nearby_neighbour_counter > 0)
    {
        direction_average /= static_cast<float>(nearby_neighbour_counter);
        direction_average = glm::normalize(direction_average) * alignment_strength;

        m_properties.direction += direction_average;
        m_properties.direction = glm::normalize(m_properties.direction);
    }
}

void Boid::separation(std::vector<Boid>& boids, const float& separation_radius, const float& separation_strength)
{
    glm::vec3 separation_average(0.0f);
    int       nearby_neighbour_counter = 0;

    for (const auto& other_boid : boids)
    {
        if (&other_boid == this)
        {
            continue;
        }

        float distance = glm::distance(m_properties.position, other_boid.m_properties.position);

        if (distance < separation_radius)
        {
            float separation_factor = (separation_radius - distance) / separation_radius;
            separation_average += (m_properties.position - other_boid.m_properties.position) * separation_factor / (distance * distance);

            nearby_neighbour_counter++;
        }
    }

    if (nearby_neighbour_counter > 0)
    {
        separation_average /= static_cast<float>(nearby_neighbour_counter);
        separation_average = glm::normalize(separation_average) * separation_strength;

        m_properties.direction += separation_average;
        m_properties.direction = glm::normalize(m_properties.direction);
    }
}

void Boid::update(const float& size_boid, p6::Context& ctx, std::vector<Boid>& boids, const float& cohesion_radius, const float& cohesion_strength, const float& alignment_radius, const float& alignment_strength, const float& separation_radius, const float& separation_strength)
{
    set_size(size_boid);
    movement();
    if (bounce_if_out_of_limits())
    {
        cohesion(boids, cohesion_radius, cohesion_strength);
        alignment(boids, alignment_radius, alignment_strength);
        separation(boids, separation_radius, separation_strength);
        movement();
    }
}