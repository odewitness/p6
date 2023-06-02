#include "TrackballCamera.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/trigonometric.hpp"

TrackballCamera::TrackballCamera(float distance, float angle_x, float angle_y)
    : m_distance(distance), m_angle_x(angle_x), m_angle_Y(angle_y) {}

void TrackballCamera::move_front(float delta)
{
    m_distance += delta;
}

void TrackballCamera::rotate_left(float degrees)
{
    m_angle_x += glm::radians(degrees);
}

void TrackballCamera::rotate_up(float degrees)
{
    m_angle_Y += glm::radians(degrees);
}

void TrackballCamera::move_left(float t)
{
    m_left += t;
}

glm::vec3 TrackballCamera::get_position()
{
    const float x = 0.0f;
    const float y = 0.0f;
    const float z = -m_distance;
    return glm::vec3(x, y, z);
}

glm::mat4 TrackballCamera::get_view_matrix() const
{
    glm::mat4 view_matrix(1.f);
    view_matrix = glm::translate(view_matrix, glm::vec3(0.f, 0.f, m_distance));
    view_matrix = glm::rotate(view_matrix, m_angle_Y, glm::vec3(1.f, 0.f, 0.f));
    view_matrix = glm::rotate(view_matrix, m_angle_x, glm::vec3(0.f, 1.f, 0.f));
    return view_matrix;
}

glm::mat4 TrackballCamera::get_shifted_view_matrix() const
{
    glm::mat4 view_matrix(1.f);
    view_matrix = glm::translate(view_matrix, glm::vec3(0.f, 0.f, m_distance));
    return view_matrix;
}