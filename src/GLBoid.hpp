#pragma once

#include "Boid.hpp"
#include "glimac/common.hpp"

class GLBoid {
private:
    float                            m_radius;
    size_t                           m_segmentLatitude;
    size_t                           m_segmentLongitude;
    GLuint                           m_vbo;
    GLuint                           m_vao;
    std::vector<glimac::ShapeVertex> m_vertices;

public:
    Boid boid;
    GLBoid(Boid boid, size_t segmentLatitude, size_t segmentLongitude)
        : boid(boid), m_segmentLatitude(segmentLatitude), m_segmentLongitude(segmentLongitude){};

    float getRadius();
    void  setRadius(float radius);

    void initVBO(size_t m_segmentLatitude, size_t m_segmentLongitude);
    void initVAO();
    void deleteVAO();
    void deleteVBO();
    void draw();
};