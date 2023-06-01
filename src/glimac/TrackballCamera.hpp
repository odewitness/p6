#pragma once

#include <glm/glm.hpp>
#include "Camera.hpp"
class TrackballCamera : public Camera {
private:
    float m_Distance;
    float m_AngleX;
    float m_AngleY;
    float m_left = 0.;

public:
    explicit TrackballCamera(float Distance = 0.f, float AngleX = 0, float AngleY = 0);

    void      moveFront(float delta) override;
    void      rotateLeft(float degrees) override;
    void      rotateUp(float degrees) override;
    void      moveLeft(float t) override;
    glm::vec3 getPosition() override;

    glm::mat4 getViewMatrix() const override;
    glm::mat4 getShiftedViewMatrix() const;
};