#pragma once

#include "Boid.hpp"
#include "Util.hpp"
#include "glimac/common.hpp"

class GLBoid {
private:
    float                            m_radius;
    size_t                           m_segmentLatitude;
    size_t                           m_segmentLongitude;
    std::vector<Mesh>                m_meshes;
    std::vector<GLuint>              m_vao;
    std::vector<GLuint>              m_vbo;
    std::vector<GLuint>              m_ibo;
    std::vector<glimac::ShapeVertex> m_vertices;

public:
    Boid boid;
    GLBoid(Boid boid, size_t segmentLatitude, size_t segmentLongitude);

    float getRadius();
    void  setRadius(float radius);

    void init();
    void deleteVertex();
    void draw(GLuint shaderId);
};