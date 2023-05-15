#include <cstdlib>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include "Boid.hpp"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/random.hpp"
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
    auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};
    ctx.maximize_window();

    GLuint vbo, vao;

    // ------------------ VBO ----------------------
    // Création
    glGenBuffers(1, &vbo);

    // Bind
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Remplissage
    float                                  radius           = 0.1f;
    size_t                                 segmentLatitude  = 32;
    size_t                                 segmentLongitude = 16;
    const std::vector<glimac::ShapeVertex> vertices         = glimac::sphere_vertices(radius, segmentLatitude, segmentLongitude);

    // Envoi des données
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);

    // Debind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // ---------------------------------------------

    // ----------------- VAO -----------------------
    // Création
    glGenVertexArrays(1, &vao);

    // Bind
    glBindVertexArray(vao);

    constexpr GLuint VERTEX_ATTR_POSITION = 0;
    constexpr GLuint VERTEX_ATTR_NORM     = 1;
    constexpr GLuint VERTEX_ATTR_UV       = 2;

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORM);
    glEnableVertexAttribArray(VERTEX_ATTR_UV);
    // ---------------------------------------------

    // Bind vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));
    glVertexAttribPointer(VERTEX_ATTR_NORM, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));
    glVertexAttribPointer(VERTEX_ATTR_UV, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    // Debind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    const p6::Shader shader = p6::load_shader(
        "assets/shaders/3D.vs.glsl",
        "assets/shaders/normals.fs.glsl"
    );

    // ------------- RÉCUPÉRATION DES LOCATIONS DES VARIABLES UNIFORMES -------------------
    GLuint uMVPMatrix    = glGetUniformLocation(shader.id(), "uMVPMatrix");
    GLuint uMVMatrix     = glGetUniformLocation(shader.id(), "uMVMatrix");
    GLuint uNormalMatrix = glGetUniformLocation(shader.id(), "uNormalMatrix");
    glEnable(GL_DEPTH_TEST);
    // ------------------------------------------------------------------------------------s
    // ---------------------------------------------------------------

    // Paramètres
    float limite_droite   = 5;
    float limite_gauche   = -5;
    float limite_haut     = 5;
    float limite_bas      = -5;
    float limite_devant   = 5;
    float limite_derriere = -5;
    int   nombre_boids    = 20;
    // float taille_boid     = 0.01f;

    float cohesion_force   = 0.f;
    float cohesion_rayon   = 1.f;
    float alignement_force = 0.f;
    float alignement_rayon = 1.f;
    float separation_force = 0.f;
    float separation_rayon = 1.f;

    std::vector<Boid>
        boids = creation_boids(nombre_boids, limite_haut, limite_bas, limite_gauche, limite_droite, radius, limite_devant, limite_derriere);

    bool etat_checkbox = false;
    // Paramètres IMGUI
    ctx.imgui = [&]() {
        ImGui::Begin("Paramètres");

        ImGui::Text("Configuration de l'affichage :");
        ImGui::SliderInt("Nombre de boids", &nombre_boids, 0, 100);
        ImGui::SliderFloat("Taille Boid", &radius, 0.01f, 0.3f);
        ImGui::Checkbox("Limiter le nombre de boids en fonction de la taille", &etat_checkbox);

        ImGui::Separator();
        ImGui::Text("Configuration du comportement des Boids :");
        ImGui::SetNextTreeNodeOpen(true);
        if (ImGui::CollapsingHeader("Cohésion"))
        {
            ImGui::SliderFloat("Force de cohésion", &cohesion_force, 0.f, 0.1f);
            ImGui::SliderFloat("Rayon de cohésion", &cohesion_rayon, 0.f, 0.1f);
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
            auto nouveaux_boids            = creation_boids(nombre_de_boids_a_ajouter, limite_haut, limite_bas, limite_gauche, limite_droite, radius, limite_devant, limite_derriere);
            boids.insert(boids.end(), nouveaux_boids.begin(), nouveaux_boids.end());
        }
        else if (taille_boids_vecteur > nombre_boids)
        {
            int nombre_de_boids_a_enlever = taille_boids_vecteur - nombre_boids;
            boids.erase(boids.end() - nombre_de_boids_a_enlever, boids.end());
        }
    };

    // ----------------------------------------------------------------------------

    // Declare your infinite update loop.
    ctx.update = [&]() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindVertexArray(vao);
        shader.use();

        glm::mat4 ProjMatrix;
        glm::mat4 MVMatrix;
        glm::mat4 NormalMatrix;

        ProjMatrix = glm::perspective(glm::radians(70.0f), ctx.aspect_ratio(), 0.1f, 100.f);

        NormalMatrix = glm::transpose(glm::inverse(glm::mat3(MVMatrix)));

        // Envoi des matrices au GPU
        glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        // Dessin de la sphère
        for (auto& boid : boids)
        {
            glm::vec3 translationVector(0.0f, 0.0f, -5.0f);
            MVMatrix = glm::translate(glm::mat4(1.0f), translationVector);

            MVMatrix = glm::scale(MVMatrix, glm::vec3{radius});
            MVMatrix = glm::translate(MVMatrix, boid.get_position() * 2.f); // Translation * Rotation * Translation

            // Envoi des matrices au GPU
            glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
            glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
            glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

            glDrawArrays(GL_TRIANGLES, 0, vertices.size());

            boid.update(radius, ctx, boids, cohesion_rayon, cohesion_force, alignement_rayon, alignement_force, separation_rayon, separation_force);
        }

        // Débind du VAO
        glBindVertexArray(0);
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
}

