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
    float left_limit   = -ctx.aspect_ratio();
    float right_limit  = ctx.aspect_ratio();
    float top_limit    = 1;
    float bottom_limit = -1;

    std::vector<Boid>
        boids = Boid::create_boids(50, top_limit, bottom_limit, left_limit, right_limit);

    float cohesion_force      = 0.f;
    float cohesion_distance   = 0.5f;
    float alignment_force     = 0.f;
    float alignment_distance  = 0.5f;
    float separation_force    = 0.f;
    float separation_distance = 0.5f;

    glm::vec2 min_window_size{-ctx.aspect_ratio(), -1};
    glm::vec2 max_window_size{ctx.aspect_ratio(), 1};
    //////////

    ctx.imgui = [&]() {
        ImGui::Begin("Parameters");
        ImGui::SliderFloat("Cohesion", &cohesion_force, 0.f, 0.1f);
        ImGui::SliderFloat("Cohesion Distance", &cohesion_distance, 0.f, 2.0f);
        ImGui::SliderFloat("Alignment", &alignment_force, 0.0f, 0.1f);
        ImGui::SliderFloat("Alignment Distance", &alignment_distance, 0.0f, 2.0f);
        ImGui::SliderFloat("Séparation", &separation_force, 0.0f, 0.1f);
        ImGui::SliderFloat("Séparation Distance", &separation_distance, 0.0f, 2.0f);
        ImGui::End();
    };

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Blue);
        for (auto& boid : boids)
        {
            boid.draw(ctx);
            boid.movement();
            if (boid.borders_bool())
            {
                boid.cohesion(boids, cohesion_distance, cohesion_force);
                boid.alignment(boids, alignment_distance, alignment_force);
                boid.separation(boids, separation_distance, separation_force);
                boid.movement();
            }
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}