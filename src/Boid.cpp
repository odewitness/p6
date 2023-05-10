#include "Boid.hpp"

Boid::Boid(const BoidProperties& properties)
    : m_properties(properties) {}

void Boid::set_taille(float taille)
{
    m_properties.taille = taille;
}

void Boid::dessin(p6::Context& ctx) const
{
    ctx.fill = {0.6627451f, 0.59215686f, 0.8745098f};

    ctx.stroke = {0.21960784f, 0.22352941f, 0.34509804f};
    ctx.equilateral_triangle(
        p6::Center{m_properties.position},
        p6::Radius{m_properties.taille},
        p6::Rotation{m_properties.direction}
    );
}

void Boid::mouvement()
{
    m_properties.position += m_properties.direction * m_properties.vitesse;
}

bool Boid::rebondir_si_hors_limite()
{
    bool  hors_limite = true;
    float rebond      = 0.5f;

    if (m_properties.position.y > m_properties.limite_haut - m_properties.taille)
    {
        m_properties.position.y  = m_properties.limite_haut - m_properties.taille;
        m_properties.direction.y = -m_properties.direction.y * rebond;
        m_properties.direction.x += rebond * (1 - std::abs(m_properties.direction.y));
        hors_limite = false;
    }
    else if (m_properties.position.y < m_properties.limite_bas + m_properties.taille)
    {
        m_properties.position.y  = m_properties.limite_bas + m_properties.taille;
        m_properties.direction.y = -m_properties.direction.y * rebond;
        m_properties.direction.x -= rebond * (1 - std::abs(m_properties.direction.y));
        hors_limite = false;
    }
    if (m_properties.position.x > m_properties.limite_droite - m_properties.taille)
    {
        m_properties.position.x  = m_properties.limite_droite - m_properties.taille;
        m_properties.direction.x = -m_properties.direction.x * rebond;
        m_properties.direction.y += rebond * (1 - std::abs(m_properties.direction.x));
        hors_limite = false;
    }
    else if (m_properties.position.x < m_properties.limite_gauche + m_properties.taille)
    {
        m_properties.position.x  = m_properties.limite_gauche + m_properties.taille;
        m_properties.direction.x = -m_properties.direction.x * rebond;
        m_properties.direction.y -= rebond * (1 - std::abs(m_properties.direction.x));
        hors_limite = false;
    }

    m_properties.direction = glm::normalize(m_properties.direction);
    return hors_limite;
}

std::vector<Boid> creation_boids(int num_boids, float limite_haut, float limite_bas, float limite_gauche, float limite_droite, float taille_boid)
{
    std::vector<Boid> boids;
    for (int i = 0; i < num_boids; i++)
    {
        glm::vec2 position  = glm::vec2(p6::random::number(limite_gauche, limite_droite), p6::random::number(limite_bas, limite_haut));
        Vitesse   vitesse   = p6::random::number(0.001f, 0.005f);
        glm::vec2 direction = glm::vec2(p6::random::number(0.5f, 1.00f), p6::random::number(0.01f, 1.0f));
        glm::vec3 color     = glm::vec3(p6::random::number(0.0f, 1.0f), p6::random::number(0.0f, 1.0f), p6::random::number(0.0f, 1.0f));

        BoidProperties properties = {position, direction, vitesse, taille_boid, limite_haut, limite_bas, limite_gauche, limite_droite, color};
        Boid           boid(properties);
        boids.push_back(boid);
    }
    return boids;
}

