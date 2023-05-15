
#include "FreeflyCamera.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"

FreeflyCamera::FreeflyCamera(glm::vec3 Position, float Phi, float Theta)
    : m_Position(Position), m_Phi(Phi), m_Theta(Theta)
{
    computeDirectionVectors();
}

FreeflyCamera::FreeflyCamera(const FreeflyCamera& camera)
    : m_Position(camera.m_Position), m_Phi(camera.m_Phi), m_Theta(camera.m_Theta)
{
    computeDirectionVectors();
}

void FreeflyCamera::computeDirectionVectors()
{
    m_FrontVector = glm::vec3(glm::cos(m_Theta) * glm::sin(m_Phi), glm::sin(m_Theta), glm::cos(m_Theta) * glm::cos(m_Phi));

    m_LeftVector = glm::vec3(glm::sin(m_Phi + (glm::pi<float>() / 2.f)), 0, glm::cos(m_Phi + (glm::pi<float>() / 2.f)));

    m_UpVector = glm::cross(m_FrontVector, m_LeftVector);
}

void FreeflyCamera::moveLeft(float t)
{
    m_Position += t * m_LeftVector;
}

void FreeflyCamera::moveFront(float t)
{
    m_Position += t * m_FrontVector;
}

void FreeflyCamera::rotateLeft(float degrees)
{
    m_Phi += glm::radians(degrees);
    computeDirectionVectors();
}

void FreeflyCamera::rotateUp(float degrees)
{
    m_Theta += glm::radians(degrees);
    computeDirectionVectors();
}

glm::mat4 FreeflyCamera::getViewMatrix() const
{
    return glm::lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
}