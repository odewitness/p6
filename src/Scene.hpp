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

    GLuint m_uMVPMatrix;
    GLuint m_uMVMatrix;
    GLuint m_uNormalMatrix;
    GLuint m_uColor;
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

    class Character {
        std::vector<Mesh>   m_meshes;
        std::vector<GLuint> m_vao;
        std::vector<GLuint> m_vbo;
        std::vector<GLuint> m_ibo;

    public:
        Character() = default;
        void init();
        void draw(GLuint shader_id);
        ~Character();
    };

    Skybox    m_skybox;
    Character m_character;

public:
    std::vector<GLBoid> m_GLBoids;
    std::vector<Boid>   m_boids;

    Scene() = default;

    void set_boids(const std::vector<Boid>& boid);

    int  get_number_boids() const;
    void add_boids(std::vector<Boid>& boids, size_t segment_latitude, size_t segment_longitude);
    void remove_boids(int nombre_de_boids_a_enlever);

    void create_GLBoids(size_t segment_latitude, size_t segment_longitude, std::vector<Boid>& boids);

    void draw(p6::Context& ctx, TrackballCamera& camera);

    void init_scene();

    void delete_GLBoids_buffer();
};