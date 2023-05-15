#pragma once

#include <glm/glm.hpp>

class TrackballCamera {
private:
    float m_Distance;
    float m_AngleX;
    float m_AngleY;

public:
    explicit TrackballCamera(float Distance = 0.f, float AngleX = 0, float AngleY = 0);

    void moveFront(float delta);
    void rotateLeft(float degrees);
    void rotateUp(float degrees);

    glm::mat4 getViewMatrix() const;
};