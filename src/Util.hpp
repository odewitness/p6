#pragma once
#include <tiny_obj_loader.h>
#include <iostream>
#include <ostream>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct Mesh {
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    Material                  material{};
};

class Util {
public:
    static std::vector<Mesh> load(std::vector<GLuint>& vaoV, std::vector<GLuint>& vboV, std::vector<GLuint>& iboV, const std::string& input_file, const std::string& base_directory)
    {
        std::vector<Mesh>                meshes;
        tinyobj::attrib_t                attrib;
        std::vector<tinyobj::shape_t>    shapes;
        std::vector<tinyobj::material_t> materials;

        std::string err;
        std::string warn;
        bool        ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, input_file.c_str(), base_directory.c_str());

        if (!err.empty())
        {
            std::cerr << "Erreur lors du chargement du modèle : " << err << std::endl;
        }

        if (!ret)
        {
            std::cerr << "Erreur lors du chargement du modèle." << std::endl;
        }
        for (const auto& shape : shapes)
        {
            Mesh mesh;
            // Parcours des faces du modèle
            for (const auto& index : shape.mesh.indices)
            {
                Vertex vertex{};

                vertex.position = glm::vec3(
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                );
                vertex.normal = glm::vec3(
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                );

                if (index.texcoord_index == -1)
                {
                    vertex.texCoords = glm::vec2(0.0f);
                }
                else
                {
                    vertex.texCoords = glm::vec2(
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        attrib.texcoords[2 * index.texcoord_index + 1]
                    );
                }

                mesh.vertices.push_back(vertex);
                mesh.indices.push_back(mesh.indices.size());
            }

            if (!shape.mesh.material_ids.empty())
            {
                if (shape.mesh.material_ids[0] == -1)
                {
                    mesh.material.ambient  = glm::vec3(0.0f);
                    mesh.material.diffuse  = glm::vec3(0.0f);
                    mesh.material.specular = glm::vec3(0.0f);
                }
                else
                {
                    const auto& material = materials[shape.mesh.material_ids[0]];

                    mesh.material.ambient  = glm::vec3(material.ambient[0], material.ambient[1], material.ambient[2]);
                    mesh.material.diffuse  = glm::vec3(material.diffuse[0], material.diffuse[1], material.diffuse[2]);
                    mesh.material.specular = glm::vec3(material.specular[0], material.specular[1], material.specular[2]);
                }
            }
            // Création du Vertex Array Object (VAO)
            GLuint vao = 0;
            GLuint vbo = 0;
            GLuint ibo = 0;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            // Création du Vertex Buffer Object (VBO) pour les sommets
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(mesh.vertices.size() * sizeof(Vertex)), mesh.vertices.data(), GL_STATIC_DRAW);

            // Spécification des attributs de sommet
            glEnableVertexAttribArray(0); // Position du sommet
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
            glEnableVertexAttribArray(1); // Normale du sommet
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
            glEnableVertexAttribArray(2); // Coordonnées de texture du sommet
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

            // Création du Index Buf 1fer Object (IBO) pour les indices
            glGenBuffers(1, &ibo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(mesh.indices.size() * sizeof(unsigned int)), mesh.indices.data(), GL_STATIC_DRAW);

            // Ajout du mesh à la liste des meshes
            meshes.push_back(mesh);
            vaoV.push_back(vao);
            vboV.push_back(vbo);
            iboV.push_back(ibo);
        }
        return meshes;
    }

    static void render(const std::vector<Mesh>& meshes, const std::vector<GLuint>& vaoV, GLuint shader_id)
    {
        // Rendu de chaque mesh
        int i = 0;
        for (const auto& mesh : meshes)
        {
            glBindVertexArray(vaoV[i]);

            // Envoi des couleurs du matériau au shader
            glUniform3fv(glGetUniformLocation(shader_id, "uMaterialAmbient"), 1, glm::value_ptr(mesh.material.ambient));
            glUniform3fv(glGetUniformLocation(shader_id, "uMaterialDiffuse"), 1, glm::value_ptr(mesh.material.diffuse));
            glUniform3fv(glGetUniformLocation(shader_id, "uMaterialSpecular"), 1, glm::value_ptr(mesh.material.specular));

            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.indices.size()), GL_UNSIGNED_INT, nullptr);

            glBindVertexArray(0);
        }
    }
};