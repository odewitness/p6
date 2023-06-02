#pragma once
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
    static std::vector<Mesh> load(std::vector<GLuint>& vaoV, std::vector<GLuint>& vboV, std::vector<GLuint>& iboV, const std::string& input_file, const std::string& base_directory);

    static void render(const std::vector<Mesh>& meshes, const std::vector<GLuint>& vaoV, GLuint shader_id);
};