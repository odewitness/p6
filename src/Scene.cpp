#include "Scene.hpp"
// #include <vcruntime.h>
#include <vector>
#include "GLBoid.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

void Scene::Personnage::init()
{
    std::string inputfile = "C:/Users/audep/Downloads/IMACCours/IMAC2/ProgrammationObjet/p6/assets/Fish.obj";
    std::string base_dir  = "C:/Users/audep/Downloads/IMACCours/IMAC2/ProgrammationObjet/p6/assets/";
    m_meshes              = Util::load(m_vao, m_vbo, m_ibo, inputfile, base_dir);
}

void Scene::Personnage::draw(GLuint shaderId)
{
    Util::render(m_meshes, m_vao, shaderId);
}

Scene::Personnage::~Personnage()
{
    for (auto& vbo : m_vbo)
        glDeleteBuffers(1, &vbo);
    for (auto& vao : m_vao)
        glDeleteVertexArrays(1, &vao);
    for (auto& ibo : m_ibo)
        glDeleteVertexArrays(1, &ibo);
}

void Scene::setBoids(std::vector<Boid> boid)
{
    m_boids = boid;
}

int Scene::getNumberBoids()
{
    return m_boids.size();
}

void Scene::addBoids(std::vector<Boid>& boids, float radius, size_t segmentLatitude, size_t segmentLongitude)
{
    createGLBoids(radius, segmentLatitude, segmentLongitude, boids);
    m_boids.insert(m_boids.end(), boids.begin(), boids.end());
}

void Scene::removeBoids(int nombre_de_boids_a_enlever)
{
    m_boids.erase(m_boids.end() - nombre_de_boids_a_enlever, m_boids.end());
    m_GLboids.erase(m_GLboids.end() - nombre_de_boids_a_enlever, m_GLboids.end());
}

void Scene::createGLBoids(float radius, size_t segmentLatitude, size_t segmentLongitude, std::vector<Boid> boids)
{
    for (auto& boid : boids)
    {
        m_GLboids.emplace_back(GLBoid(boid, segmentLatitude, segmentLongitude));
    }
}

void Scene::initScene()
{
    // ------------- RÉCUPÉRATION DES LOCATIONS DES VARIABLES UNIFORMES -------------------
    m_uMVPMatrix           = glGetUniformLocation(m_shader.id(), "uMVPMatrix");
    m_uMVMatrix            = glGetUniformLocation(m_shader.id(), "uMVMatrix");
    m_uNormalMatrix        = glGetUniformLocation(m_shader.id(), "uNormalMatrix");
    m_color                = glGetUniformLocation(m_shader.id(), "color");
    m_uSecondLightPosition = glGetUniformLocation(m_shader.id(), "uSecondLightPosition");
    m_uSecondLightColor    = glGetUniformLocation(m_shader.id(), "uSecondLightColor");
    m_uUseShadow           = glGetUniformLocation(m_shader.id(), "uUseShadow");
    m_uMaterialAmbient     = glGetUniformLocation(m_shader.id(), "uMaterialAmbient");
    m_uMaterialDiffuse     = glGetUniformLocation(m_shader.id(), "uMaterialDiffuse");
    m_uMaterialSpecular    = glGetUniformLocation(m_shader.id(), "uMaterialSpecular");
    m_uMaterialShininess   = glGetUniformLocation(m_shader.id(), "uMaterialShininess");
    m_bordures             = Skybox();
    m_bordures.init();
    m_personnage.init();
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
    m_MVMatrix     = camera.getShiftedViewMatrix();
    m_MVMatrix     = glm::translate(m_MVMatrix, camera.getPosition()); // Translation * Rotation * Translation

    glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(m_ProjMatrix * m_MVMatrix));
    glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE, glm::value_ptr(m_MVMatrix));
    glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE, glm::value_ptr(m_NormalMatrix));

    glUniform3f(m_uSecondLightPosition, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
    glUniform3f(m_uSecondLightColor, 0.9882f, 0.6392f, 0.0667f);

    glUniform1i(m_uUseShadow, 1);
    m_personnage.draw(m_shader.id());

    glUniform1i(m_uUseShadow, 0);
    glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(m_ProjMatrix * camera.getViewMatrix() * m_MVMatrix));
    glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE, glm::value_ptr(m_MVMatrix));
    glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE, glm::value_ptr(m_NormalMatrix));
    colors[0] = 0.f;
    colors[1] = 1.f;
    glUniform3f(m_color, 0.f, 0.588f, 0.780f);
    m_bordures.draw(m_shader.id(), m_color);

    colors[1] = 0.f;
    glUniform3f(m_color, 0.f, 0.f, 0.0f);
    glUniform1i(m_uUseShadow, 1);
    glUniform3f(m_uMaterialAmbient, 1.f, 0.5f, 0.3f);
    glUniform3f(m_uMaterialDiffuse, 1.f, 0.5f, 0.3f);
    glUniform3f(m_uMaterialSpecular, 0.5f, 0.5f, 0.5f);
    glUniform1f(m_uMaterialShininess, 32.0f);

    for (auto& boid : m_GLboids)
    {
        m_MVMatrix = glm::mat4(1.0f);
        m_MVMatrix = glm::translate(m_MVMatrix, boid.boid.get_position()); // Translation * Rotation * Translation
        m_MVMatrix = glm::scale(m_MVMatrix, glm::vec3{boid.getRadius()});

        glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(m_ProjMatrix * camera.getViewMatrix() * m_MVMatrix));
        glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE, glm::value_ptr(m_MVMatrix));
        glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE, glm::value_ptr(m_NormalMatrix));

        boid.draw(m_shader.id());
    }
}

void Scene::deleteGLBoidsBuffer()
{
    for (auto& boid : m_GLboids)
        boid.deleteVertex();
}