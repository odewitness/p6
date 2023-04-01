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
    float     m_limite_haut;
    float     m_limite_bas;
    float     m_limite_gauche;
    float     m_limite_droite;

public:
    Boid();
    Boid(glm::vec2 position, glm::vec2 direction, float vitesse, float taille, float m_limite_haut, float m_limite_bas, float m_limite_gauche, float m_limite_droite);

    void                     dessin(p6::Context& ctx);
    void                     mouvement();
    bool                     rebondir_si_hors_limite();
    static std::vector<Boid> creation_boids(int num_boids, float top_limit, float limite_bas, float limite_gauche, float limite_droite);
    void                     cohesion(std::vector<Boid>& boids, const float& cohesion_distance, const float& cohesion_force);
    void                     alignement(std::vector<Boid>& boids, const float& alignement_distance, const float& alignement_force);
    void                     separation(const std::vector<Boid>& boids, float separation_distance, float separation_force);
};

#endif