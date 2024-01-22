#pragma once

// SDL
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// C++
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <optional>

class BaseObject;

// direction vector must be a unit vector.
struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
};

struct Intersection {
    float distance;
    glm::vec3 point;
    glm::vec3 normal;
    BaseObject* pObject;
};

struct Material {
    glm::vec3 ambient { 0.1f };
    glm::vec3 diffuse { 0.9f };
    glm::vec3 specular { 0.9f };
    uint32_t shininess { 16 };
    float reflectance {};
};