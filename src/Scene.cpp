#include "Scene.hpp"
#include <vcruntime.h>
#include <vector>
#include "GLBoid.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

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
    // m_GLboids = std::vector<GLBoid>();

    for (auto& boid : boids)
    {
        m_GLboids.emplace_back(GLBoid(boid, segmentLatitude, segmentLongitude));
        m_GLboids.back().initVBO(segmentLatitude, segmentLongitude);
        m_GLboids.back().initVAO();
    }
}

void Scene::initScene()
{
    // ------------- RÉCUPÉRATION DES LOCATIONS DES VARIABLES UNIFORMES -------------------
    m_uMVPMatrix    = glGetUniformLocation(m_shader.id(), "uMVPMatrix");
    m_uMVMatrix     = glGetUniformLocation(m_shader.id(), "uMVMatrix");
    m_uNormalMatrix = glGetUniformLocation(m_shader.id(), "uNormalMatrix");

    glEnable(GL_DEPTH_TEST);
}

void Scene::draw(p6::Context& ctx)
{
    m_shader.use();

    m_ProjMatrix   = glm::mat4(1.0f);
    m_MVMatrix     = glm::mat4(1.0f);
    m_NormalMatrix = glm::mat4(1.0f);

    m_ProjMatrix = glm::perspective(glm::radians(70.0f), ctx.aspect_ratio(), 0.1f, 100.f);

    m_NormalMatrix = glm::transpose(glm::inverse(glm::mat3(m_MVMatrix)));

    // Envoi des matrices au GPU
    glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(m_ProjMatrix * m_MVMatrix));
    glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE, glm::value_ptr(m_MVMatrix));
    glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE, glm::value_ptr(m_NormalMatrix));

    for (auto& boid : m_GLboids)
    {
        glm::vec3 translationVector(0.0f, 0.0f, -5.0f);
        m_MVMatrix = glm::translate(glm::mat4(1.0f), translationVector);

        m_MVMatrix = glm::translate(m_MVMatrix, boid.boid.get_position() * 2.f); // Translation * Rotation * Translation
        m_MVMatrix = glm::scale(m_MVMatrix, glm::vec3{boid.getRadius()});
        std::cout << boid.getRadius() << std::endl;

        // Envoi des matrices au GPU
        glUniformMatrix4fv(m_uMVPMatrix, 1, GL_FALSE, glm::value_ptr(m_ProjMatrix * m_MVMatrix));
        glUniformMatrix4fv(m_uMVMatrix, 1, GL_FALSE, glm::value_ptr(m_MVMatrix));
        glUniformMatrix4fv(m_uNormalMatrix, 1, GL_FALSE, glm::value_ptr(m_NormalMatrix));

        boid.draw();
    }
}

void Scene::deleteGLBoidsBuffer()
{
    for (auto& boid : m_GLboids)
    {
        boid.deleteVAO();
        boid.deleteVBO();
    }
}