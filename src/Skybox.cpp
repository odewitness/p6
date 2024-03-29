#include "Skybox.hpp"
#include <array>
void Skybox::init()
{
    float         limite_droite   = 5;
    float         limite_gauche   = -5;
    float         limite_haut     = 5;
    float         limite_bas      = -5;
    float         limite_devant   = 5;
    float         limite_derriere = -5;
    const GLfloat vertices[]      = {
        limite_droite,
        limite_bas,
        limite_derriere,
        limite_droite,
        limite_haut,
        limite_derriere,
        limite_gauche,
        limite_bas,
        limite_derriere,

        limite_gauche,
        limite_haut,
        limite_derriere,
        limite_droite,
        limite_haut,
        limite_derriere,
        limite_gauche,
        limite_bas,
        limite_derriere,

        limite_droite,
        limite_bas,
        limite_devant,
        limite_droite,
        limite_haut,
        limite_devant,
        limite_gauche,
        limite_bas,
        limite_devant,

        limite_gauche,
        limite_haut,
        limite_devant,
        limite_droite,
        limite_haut,
        limite_devant,
        limite_gauche,
        limite_bas,
        limite_devant,

        limite_gauche,
        limite_bas,
        limite_devant,
        limite_gauche,
        limite_bas,
        limite_derriere,
        limite_gauche,
        limite_haut,
        limite_derriere,

        limite_droite,
        limite_bas,
        limite_devant,
        limite_droite,
        limite_bas,
        limite_derriere,
        limite_droite,
        limite_haut,
        limite_derriere,

        limite_gauche,
        limite_bas,
        limite_devant,
        limite_gauche,
        limite_haut,
        limite_devant,
        limite_gauche,
        limite_haut,
        limite_derriere,

        limite_droite,
        limite_bas,
        limite_devant,
        limite_droite,
        limite_haut,
        limite_devant,
        limite_droite,
        limite_haut,
        limite_derriere,

        limite_gauche,
        limite_bas,
        limite_derriere,
        limite_gauche,
        limite_bas,
        limite_devant,
        limite_droite,
        limite_bas,
        limite_devant,

        limite_gauche,
        limite_bas,
        limite_derriere,
        limite_droite,
        limite_bas,
        limite_derriere,
        limite_droite,
        limite_bas,
        limite_devant,

        limite_gauche,
        limite_haut,
        limite_derriere,
        limite_gauche,
        limite_haut,
        limite_devant,
        limite_droite,
        limite_haut,
        limite_devant,

        limite_gauche,
        limite_haut,
        limite_derriere,
        limite_droite,
        limite_haut,
        limite_derriere,
        limite_droite,
        limite_haut,
        limite_devant,

    };
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glBindVertexArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::string input_file     = "assets/Algues.obj";
    std::string base_directory = "assets/";
    m_meshes                   = Util::load(m_vaoAlgues, m_vboAlgues, m_iboAlgues, input_file, base_directory);
}

void Skybox::draw(GLuint shader_id, GLuint colorIndex)
{
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 4 * 3);
    glUniform3f(colorIndex, 0.f, 0.706f, 0.847f);
    glDrawArrays(GL_TRIANGLES, 4 * 3, 4 * 3);
    glUniform3f(colorIndex, 0.87f, 0.72f, 0.57f);
    glDrawArrays(GL_TRIANGLES, 8 * 3, 2 * 3);
    glUniform3f(colorIndex, 0.678f, 0.910f, 0.957);
    glDrawArrays(GL_TRIANGLES, 10 * 3, 2 * 3);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glUniform3f(colorIndex, 0.3765f, 0.4235f, 0.2196f);
    Util::render(m_meshes, m_vaoAlgues, shader_id);
}

Skybox::~Skybox()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
    for (auto& vbo : m_vboAlgues)
        glDeleteBuffers(1, &vbo);
    for (auto& vao : m_vaoAlgues)
        glDeleteVertexArrays(1, &vao);
    for (auto& ibo : m_iboAlgues)
        glDeleteVertexArrays(1, &ibo);
}
