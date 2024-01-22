#include "pch.h"
#include "screen.h"

Screen::Screen(uint32_t width, uint32_t height, const std::string& title)
    : m_width { width }
    , m_height { height }
    , m_aspect { static_cast<float>(width) / height }
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Failed to initialize SDL: " << SDL_GetError();
        exit(EXIT_FAILURE);
    }

    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, 0);

    if (!m_window) {
        std::cout << "Failed to create window: " << SDL_GetError();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    m_surface = SDL_GetWindowSurface(m_window);
    m_format = m_surface->format;
    m_pixels = m_surface->pixels;
}

Screen::~Screen()
{
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Screen::WritePixel(uint32_t x, uint32_t y, const glm::vec3& color)
{
    glm::vec3 c { glm::clamp(color, 0.0f, 1.0f) };
    c *= 255.0f;

    uint32_t index = x + y * m_width;
    uint32_t* pixels = static_cast<uint32_t*>(m_pixels);
    pixels[index] = SDL_MapRGB(m_format, static_cast<uint8_t>(c.r), static_cast<uint8_t>(c.g), static_cast<uint8_t>(c.b));
}

void Screen::UpdateSurface(void)
{
    SDL_UpdateWindowSurface(m_window);
}

void Screen::CaptureImageToBMP(void)
{
    std::string filename = "screenshot.bmp";

    if (SDL_SaveBMP(m_surface, filename.c_str()) == 0) {
        std::cout << filename << " created\n";
    } else {
        std::cout << "Failed to save image: " << SDL_GetError() << '\n';
    }
}
