#include "GLBoid.hpp"
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"

void GLBoid::initVBO(size_t m_segmentLatitude, size_t m_segmentLongitude)
{
    // Création
    glGenBuffers(1, &m_vbo);

    // Bind
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // Remplissage

    this->m_vertices = glimac::sphere_vertices(1, m_segmentLatitude, m_segmentLongitude);

    // Envoi des données
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glimac::ShapeVertex), m_vertices.data(), GL_STATIC_DRAW);

    // Debind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

float GLBoid::getRadius()
{
    return m_radius;
}

void GLBoid::setRadius(float radius)
{
    m_radius = radius;
}

void GLBoid::initVAO()
{
    // Création
    glGenVertexArrays(1, &m_vao);

    // Bind
    glBindVertexArray(m_vao);

    constexpr GLuint VERTEX_ATTR_POSITION = 0;
    constexpr GLuint VERTEX_ATTR_NORM     = 1;
    constexpr GLuint VERTEX_ATTR_UV       = 2;

    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORM);
    glEnableVertexAttribArray(VERTEX_ATTR_UV);
    // ---------------------------------------------

    // Bind m_vbo
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));
    glVertexAttribPointer(VERTEX_ATTR_NORM, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));
    glVertexAttribPointer(VERTEX_ATTR_UV, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    // Debind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GLBoid::deleteVAO()
{
    glDeleteVertexArrays(1, &m_vao);
}

void GLBoid::deleteVBO()
{
    glDeleteBuffers(1, &m_vbo);
}

void GLBoid::draw()
{
    glBindVertexArray(m_vao);

    glDrawArrays(GL_TRIANGLES, 0, this->m_vertices.size());

    glBindVertexArray(0);
}