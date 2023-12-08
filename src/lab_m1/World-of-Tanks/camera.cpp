#include "camera.h"
#include "components/camera.h"
#include "components/camera.h"
#include "components/camera.h"
#include "components/camera.h"

#include <glm/ext/matrix_clip_space.hpp>

#include "tank.h"

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

void Camera::RotateOY(const float angle) {
    const glm::vec4 new_forward = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward_, 1);
    forward_ = glm::normalize(glm::vec3(new_forward));
            
    const glm::vec4 new_right = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right_, 1);
    right_ = glm::normalize(glm::vec3(new_right));

    up_ = glm::cross(right_, forward_);
}

void Camera::RotateThirdOY(const float angle, glm::vec3 player_tank_position) {
    float distance = glm::distance(glm::vec2(player_tank_position.x, player_tank_position.z), glm::vec2(position_.x, position_.z));
    // float distance = glm::distance(player_tank_position, position_);
    
    MoveForward(distance);
    RotateOY(angle);
    MoveForward(-distance);
    
}

// void Camera::MoveForward(float distance) {
//     // Translates the camera using the `dir` vector computed from
//     // `forward`. Movement will always keep the camera at the same
//     // height. For example, if you rotate your head up/down, and then
//     // walk forward, then you will still keep the same relative
//     // distance (height) to the ground!
//     glm::vec3 dir = glm::normalize(glm::vec3(forward_.x, 0, forward_.z));
//     position_ += dir * distance;
// }
//
// void Camera::TranslateForward(float distance) {
//     // TODO(student): Translate the camera using the `forward` vector.
//     // What's the difference between `TranslateForward()` and
//     // `MoveForward()`?
//     glm::vec3 dir = glm::normalize(glm::vec3(forward_.x, forward_.y, forward_.z));
//     position_ += dir * distance;
// }
//
// void Camera::TranslateUpward(float distance) {
//     // TODO(student): Translate the camera using the `up` vector.
//     glm::vec3 dir = glm::normalize(glm::vec3(0, up_.y, 0));
//     position_ += dir * distance;
// }
//
// void Camera::TranslateRight(float distance) {
//     // TODO(student): See instructions below. Read the entire thing!
//     // You need to translate the camera using the `right` vector.
//     // Usually, however, translation using camera's `right` vector
//     // is not very useful, because if the camera is rotated around the
//     // `forward` vector, then the translation on the `right` direction
//     // will have an undesired effect, more precisely, the camera will
//     // get closer or farther from the ground. The solution is to
//     // actually use the projected `right` vector (projected onto the
//     // ground plane), which makes more sense because we will keep the
//     // same distance from the ground plane.
//     // glm::vec3 dir = glm::normalize(glm::vec3(right.x, 0, 0));
//     // position += dir * distance;
//
//     position_ += glm::normalize(right_) * distance;
// }
//
// void Camera::RotateFirstPerson_OX(float angle) {
//     // TODO(student): Compute the new `forward` and `up` vectors.
//     // Don't forget to normalize the vectors! Use `glm::rotate()`.
//     // get the rotate vec4 vector
//     glm::vec4 newVector = glm::rotate(glm::mat4(1.0f), angle, right_) * glm::vec4(forward_, 1);
//     forward_ = glm::normalize(glm::vec3(newVector));
//
//     // glm::vec4 newVectorUp = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(1, 0, 0)) * glm::vec4(up, 1);
//     // up = glm::normalize(glm::vec3(newVectorUp));
//     up_ = glm::cross(right_, forward_);
// }
//
// void Camera::RotateFirstPerson_OY(float angle) {
//     // TODO(student): Compute the new `forward`, `up` and `right`
//     // vectors. Use `glm::rotate()`. Don't forget to normalize the
//     // vectors!
//     glm::vec4 newVector = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward_, 1);
//     forward_ = glm::normalize(glm::vec3(newVector));
//
//     // glm::vec4 newVectorUp = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(up, 1);
//     // up = glm::normalize(glm::vec3(newVectorUp));
//
//     glm::vec4 newVectorR = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right_, 1);
//     right_ = glm::normalize(glm::vec3(newVectorR));
//
//     up_ = glm::cross(right_, forward_);
// }
//
// void Camera::RotateFirstPerson_OZ(float angle) {
//     // TODO(student): Compute the new `right` and `up`. This time,
//     // `forward` stays the same. Use `glm::rotate()`. Don't forget
//     // to normalize the vectors!
//     glm::vec4 newVectorUp = glm::rotate(glm::mat4(1.0f), angle, forward_) * glm::vec4(up_, 1);
//     up_ = glm::normalize(glm::vec3(newVectorUp));
//
//     glm::vec4 newVectorR = glm::rotate(glm::mat4(1.0f), angle, forward_) * glm::vec4(right_, 1);
//     right_ = glm::normalize(glm::vec3(newVectorR));
// }
//
// void Camera::RotateThirdPerson_OX(float angle) {
//     // TODO(student): Rotate the camera in third-person mode around
//     // the OX axis. Use `distanceToTarget` as translation distance.
//     TranslateForward(distance_to_target_);
//     RotateFirstPerson_OX(angle);
//     TranslateForward(-distance_to_target_);
// }
//
// void Camera::RotateThirdPerson_OY(float angle) {
//     // TODO(student): Rotate the camera in third-person mode around
//     // the OY axis.
//     TranslateForward(distance_to_target_);
//     RotateFirstPerson_OY(angle);
//     TranslateForward(-distance_to_target_);
// }
//
// void Camera::RotateThirdPerson_OZ(float angle) {
//     // TODO(student): Rotate the camera in third-person mode around
//     // the OZ axis.
//     TranslateForward(distance_to_target_);
//     RotateFirstPerson_OZ(angle);
//     TranslateForward(-distance_to_target_);
// }

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(position_, position_ + forward_, up_);
}

glm::mat4 Camera::GetProjectionMatrix() const {
    return projection_matrix_;
}

void Camera::SetProjectionMatrix(const float aspect_ratio, const float fov_y, const float z_near, const float z_far) {
    projection_matrix_ = glm::perspective(fov_y, aspect_ratio, z_near, z_far);
}

// glm::vec3 Camera::GetTargetPosition() {
//     return position_ + forward_ * distance_to_target_;
// }
