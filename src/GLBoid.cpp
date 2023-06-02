#include "GLBoid.hpp"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"

float GLBoid::get_radius()
{
    return m_radius;
}

void GLBoid::set_radius(float radius)
{
    m_radius = radius;
}

GLBoid::GLBoid(Boid boid, size_t segment_latitude, size_t segment_longitude)
    : boid(boid), m_segment_latitude(segment_latitude), m_segment_longitude(segment_longitude)
{
    std::string input_file     = "C:/Users/audep/Downloads/IMACCours/IMAC2/ProgrammationObjet/p6/assets/shell.obj";
    std::string base_directory = "C:/Users/audep/Downloads/IMACCours/IMAC2/ProgrammationObjet/p6/assets/";
    m_meshes                   = Util::load(m_vao, m_vbo, m_ibo, input_file, base_directory);
};

void GLBoid::delete_vertex()
{
    for (auto& vbo : m_vbo)
        glDeleteBuffers(1, &vbo);
    for (auto& vao : m_vao)
        glDeleteVertexArrays(1, &vao);
    for (auto& ibo : m_ibo)
        glDeleteVertexArrays(1, &ibo);
}

void GLBoid::draw(GLuint shader_id)
{
    Util::render(m_meshes, m_vao, shader_id);
}
