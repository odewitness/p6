#pragma once

#include <glm/glm.hpp>
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"

class FreeflyCamera {
private:
    glm::vec3 m_Position;
    float     m_Phi;
    float     m_Theta;
    glm::vec3 m_FrontVector;
    glm::vec3 m_LeftVector;
    glm::vec3 m_UpVector;

public:
    FreeflyCamera(glm::vec3 Position = glm::vec3(0, 0, 0), float Phi = glm::pi<float>(), float Theta = 0); // call compute
    FreeflyCamera(const FreeflyCamera& camera);

    void      computeDirectionVectors();
    void      moveLeft(float t);
    void      moveFront(float t);
    void      rotateLeft(float degrees);
    void      rotateUp(float degrees);
    glm::mat4 getViewMatrix() const;
};