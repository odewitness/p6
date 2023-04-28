#include <cstdlib>
#include <vector>
#include "Boid.hpp"
#include "Proie.hpp"
#include "glm/fwd.hpp"
#include "p6/p6.h"

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

int main(int argc, char* argv[])
{
    { // Run the tests
        if (doctest::Context{}.run() != 0)
            return EXIT_FAILURE;
        // The CI does not have a GPU so it cannot run the rest of the code.
        const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (no_gpu_available)
            return EXIT_SUCCESS;
    }

    // Actual app
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};
    ctx.maximize_window();

    // Paramètres
    float limite_gauche = -ctx.aspect_ratio();
    float limite_droite = ctx.aspect_ratio();
    float limite_haut   = 1;
    float limite_bas    = -1;
    int   nombre_boids  = 2;
    float taille_boid   = 0.05f;

    float cohesion_force      = 0.f;
    float cohesion_distance   = 0.5f;
    float alignement_force    = 0.f;
    float alignement_distance = 0.5f;
    float separation_force    = 0.f;
    float separation_distance = 0.5f;

    std::vector<Boid>
        boids = Boid::creation_boids(nombre_boids, limite_haut, limite_bas, limite_gauche, limite_droite, taille_boid);

    // Paramètres IMGUI
    ctx.imgui = [&]() {
        ImGui::Begin("Paramètres");
        ImGui::SliderInt("Nombre de boids", &nombre_boids, 0, 100);
        ImGui::SliderFloat("Taille Boid", &taille_boid, 0.01f, 0.3f);
        ImGui::SliderFloat("Cohesion", &cohesion_force, 0.f, 0.1f);
        ImGui::SliderFloat("Cohesion Distance", &cohesion_distance, 0.f, 2.0f);
        ImGui::SliderFloat("Alignement", &alignement_force, 0.0f, 0.1f);
        ImGui::SliderFloat("Alignement Distance", &alignement_distance, 0.0f, 2.0f);
        ImGui::SliderFloat("Séparation", &separation_force, 0.0f, 0.1f);
        ImGui::SliderFloat("Séparation Distance", &separation_distance, 0.0f, 2.0f);
        ImGui::End();

        // Gestion de la taille maximale des boids
        if (taille_boid > 0.2)
            nombre_boids %= 20;

        // Ajout et suppression des boids
        int taille_boids_vecteur = boids.size(); // OK car taille max = 100 < taille max int
        if (taille_boids_vecteur < nombre_boids)
        {
            int  nombre_de_boids_a_ajouter = nombre_boids - taille_boids_vecteur;
            auto nouveaux_boids            = Boid::creation_boids(nombre_de_boids_a_ajouter, limite_haut, limite_bas, limite_gauche, limite_droite, taille_boid);
            boids.insert(boids.end(), nouveaux_boids.begin(), nouveaux_boids.end());
        }
        else if (taille_boids_vecteur > nombre_boids)
        {
            int nombre_de_boids_a_enlever = taille_boids_vecteur - nombre_boids;
            boids.erase(boids.end() - nombre_de_boids_a_enlever, boids.end());
        }
    };

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background({0.86f, 0.81f, 0.92f});

        for (auto& boid : boids)
        {
            boid.set_taille(taille_boid);
            boid.dessin(ctx);
            // boid.update(boids, cohesion_distance, cohesion_force, alignement_distance, alignement_force, separation_distance, separation_force);
            boid.mouvement();
            if (boid.rebondir_si_hors_limite())
            {
                boid.cohesion(boids, cohesion_distance, cohesion_force);
                boid.alignement(boids, alignement_distance, alignement_force);
                boid.separation(boids, separation_distance, separation_force);
                boid.mouvement();
            }
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}