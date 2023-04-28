#ifndef BOID_H
#define BOID_H

#include <cmath>
#include <utility>
#include "config.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

struct BoidProperties {
    glm::vec2 position;
    glm::vec2 direction;
    float     vitesse;
    float     taille;
    float     limite_haut;
    float     limite_bas;
    float     limite_gauche;
    float     limite_droite;
    glm::vec3 color;
};

class Boid {
private:
    glm::vec2      m_position;
    glm::vec2      m_direction;
    float          m_vitesse;
    float          m_taille;
    float          m_limite_haut;
    float          m_limite_bas;
    float          m_limite_gauche;
    float          m_limite_droite;
    glm::vec3      m_color;
    BoidProperties m_properties;

public:
    Boid();
    Boid(const BoidProperties& properties);

    void set_taille(float taille);
    void set_color(glm::vec3 color);

    void                     dessin(p6::Context& ctx);
    void                     afficher_statistiques() const;
    void                     update(std::vector<Boid>& boids, const float& cohesion_distance, const float& cohesion_force, const float& alignement_distance, const float& alignement_force, const float& separation_distance, const float& separation_force);
    static std::vector<Boid> creation_boids(int num_boids, float limite_haut, float limite_bas, float limite_gauche, float limite_droite, float taille_boid);

    // private:
    void separation(const std::vector<Boid>& boids, float separation_distance, float separation_force);
    void cohesion(std::vector<Boid>& boids, const float& cohesion_distance, const float& cohesion_force);
    void alignement(std::vector<Boid>& boids, const float& alignement_distance, const float& alignement_force);
    bool rebondir_si_hors_limite();
    void mouvement();
};

#endif