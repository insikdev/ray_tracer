#include "pch.h"
#include "camera.h"

Ray Camera::GetRay(const glm::vec3& worldPos)
{
    glm::mat4 mat = glm::lookAtLH(position, at, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::vec3 origin = worldPos + position;
    glm::vec3 dir = glm::normalize(worldPos - position);
    dir = glm::mat3(mat) * dir;

    return Ray { origin, dir };
}
