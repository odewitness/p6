#pragma once
#include "glm/fwd.hpp"

class Camera {
private:
public:
    virtual glm::mat4 getViewMatrix() const     = 0;
    virtual void      moveFront(float delta)    = 0;
    virtual void      rotateLeft(float degrees) = 0;
    virtual void      rotateUp(float degrees)   = 0;
    virtual void      moveLeft(float t)         = 0;
    virtual glm::vec3 getPosition()             = 0;
};