#include "camera.h"
#include "components/camera.h"

#include <glm/ext/matrix_clip_space.hpp>

using namespace world_of_tanks;


Camera::Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up) {
    Set(position, center, up);
}


void Camera::Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up) {
    position_ = position;
    forward_ = glm::normalize(center - position);
    right_ = glm::cross(forward_, up);
    up_ = glm::cross(right_, forward_);
}

void Camera::MoveForward(float distance) {
    const glm::vec3 dir = glm::normalize(glm::vec3(forward_.x, 0, forward_.z));
    position_ += dir * distance;
}

void Camera::RotateFirstOY(const float angle) {
    
    const glm::vec4 new_forward = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward_, 1);
    forward_ = glm::normalize(glm::vec3(new_forward));
            
    const glm::vec4 new_right = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right_, 1);
    right_ = glm::normalize(glm::vec3(new_right));

    up_ = glm::cross(right_, forward_);
}

void Camera::RotateThirdOY(const float angle, glm::vec3 player_tank_position) {
    float distance = glm::distance(glm::vec2(player_tank_position.x, player_tank_position.z), glm::vec2(position_.x, position_.z));
    
    MoveForward(distance);
    RotateFirstOY(angle);
    MoveForward(-distance);
    
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(position_, position_ + forward_, up_);
}

glm::mat4 Camera::GetProjectionMatrix() const {
    return projection_matrix_;
}

void Camera::SetProjectionMatrix(const float aspect_ratio, const float fov_y, const float z_near, const float z_far) {
    projection_matrix_ = glm::perspective(fov_y, aspect_ratio, z_near, z_far);
}
