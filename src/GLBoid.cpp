#include "GLBoid.hpp"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"

float GLBoid::getRadius()
{
    return m_radius;
}

void GLBoid::setRadius(float radius)
{
    m_radius = radius;
}

GLBoid::GLBoid(Boid boid, size_t segmentLatitude, size_t segmentLongitude)
    : boid(boid), m_segmentLatitude(segmentLatitude), m_segmentLongitude(segmentLongitude)
{
    std::string inputfile = "C:/Users/audep/Downloads/IMACCours/IMAC2/ProgrammationObjet/p6/assets/shell.obj";
    std::string base_dir  = "C:/Users/audep/Downloads/IMACCours/IMAC2/ProgrammationObjet/p6/assets/";
    m_meshes              = Util::load(m_vao, m_vbo, m_ibo, inputfile, base_dir);
};

void GLBoid::deleteVertex()
{
    for (auto& vbo : m_vbo)
        glDeleteBuffers(1, &vbo);
    for (auto& vao : m_vao)
        glDeleteVertexArrays(1, &vao);
    for (auto& ibo : m_ibo)
        glDeleteVertexArrays(1, &ibo);
}

void GLBoid::draw(GLuint shaderId)
{
    Util::render(m_meshes, m_vao, shaderId);
}
