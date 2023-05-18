#pragma once

#include <vector>
#include "Boid.hpp"
#include "GLBoid.hpp"
#include "glimac/common.hpp"
#include "p6/p6.h"

class Scene {
private:
    glm::mat4 m_ProjMatrix;
    glm::mat4 m_MVMatrix;
    glm::mat4 m_NormalMatrix;

    GLuint m_uMVPMatrix;
    GLuint m_uMVMatrix;
    GLuint m_uNormalMatrix;

    p6::Shader m_shader = p6::load_shader(
        "assets/shaders/3D.vs.glsl",
        "assets/shaders/normals.fs.glsl"
    );

public:
    std::vector<GLBoid>
                      m_GLboids;
    std::vector<Boid> m_boids;
    Scene() = default;

    void setBoids(std::vector<Boid> boid);
    int  getNumberBoids();

    void addBoids(std::vector<Boid>& boids, float radius, size_t segmentLatitude, size_t segmentLongitude);

    void removeBoids(int nombre_de_boids_a_enlever);

    void createGLBoids(float radius, size_t segmentLatitude, size_t segmentLongitude, std::vector<Boid> boids);

    void draw(p6::Context& ctx);

    void initScene();

    void deleteGLBoidsBuffer();
};