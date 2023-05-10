#include <cstdlib>
#include <vector>
#include "Boid.hpp"
#include "glm/fwd.hpp"
#include "imgui.h"
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
    int   nombre_boids  = 20;
    float taille_boid   = 0.01f;

    float cohesion_force   = 0.f;
    float cohesion_rayon   = 0.5f;
    float alignement_force = 0.f;
    float alignement_rayon = 0.5f;
    float separation_force = 0.f;
    float separation_rayon = 0.5f;

    std::vector<Boid>
        boids = creation_boids(nombre_boids, limite_haut, limite_bas, limite_gauche, limite_droite, taille_boid);

    bool etat_checkbox = false; // Variable pour stocker l'état de la checkbox
    // Paramètres IMGUI
    ctx.imgui = [&]() {
        ImGui::Begin("Paramètres");

        ImGui::Text("Configuration de l'affichage :");
        ImGui::SliderInt("Nombre de boids", &nombre_boids, 0, 100);
        ImGui::SliderFloat("Taille Boid", &taille_boid, 0.01f, 0.3f);
        ImGui::Checkbox("Limiter le nombre de boids en fonction de la taille", &etat_checkbox);

        ImGui::Separator();
        ImGui::Text("Configuration du comportement des Boids :");
        ImGui::SetNextTreeNodeOpen(true);
        if (ImGui::CollapsingHeader("Cohésion"))
        {
            ImGui::SliderFloat("Force de cohésion", &cohesion_force, 0.f, 0.1f);
            ImGui::SliderFloat("Rayon de cohésion", &cohesion_rayon, 0.f, 2.0f);
        }
        ImGui::SetNextTreeNodeOpen(true);
        if (ImGui::CollapsingHeader("Alignement"))
        {
            ImGui::SliderFloat("Force d'alignement", &alignement_force, 0.0f, 0.1f);
            ImGui::SliderFloat("Rayon d'alignement", &alignement_rayon, 0.0f, 2.0f);
        }
        ImGui::SetNextTreeNodeOpen(true);
        if (ImGui::CollapsingHeader("Séparation"))
        {
            ImGui::SliderFloat("Force de séparation", &separation_force, 0.0f, 0.1f);
            ImGui::SliderFloat("Rayon de séparation", &separation_rayon, 0.0f, 2.0f);
        }
        ImGui::End();

        // Ajout et suppression des boids
        int taille_boids_vecteur = boids.size(); // OK car taille max = 100 < taille max int
        if (taille_boids_vecteur < nombre_boids)
        {
            int  nombre_de_boids_a_ajouter = nombre_boids - taille_boids_vecteur;
            auto nouveaux_boids            = creation_boids(nombre_de_boids_a_ajouter, limite_haut, limite_bas, limite_gauche, limite_droite, taille_boid);
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
            if (etat_checkbox)
            {
                if (taille_boid > 0.2)
                {
                    if (nombre_boids > 20)
                    {
                        nombre_boids = 20;
                    }
                }
            }
            boid.update(taille_boid, ctx, boids, cohesion_rayon, cohesion_force, alignement_rayon, alignement_force, separation_rayon, separation_force);
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}