#pragma once

#include <glm/glm.hpp>
#include "Camera.hpp"
class TrackballCamera : public Camera {
private:
    float m_distance;
    float m_angle_x;
    float m_angle_Y;
    float m_left = 0.;

public:
    explicit TrackballCamera(float distance = 0.f, float angle_x = 0, float angle_y = 0);

    void      move_front(float delta) override;
    void      rotate_left(float degrees) override;
    void      rotate_up(float degrees) override;
    void      move_left(float t) override;
    glm::vec3 get_position() override;

    glm::mat4 get_view_matrix() const override;
    glm::mat4 get_shifted_view_matrix() const;
};