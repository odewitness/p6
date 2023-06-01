#include "TrackballCamera.hpp"
#include <iostream>
#include "glm/gtx/transform.hpp"
#include "glm/trigonometric.hpp"

TrackballCamera::TrackballCamera(float Distance, float AngleX, float AngleY)
    : m_Distance(Distance), m_AngleX(AngleX), m_AngleY(AngleY) {}

void TrackballCamera::moveFront(float delta)
{
    m_Distance += delta;
}

void TrackballCamera::rotateLeft(float degrees)
{
    m_AngleX += glm::radians(degrees);
}

void TrackballCamera::rotateUp(float degrees)
{
    m_AngleY += glm::radians(degrees);
}

void TrackballCamera::moveLeft(float t)
{
    m_left += t;
}

glm::vec3 TrackballCamera::getPosition()
{
    return glm::vec3(0.f, 0.f, -m_Distance);
}

glm::mat4 TrackballCamera::getViewMatrix() const
{
    glm::mat4 viewMatrix(1.f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.f, 0.f, m_Distance));
    viewMatrix = glm::rotate(viewMatrix, m_AngleY, glm::vec3(1.f, 0.f, 0.f));
    viewMatrix = glm::rotate(viewMatrix, m_AngleX, glm::vec3(0.f, 1.f, 0.f));
    return viewMatrix;
}

glm::mat4 TrackballCamera::getShiftedViewMatrix() const
{
    glm::mat4 viewMatrix(1.f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.f, 0.f, m_Distance));
    return viewMatrix;
}