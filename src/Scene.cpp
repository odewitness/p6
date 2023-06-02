#include "Scene.hpp"
// #include <vcruntime.h>
#include <utility>
#include <vector>
#include "GLBoid.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

void Scene::Character::init()
{
    std::string input_file     = "C:/Users/audep/Downloads/IMACCours/IMAC2/ProgrammationObjet/p6/assets/Fish.obj";
    std::string base_directory = "C:/Users/audep/Downloads/IMACCours/IMAC2/ProgrammationObjet/p6/assets/";
    m_meshes                   = Util::load(m_vao, m_vbo, m_ibo, input_file, base_directory);
}

void Scene::Character::draw(GLuint shader_id)
{
    Util::render(m_meshes, m_vao, shader_id);
}

Scene::Character::~Character()
{
    for (auto& vbo : m_vbo)
        glDeleteBuffers(1, &vbo);
    for (auto& vao : m_vao)
        glDeleteVertexArrays(1, &vao);
    for (auto& ibo : m_ibo)
        glDeleteVertexArrays(1, &ibo);
}

void Scene::set_boids(std::vector<Boid> boid)
{
    m_boids = std::move(boid);
}

int Scene::get_number_boids() const
{
    return m_boids.size();
}

void Scene::add_boids(std::vector<Boid>& boids, float radius, size_t segment_latitude, size_t segment_longitude)
{
    create_GLBoids(radius, segment_latitude, segment_longitude, boids);
    m_boids.insert(m_boids.end(), boids.begin(), boids.end());
}

void Scene::remove_boids(int nombre_de_boids_a_enlever)
{
    m_boids.erase(m_boids.end() - nombre_de_boids_a_enlever, m_boids.end());
    m_GLBoids.erase(m_GLBoids.end() - nombre_de_boids_a_enlever, m_GLBoids.end());
}

void Scene::create_GLBoids(float radius, size_t segment_latitude, size_t segment_longitude, std::vector<Boid> boids)
{
    for (auto& boid : boids)
    {
        m_GLBoids.emplace_back(GLBoid(boid, segment_latitude, segment_longitude));
    }
}

void Scene::init_scene()
{
    // ------------- RÉCUPÉRATION DES LOCATIONS DES VARIABLES UNIFORMES -------------------
    m_uMVPMatrix           = glGetUniformLocation(m_shader.id(), "uMVPMatrix");
    m_uMVMatrix            = glGetUniformLocation(m_shader.id(), "uMVMatrix");
    m_uNormalMatrix        = glGetUniformLocation(m_shader.id(), "uNormalMatrix");
    m_uColor               = glGetUniformLocation(m_shader.id(), "uColor");
    m_uSecondLightPosition = glGetUniformLocation(m_shader.id(), "uSecondLightPosition");
    m_uSecondLightColor    = glGetUniformLocation(m_shader.id(), "uSecondLightColor");
    m_uUseShadow           = glGetUniformLocation(m_shader.id(), "uUseShadow");
    m_uMaterialAmbient     = glGetUniformLocation(m_shader.id(), "uMaterialAmbient");
    m_uMaterialDiffuse     = glGetUniformLocation(m_shader.id(), "uMaterialDiffuse");
    m_uMaterialSpecular    = glGetUniformLocation(m_shader.id(), "uMaterialSpecular");
    m_uMaterialShininess   = glGetUniformLocation(m_shader.id(), "uMaterialShininess");
    m_skybox               = Skybox();
    m_skybox.init();
    m_character.init();
    glEnable(GL_DEPTH_TEST);
}

void Scene::draw(p6::Context& ctx, TrackballCamera& camera)
{
    GLfloat colors[3] = {1.f, 0.f, 0.0f};
    m_shader.use();

    m_ProjMatrix   = glm::mat4(1.0f);
    m_MVMatrix     = glm::mat4(1.0f);
    m_NormalMatrix = glm::mat4(1.0f);

    m_ProjMatrix   = glm::perspective(glm::radians(70.0f), ctx.aspect_ratio(), 0.1f, 100.f);
    m_NormalMatrix = glm::transpose(glm::inverse(glm::mat3(m_MVMatrix)));
    m_MVMatrix     = camera.get_shifted_view_matrix();
    m_MVMatrix     = glm::translate(m_MVMatrix, camera.get_position()); // Translation * Rotation * Translation

    glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(m_ProjMatrix * m_MVMatrix));
    glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE, glm::value_ptr(m_MVMatrix));
    glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE, glm::value_ptr(m_NormalMatrix));

    glUniform3f(m_uSecondLightPosition, camera.get_position().x, camera.get_position().y, camera.get_position().z);
    glUniform3f(m_uSecondLightColor, 0.9882f, 0.6392f, 0.0667f);

    glUniform1i(m_uUseShadow, 1);
    m_character.draw(m_shader.id());

    glUniform1i(m_uUseShadow, 0);
    glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(m_ProjMatrix * camera.get_view_matrix() * m_MVMatrix));
    glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE, glm::value_ptr(m_MVMatrix));
    glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE, glm::value_ptr(m_NormalMatrix));
    colors[0] = 0.f;
    colors[1] = 1.f;
    glUniform3f(m_uColor, 0.f, 0.588f, 0.780f);
    m_skybox.draw(m_shader.id(), m_uColor);

    colors[1] = 0.f;
    glUniform3f(m_uColor, 0.f, 0.f, 0.0f);
    glUniform1i(m_uUseShadow, 1);
    glUniform3f(m_uMaterialAmbient, 1.f, 0.5f, 0.3f);
    glUniform3f(m_uMaterialDiffuse, 1.f, 0.5f, 0.3f);
    glUniform3f(m_uMaterialSpecular, 0.5f, 0.5f, 0.5f);
    glUniform1f(m_uMaterialShininess, 32.0f);

    for (auto& boid : m_GLBoids)
    {
        m_MVMatrix = glm::mat4(1.0f);
        m_MVMatrix = glm::translate(m_MVMatrix, boid.boid.get_position()); // Translation * Rotation * Translation
        m_MVMatrix = glm::scale(m_MVMatrix, glm::vec3{boid.get_radius()});

        glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(m_ProjMatrix * camera.get_view_matrix() * m_MVMatrix));
        glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE, glm::value_ptr(m_MVMatrix));
        glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE, glm::value_ptr(m_NormalMatrix));

        boid.draw(m_shader.id());
    }
}

void Scene::delete_GLBoids_buffer()
{
    for (auto& boid : m_GLBoids)
        boid.delete_vertex();
}