//////////////////////////////
/////// Sauvegarde 2D ////////
//////////////////////////////

// #include <cstdlib>
// #include <vector>
// #include "Boid.hpp"
// #include "glm/fwd.hpp"
// #include "imgui.h"
// #include "p6/p6.h"

// #define DOCTEST_CONFIG_IMPLEMENT
// #include "doctest/doctest.h"

// int main(int argc, char* argv[])
// {
//     { // Run the tests
//         if (doctest::Context{}.run() != 0)
//             return EXIT_FAILURE;
//         // The CI does not have a GPU so it cannot run the rest of the code.
//         const bool no_gpu_available = argc >= 2 && strcmp(argv[1], "-nogpu") == 0; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
//         if (no_gpu_available)
//             return EXIT_SUCCESS;
//     }

//     // Actual app
//     auto ctx = p6::Context{{.title = "Simple-p6-Setup"}};
//     ctx.maximize_window();

//     // Paramètres
//     float limite_gauche = -ctx.aspect_ratio();
//     float limite_droite = ctx.aspect_ratio();
//     float limite_haut   = 1;
//     float limite_bas    = -1;
//     int   nombre_boids  = 20;
//     float taille_boid   = 0.01f;

//     float cohesion_force   = 0.f;
//     float cohesion_rayon   = 0.5f;
//     float alignement_force = 0.f;
//     float alignement_rayon = 0.5f;
//     float separation_force = 0.f;
//     float separation_rayon = 0.5f;

//     std::vector<Boid>
//         boids = creation_boids(nombre_boids, limite_haut, limite_bas, limite_gauche, limite_droite, taille_boid);

//     bool etat_checkbox = false; // Variable pour stocker l'état de la checkbox
//     // Paramètres IMGUI
//     ctx.imgui = [&]() {
//         ImGui::Begin("Paramètres");

//         ImGui::Text("Configuration de l'affichage :");
//         ImGui::SliderInt("Nombre de boids", &nombre_boids, 0, 100);
//         ImGui::SliderFloat("Taille Boid", &taille_boid, 0.01f, 0.3f);
//         ImGui::Checkbox("Limiter le nombre de boids en fonction de la taille", &etat_checkbox);

//         ImGui::Separator();
//         ImGui::Text("Configuration du comportement des Boids :");
//         ImGui::SetNextTreeNodeOpen(true);
//         if (ImGui::CollapsingHeader("Cohésion"))
//         {
//             ImGui::SliderFloat("Force de cohésion", &cohesion_force, 0.f, 0.1f);
//             ImGui::SliderFloat("Rayon de cohésion", &cohesion_rayon, 0.f, 2.0f);
//         }
//         ImGui::SetNextTreeNodeOpen(true);
//         if (ImGui::CollapsingHeader("Alignement"))
//         {
//             ImGui::SliderFloat("Force d'alignement", &alignement_force, 0.0f, 0.1f);
//             ImGui::SliderFloat("Rayon d'alignement", &alignement_rayon, 0.0f, 2.0f);
//         }
//         ImGui::SetNextTreeNodeOpen(true);
//         if (ImGui::CollapsingHeader("Séparation"))
//         {
//             ImGui::SliderFloat("Force de séparation", &separation_force, 0.0f, 0.1f);
//             ImGui::SliderFloat("Rayon de séparation", &separation_rayon, 0.0f, 2.0f);
//         }
//         ImGui::End();

//         // Ajout et suppression des boids
//         int taille_boids_vecteur = boids.size(); // OK car taille max = 100 < taille max int
//         if (taille_boids_vecteur < nombre_boids)
//         {
//             int  nombre_de_boids_a_ajouter = nombre_boids - taille_boids_vecteur;
//             auto nouveaux_boids            = creation_boids(nombre_de_boids_a_ajouter, limite_haut, limite_bas, limite_gauche, limite_droite, taille_boid);
//             boids.insert(boids.end(), nouveaux_boids.begin(), nouveaux_boids.end());
//         }
//         else if (taille_boids_vecteur > nombre_boids)
//         {
//             int nombre_de_boids_a_enlever = taille_boids_vecteur - nombre_boids;
//             boids.erase(boids.end() - nombre_de_boids_a_enlever, boids.end());
//         }
//     };

//     // Declare your infinite update loop.
//     ctx.update = [&]() {
//         ctx.background({0.86f, 0.81f, 0.92f});

//         for (auto& boid : boids)
//         {
//             if (etat_checkbox)
//             {
//                 if (taille_boid > 0.2)
//                 {
//                     if (nombre_boids > 20)
//                     {
//                         nombre_boids = 20;
//                     }
//                 }
//             }
//             boid.update(taille_boid, ctx, boids, cohesion_rayon, cohesion_force, alignement_rayon, alignement_force, separation_rayon, separation_force);
//         }
//     };

//     // Should be done last. It starts the infinite loop.
//     ctx.start();
// }