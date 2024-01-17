#include "pch.h"
#include "camera.h"
#include "screen.h"

Camera::Camera(const Screen* pScreen)
    : p_screen { pScreen }
{
    viewportHeight = 2.0f;
    viewportWidth = viewportHeight * p_screen->GetAspect();

    glm::vec3 viewportX { viewportWidth, 0.0f, 0.0f };
    glm::vec3 viewportY { 0.0f, -viewportHeight, 0.0f };

    dx = viewportX / static_cast<float>(p_screen->GetWidth());
    dy = viewportY / static_cast<float>(p_screen->GetHeight());

    viewportOrigin = eyePos + eyeFront * focalLength - viewportX * 0.5f - viewportY * 0.5f;
}

Ray Camera::GetRay(const glm::vec2& screenPos)
{
    glm::vec3 pixelCenter { viewportOrigin + (0.5f + screenPos.x) * dx + (0.5f + screenPos.y) * dy };
    glm::vec3 dir { pixelCenter - eyePos };

    return Ray { pixelCenter, glm::normalize(dir) };
}
