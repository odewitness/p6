#pragma once
#include "glm/fwd.hpp"

class Camera {
private:
public:
    virtual glm::mat4 get_view_matrix() const    = 0;
    virtual void      move_front(float delta)    = 0;
    virtual void      rotate_left(float degrees) = 0;
    virtual void      rotate_up(float degrees)   = 0;
    virtual void      move_left(float t)         = 0;
    virtual glm::vec3 get_position()             = 0;
};