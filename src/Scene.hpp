#pragma once

#include <vector>
#include "Boid.hpp"
#include "GLBoid.hpp"
#include "Skybox.hpp"
#include "Util.hpp"
#include "glimac/TrackballCamera.hpp"
#include "glimac/common.hpp"
#include "p6/p6.h"

class Scene {
private:
    glm::mat4 m_ProjMatrix;
    glm::mat4 m_MVMatrix;
    glm::mat4 m_NormalMatrix;
    glm::vec3 m_colorVec;

    GLuint m_uMVPMatrix;
    GLuint m_uMVMatrix;
    GLuint m_uNormalMatrix;
    GLuint m_color;
    GLuint m_uSecondLightPosition;
    GLuint m_uSecondLightColor;
    GLuint m_uUseShadow;
    GLuint m_uMaterialAmbient;
    GLuint m_uMaterialDiffuse;
    GLuint m_uMaterialSpecular;
    GLuint m_uMaterialShininess;

    p6::Shader m_shader = p6::load_shader(
        "assets/shaders/3D.vs.glsl",
        "assets/shaders/normals.fs.glsl"
    );

    class Personnage {
        std::vector<Mesh>   m_meshes;
        std::vector<GLuint> m_vao;
        std::vector<GLuint> m_vbo;
        std::vector<GLuint> m_ibo;

    public:
        Personnage() = default;
        void init();
        void draw(GLuint shaderId);
        ~Personnage();
    };

    Skybox     m_bordures;
    Personnage m_personnage;

public:
    std::vector<GLBoid> m_GLboids;
    std::vector<Boid>   m_boids;

    Scene() = default;

    void setBoids(std::vector<Boid> boid);

    int  getNumberBoids();
    void addBoids(std::vector<Boid>& boids, float radius, size_t segmentLatitude, size_t segmentLongitude);
    void removeBoids(int nombre_de_boids_a_enlever);

    void createGLBoids(float radius, size_t segmentLatitude, size_t segmentLongitude, std::vector<Boid> boids);

    void draw(p6::Context& ctx, TrackballCamera& camera);

    void initScene();

    void deleteGLBoidsBuffer();
};