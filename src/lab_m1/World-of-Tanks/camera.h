#pragma once
#include <glm/vec3.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "constants.h"
#include "tank.h"

namespace world_of_tanks {
    class Camera {
     private:
        glm::vec3 position_;
        glm::vec3 forward_;
        glm::vec3 right_;
        glm::vec3 up_;

        glm::mat4 projection_matrix_;
        glm::mat4 view_matrix_;

    public:
        Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up);
        
        void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up);
        void UpdatePosition(const Tank &player_tank);
        void MoveForward(float distance);
        void RotateOY(float angle);
        void RotateThirdOY(const float angle, glm::vec3 player_tank_position);
        void SetProjectionMatrix(float aspect_ratio, float fov_y = DEFAULT_FOV_Y, float z_near = DEFAULT_Z_NEAR,
                                 float z_far = DEFAULT_Z_FAR);

        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjectionMatrix() const;
        
        // void MoveForward(float distance);
        // void TranslateForward(float distance);
        // void TranslateUpward(float distance);
        // void TranslateRight(float distance);
        // void RotateFirstPerson_OX(float angle);
        // void RotateFirstPerson_OY(float angle);
        // void RotateFirstPerson_OZ(float angle);
        // void RotateThirdPerson_OX(float angle);
        // void RotateThirdPerson_OY(float angle);
        // void RotateThirdPerson_OZ(float angle);
        // glm::vec3 GetTargetPosition();
    };
}
