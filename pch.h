#pragma once

// SDL
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

// glm
#include <glm/glm.hpp>

// C++
#include <string>
#include <iostream>
#include <vector>

class BaseObject;

// direction vector must be a unit vector.
struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
};

struct Intersection {
    float distatnce;
    BaseObject* pObject;
};