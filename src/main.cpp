#include <cstdlib>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include "Boid.hpp"
#include "GLBoid.hpp"
#include "Scene.hpp"
#include "glimac/TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
// #include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/matrix_transform_2d.hpp"
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
    auto ctx = p6::Context{{.title = "Sarah-Aude"}};
    ctx.maximize_window();

    Scene scene;
    scene.init_scene();
    // ------------------------------------------------------------------------------------s
    // ---------------------------------------------------------------

    // Paramètres
    float right_limit      = 5;
    float left_limit       = -5;
    float upper_limit      = 5;
    float lower_limit      = -5;
    float foreground_limit = 5;
    float background_limit = -5;
    int   nombre_boids     = 20;
    float size_boid        = 0.01f;

    float cohesion_strength   = 0.f;
    float cohesion_radius     = 1.f;
    float alignment_strength  = 0.f;
    float alignment_radius    = 1.f;
    float separation_strength = 0.f;
    float separation_radius   = 1.f;

    float  radius            = 0.5f;
    size_t segment_latitude  = 32;
    size_t segment_longitude = 16;

    scene.set_boids(creation_boids(nombre_boids, upper_limit, lower_limit, left_limit, right_limit, size_boid, foreground_limit, background_limit));

    scene.create_GLBoids(radius, segment_latitude, segment_longitude, scene.m_boids);

    bool imguiActive = false;
    // Paramètres IMGUI
    ctx.imgui = [&]() {
        ImGui::Begin("Paramètres");
        imguiActive = ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
        ImGui::Text("Configuration de l'affichage :");
        ImGui::SliderInt("Nombre de boids", &nombre_boids, 0, 100);
        ImGui::SliderFloat("size Boid", &radius, 0.01f, 0.8f);

        ImGui::Separator();
        ImGui::Text("Configuration du comportement des Boids :");
        ImGui::SetNextTreeNodeOpen(true);
        if (ImGui::CollapsingHeader("Cohésion"))
        {
            ImGui::SliderFloat("strength de cohésion", &cohesion_strength, 0.f, 0.6f);
            ImGui::SliderFloat("radius de cohésion", &cohesion_radius, 0.f, 5.0f);
        }
        ImGui::SetNextTreeNodeOpen(true);
        if (ImGui::CollapsingHeader("alignment"))
        {
            ImGui::SliderFloat("strength d'alignment", &alignment_strength, 0.0f, 0.6f);
            ImGui::SliderFloat("radius d'alignment", &alignment_radius, 0.0f, 5.0f);
        }
        ImGui::SetNextTreeNodeOpen(true);
        if (ImGui::CollapsingHeader("Séparation"))
        {
            ImGui::SliderFloat("strength de séparation", &separation_strength, 0.0f, 0.6f);
            ImGui::SliderFloat("radius de séparation", &separation_radius, 0.0f, 5.0f);
        }
        ImGui::End();

        // Ajout et suppression des boids
        int size_boids_vecteur = scene.get_number_boids(); // OK car size max = 100 < size max int
        if (size_boids_vecteur < nombre_boids)
        {
            int  nombre_de_boids_a_ajouter = nombre_boids - size_boids_vecteur;
            auto nouveaux_boids            = creation_boids(nombre_de_boids_a_ajouter, upper_limit, lower_limit, left_limit, right_limit, radius, foreground_limit, background_limit);
            scene.add_boids(nouveaux_boids, radius, segment_latitude, segment_longitude);
        }
        else if (size_boids_vecteur > nombre_boids)
        {
            int nombre_de_boids_a_enlever = size_boids_vecteur - nombre_boids;
            scene.remove_boids(nombre_de_boids_a_enlever);
        }
    };

    // ----------------------------------------------------------------------------
    TrackballCamera camera = TrackballCamera();

    bool Z = false;
    bool Q = false;
    bool S = false;
    bool D = false;
    // Declare your infinite update loop.
    ctx.update = [&]() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        if (Z)
        {
            camera.move_front(0.1);
        }
        if (Q)
        {
            camera.rotate_left(-0.3);
        }
        if (S)
        {
            camera.move_front(-0.1);
        }
        if (D)
        {
            camera.rotate_left(0.3);
        }
        if (camera.get_position().z < -foreground_limit || camera.get_position().z > -background_limit)
        {
            if (Z)
            {
                camera.move_front(-0.1);
            }
            if (S)
            {
                camera.move_front(0.1);
            }
        }

        for (auto& boid : scene.m_GLBoids)
        {
            boid.set_radius(radius);
        }

        scene.draw(ctx, camera);

        // Dessin de la sphère
        for (auto& boid : scene.m_GLBoids)
        {
            boid.boid.update(radius, ctx, scene.m_boids, cohesion_radius, cohesion_strength, alignment_radius, alignment_strength, separation_radius, separation_strength);
        }
    };

    ctx.key_pressed = [&Z, &Q, &S, &D, &imguiActive](const p6::Key& key) {
        if (!imguiActive)
        {
            // ... votre code actuel de manipulation des touches
            if (key.physical == GLFW_KEY_W)
            {
                Z = true;
            }
            if (key.physical == GLFW_KEY_A)
            {
                Q = true;
            }
            if (key.physical == GLFW_KEY_S)
            {
                S = true;
            }
            if (key.physical == GLFW_KEY_D)
            {
                D = true;
            }
        }
    };

    ctx.key_released = [&Z, &Q, &S, &D, &imguiActive](const p6::Key& key) {
        if (!imguiActive)
        {
            // ... votre code actuel de manipulation des touches
            if (key.physical == GLFW_KEY_W)
            {
                Z = false;
            }
            if (key.physical == GLFW_KEY_A)
            {
                Q = false;
            }
            if (key.physical == GLFW_KEY_S)
            {
                S = false;
            }
            if (key.physical == GLFW_KEY_D)
            {
                D = false;
            }
        }
    };

    ctx.mouse_dragged = [&camera, &imguiActive](const p6::MouseDrag& button) {
        if (!imguiActive)
        {
            camera.rotate_left(button.delta.x * 50);
            camera.rotate_up(-button.delta.y * 50);
        }
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    scene.delete_GLBoids_buffer();
}
