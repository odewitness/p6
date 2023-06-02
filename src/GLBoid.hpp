#pragma once

#include "Boid.hpp"
#include "Util.hpp"
#include "glimac/common.hpp"

class GLBoid {
private:
    float                            m_radius;
    size_t                           m_segment_latitude;
    size_t                           m_segment_longitude;
    std::vector<Mesh>                m_meshes;
    std::vector<GLuint>              m_vao;
    std::vector<GLuint>              m_vbo;
    std::vector<GLuint>              m_ibo;
    std::vector<glimac::ShapeVertex> m_vertices;

public:
    Boid boid;
    GLBoid(Boid boid, size_t segment_latitude, size_t segment_longitude);

    float get_radius() const;
    void  set_radius(float radius);

    void init();
    void delete_vertex();
    void draw(GLuint shader_id);
};