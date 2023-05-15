#pragma once

#include "glm/fwd.hpp"
#include "p6/p6.h"

// A changer par une struct, ce n'est pas un strong type
using Vitesse = float;

struct BoidProperties {
    glm::vec3 position;
    glm::vec3 direction;
    Vitesse   vitesse;
    float     taille;
    float     limite_haut;
    float     limite_bas;
    float     limite_gauche;
    float     limite_droite;
    float     limite_devant;
    float     limite_derriere;
    glm::vec3 color;
};

class Boid {
private:
    // glm::vec3      m_position;
    glm::vec3      m_direction;
    Vitesse        m_vitesse;
    float          m_taille;
    float          m_limite_haut;
    float          m_limite_bas;
    float          m_limite_gauche;
    float          m_limite_droite;
    float          m_limite_devant;
    float          m_limite_derriere;
    glm::vec3      m_color;
    BoidProperties m_properties;

public:
    Boid();
    explicit Boid(const BoidProperties& properties);

    void update(const float& taille_boid, p6::Context& ctx, std::vector<Boid>& boids, const float& cohesion_rayon, const float& cohesion_force, const float& alignement_rayon, const float& alignement_force, const float& separation_rayon, const float& separation_force);

    glm::vec3 get_position();

private:
    void set_taille(float taille);

    void dessin(p6::Context& ctx) const;

    void separation(std::vector<Boid>& boids, const float& separation_rayon, const float& separation_force);
    void cohesion(std::vector<Boid>& boids, const float& cohesion_rayon, const float& cohesion_force);
    void alignement(std::vector<Boid>& boids, const float& alignement_rayon, const float& alignement_force);

    bool rebondir_si_hors_limite();
    void mouvement();
};

std::vector<Boid> creation_boids(int num_boids, float limite_haut, float limite_bas, float limite_gauche, float limite_droite, float taille_boid, float limite_devant, float limite_derriere);

///////////////////////
//// Sauvegarde 2D ////
///////////////////////

// #pragma once

// #include "p6/p6.h"
// using Vitesse = float;

// struct BoidProperties {
//     glm::vec2 position;
//     glm::vec2 direction;
//     Vitesse   vitesse;
//     float     taille;
//     float     limite_haut;
//     float     limite_bas;
//     float     limite_gauche;
//     float     limite_droite;
//     glm::vec3 color;
// };

// class Boid {
// private:
//     glm::vec2      m_position;
//     glm::vec2      m_direction;
//     Vitesse        m_vitesse;
//     float          m_taille;
//     float          m_limite_haut;
//     float          m_limite_bas;
//     float          m_limite_gauche;
//     float          m_limite_droite;
//     glm::vec3      m_color;
//     BoidProperties m_properties;

// public:
//     Boid();
//     explicit Boid(const BoidProperties& properties);

//     void update(const float& taille_boid, p6::Context& ctx, std::vector<Boid>& boids, const float& cohesion_rayon, const float& cohesion_force, const float& alignement_rayon, const float& alignement_force, const float& separation_rayon, const float& separation_force);

// private:
//     void set_taille(float taille);

//     void dessin(p6::Context& ctx) const;

//     void separation(std::vector<Boid>& boids, const float& separation_rayon, const float& separation_force);
//     void cohesion(std::vector<Boid>& boids, const float& cohesion_rayon, const float& cohesion_force);
//     void alignement(std::vector<Boid>& boids, const float& alignement_rayon, const float& alignement_force);

//     bool rebondir_si_hors_limite();
//     void mouvement();
// };

// std::vector<Boid> creation_boids(int num_boids, float limite_haut, float limite_bas, float limite_gauche, float limite_droite, float taille_boid);