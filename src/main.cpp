#include <cstdlib>
#include <vector>
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

    // Declare your infinite update loop.
    ctx.update = [&]() {
        ctx.background(p6::NamedColor::Blue);
        ctx.circle(
            // p6::Center{ctx.mouse()},
            // p6::Center{0.5f, 0.5f},
            // p6::Radius{0.2f}
            // p6::FullScreen{}
        );
        ctx.square(
            p6::FullScreen{}
        );
        p6::Point2D p1((ctx.aspect_ratio() + (-ctx.aspect_ratio())) / 2, 1);
        p6::Point2D p2(ctx.aspect_ratio() / 2, -1);
        p6::Point2D p3(-ctx.aspect_ratio() / 2, -1);
        ctx.triangle(p1, p2, p3);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
}