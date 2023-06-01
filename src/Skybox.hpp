#pragma once

#include <vector>
#include "Boid.hpp"
#include "Util.hpp"
#include "glimac/common.hpp"

class Skybox {
    GLuint              m_vbo = 0;
    GLuint              m_vao = 0;
    std::vector<Mesh>   m_meshes;
    std::vector<GLuint> m_vaoAlgues;
    std::vector<GLuint> m_vboAlgues;
    std::vector<GLuint> m_iboAlgues;

public:
    Skybox() = default;
    void init();
    void draw(GLuint shaderId, GLuint colorIndex);
    ~Skybox();
};