void Boid::cohesion(std::vector<Boid>& boids, const float& cohesion_rayon, const float& cohesion_force)
{
    glm::vec2 centre_boids(0.0f);
    int       compteur_voisin_proximite = 0;

    for (const auto& boid : boids)
    {
        // On ignore le boid courant
        if (&boid == this)
        {
            continue;
        }

        float distance = glm::distance(m_properties.position, boid.m_properties.position);
        if (distance < cohesion_rayon)
        {
            // On ajoute la position du boid courant au centre de gravité des boids voisins
            centre_boids += boid.m_properties.position;
            compteur_voisin_proximite++;
        }
    }

    // Si au moins un voisin à proximité a été trouvé
    if (compteur_voisin_proximite > 0)
    {
        // On calcule la position moyenne du centre de gravité des boids voisins
        centre_boids /= static_cast<float>(compteur_voisin_proximite);
        glm::vec2 vecteur_cohesion = centre_boids - m_properties.position;
        vecteur_cohesion           = glm::normalize(vecteur_cohesion) * cohesion_force;

        // On ajoute le vecteur de cohésion à la direction actuelle du boid
        m_properties.direction += vecteur_cohesion;
        m_properties.direction = glm::normalize(m_properties.direction);
    }
}

void Boid::alignement(std::vector<Boid>& boids, const float& alignement_rayon, const float& alignement_force)
{
    glm::vec2 direction_moyenne(0.0f);
    int       compteur_voisin_proximite = 0;

    for (const auto& boid : boids)
    {
        // On ignore le boid courant
        if (&boid == this)
        {
            continue;
        }

        float distance = glm::distance(m_properties.position, boid.m_properties.position);

        // Si le boid est dans la distance d'alignement
        if (distance < alignement_rayon)
        {
            // On ajoute la direction du voisin au total
            direction_moyenne += boid.m_properties.direction;
            compteur_voisin_proximite++;
        }
    }

    // Si au moins un voisin est aligné
    if (compteur_voisin_proximite > 0)
    {
        // On calcule la direction moyenne et on normalise
        direction_moyenne /= static_cast<float>(compteur_voisin_proximite);
        direction_moyenne = glm::normalize(direction_moyenne) * alignement_force;

        // On ajoute la direction moyenne à la direction du boid courant et on normalise
        m_properties.direction += direction_moyenne;
        m_properties.direction = glm::normalize(m_properties.direction);
    }
}

void Boid::separation(std::vector<Boid>& boids, const float& separation_rayon, const float& separation_force)
{
    glm::vec2 separation_moyenne(0.0f);
    int       compteur_voisin_proximite = 0;

    for (const auto& other_boid : boids)
    {
        // On ignore le boid courant
        if (&other_boid == this)
        {
            continue;
        }

        float distance = glm::distance(m_properties.position, other_boid.m_properties.position);

        if (distance < separation_rayon)
        {
            // On calcule le facteur de séparation en fonction de la distance
            float facteur_separation = (separation_rayon - distance) / separation_rayon;

            // On ajoute la force de séparation à la force totale, en tenant compte du facteur de séparation et de la distance
            separation_moyenne += (m_properties.position - other_boid.m_properties.position) * facteur_separation / (distance * distance);

            compteur_voisin_proximite++;
        }
    }

    if (compteur_voisin_proximite > 0)
    {
        // On calcule la moyenne de la force de séparation et on normalise
        separation_moyenne /= static_cast<float>(compteur_voisin_proximite);
        separation_moyenne = glm::normalize(separation_moyenne) * separation_force;

        // On ajoute la force de séparation à la direction actuelle du boid et on normalise
        m_properties.direction += separation_moyenne;
        m_properties.direction = glm::normalize(m_properties.direction);
    }
}

void Boid::update(const float& taille_boid, p6::Context& ctx, std::vector<Boid>& boids, const float& cohesion_rayon, const float& cohesion_force, const float& alignement_rayon, const float& alignement_force, const float& separation_rayon, const float& separation_force)
{
    set_taille(taille_boid);
    dessin(ctx);
    mouvement();
    if (rebondir_si_hors_limite())
    {
        cohesion(boids, cohesion_rayon, cohesion_force);
        alignement(boids, alignement_rayon, alignement_force);
        separation(boids, separation_rayon, separation_force);
        mouvement();
    }
}