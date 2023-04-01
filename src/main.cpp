#include <cstdlib>
#include <vector>
#include "Boid.hpp"
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
    //////// TODO faire ue fonction ?
    float limite_gauche = -ctx.aspect_ratio();
    float limite_droite = ctx.aspect_ratio();
    float limite_haut   = 1;
    float limite_bas    = -1;

    std::vector<Boid>
        boids = Boid::creation_boids(50, limite_haut, limite_bas, limite_gauche, limite_droite);

    float cohesion_force      = 0.f;
    float cohesion_distance   = 0.5f;
    float alignement_force    = 0.f;
    float alignement_distance = 0.5f;
    float separation_force    = 0.f;
    float separation_distance = 0.5f;

    glm::vec2 min_window_size{-ctx.aspect_ratio(), -1};
    glm::vec2 max_window_size{ctx.aspect_ratio(), 1};
    //////////

    ctx.imgui = [&]() {
        ImGui::Begin("Parameters");
        ImGui::SliderFloat("Cohesion", &cohesion_force, 0.f, 0.1f);
        ImGui::SliderFloat("Cohesion Distance", &cohesion_distance, 0.f, 2.0f);
        ImGui::SliderFloat("alignement", &alignement_force, 0.0f, 0.1f);
        ImGui::SliderFloat("alignement Distance", &alignement_distance, 0.0f, 2.0f);
        ImGui::SliderFloat("Séparation", &separation_force, 0.0f, 0.1f);
        ImGui::SliderFloat("Séparation Distance", &separation_distance, 0.0f, 2.0f);
        ImGui::End();
    };

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Blue);
        for (auto& boid : boids)
        {
            boid.dessin(ctx);
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