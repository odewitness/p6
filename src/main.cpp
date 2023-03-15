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

    float left_limit   = -ctx.aspect_ratio();
    float right_limit  = ctx.aspect_ratio();
    float top_limit    = 1;
    float bottom_limit = -1;

    std::vector<Boid> boids = Boid::create_boids(10, top_limit, bottom_limit, left_limit, right_limit);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Blue);
        for (auto& boid : boids)
        {
            boid.draw(ctx);
            if (boid.borders_bool())
            {
                boid.position();
            }
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}