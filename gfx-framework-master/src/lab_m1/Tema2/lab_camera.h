#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"


namespace implemented
{
    class Camera
    {
    public:
        Camera()
        {
            position = glm::vec3(0, 2, 5);
            forward = glm::vec3(0, 0, -1);
            up = glm::vec3(0, 1, 0);
            right = glm::vec3(1, 0, 0);
            distanceToTarget = 2;
        }

        Camera(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            Set(position, center, up);
        }

        ~Camera()
        { }


        void SetDependingTank(const glm::vec3& target) {
            glm::vec3 newPosition = target - forward * distanceToTarget;
            newPosition.y = position.y;
            position = newPosition;
            forward = glm::normalize(target - position);
            right = glm::cross(forward, up);
            this->up = glm::cross(right, forward);
        }

        // Update camera
        void Set(const glm::vec3& position, const glm::vec3& center, const glm::vec3& up)
        {
            this->position = position;
            forward = glm::normalize(center - position);
            right = glm::cross(forward, up);
            this->up = glm::cross(right, forward);
        }

        void MoveForward(float distance)
        {
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            position += dir * distance;
        }

        void TranslateForward(float distance)
        {
            // TODO(student): Translate the camera using the `forward` vector.
            // What's the difference between `TranslateForward()` and
            // `MoveForward()`?
            position += glm::normalize(forward) * distance;

        }

        void TranslateUpward(float distance)
        {
            // TODO(student): Translate the camera using the `up` vector.
            position += distance * glm::normalize(up);

        }

        void TranslateRight(float distance)
        {
            position += distance * glm::normalize(right);

        }

        void RotateFirstPerson_OX(float angle)
        {
            // TODO(student): Compute the new `forward` and `up` vectors.
            // Don't forget to normalize the vectors! Use `glm::rotate()`.
            forward = glm::normalize(glm::rotate(glm::mat4(1.f), angle, right) * glm::vec4(forward, 0));
            up = glm::normalize(glm::cross(right, forward));

        }

        void RotateFirstPerson_OY(float angle)
        {
            // TODO(student): Compute the new `forward`, `up` and `right`
            // vectors. Use `glm::rotate()`. Don't forget to normalize the
            // vectors!
            forward = glm::normalize(glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 0));
            right = glm::normalize(glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 0));
            up = glm::normalize(glm::cross(right, forward));
        }

        void RotateFirstPerson_OZ(float angle)
        {
            // TODO(student): Compute the new `right` and `up`. This time,
            // `forward` stays the same. Use `glm::rotate()`. Don't forget
            // to normalize the vectors!
            right = glm::normalize(glm::rotate(glm::mat4(1.f), angle, forward) * glm::vec4(right, 0));
            up = glm::normalize(glm::cross(right, forward));
            // forward = glm::cross(up, right);

        }

        void RotateThirdPerson_OX(float angle)
        {
            // TODO(student): Rotate the camera in third-person mode around
            // the OX axis. Use `distanceToTarget` as translation distance.
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OX(angle);
            TranslateForward(-distanceToTarget);

        }

        void RotateThirdPerson_OY(float angle)
        {
            // TODO(student): Rotate the camera in third-person mode around
            // the OY axis
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OY(angle);
            TranslateForward(-distanceToTarget);

        }

        void RotateThirdPerson_OZ(float angle)
        {
            // TODO(student): Rotate the camera in third-person mode around
            // the OZ axis.
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OZ(angle);
            TranslateForward(-distanceToTarget);

        }

        glm::mat4 GetViewMatrix()
        {
            // Returns the view matrix
            return glm::lookAt(position, position + forward, up);
        }

        glm::vec3 GetTargetPosition()
        {
            return position + forward * distanceToTarget;
        }

    public:
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
    };
}   // namespace implemented
