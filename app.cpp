#include "pch.h"
#include "app.h"
#include "ray_tracer.h"

App::App(uint32_t width, uint32_t height, std::string title)
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

    SDL_Surface* surface = SDL_GetWindowSurface(m_window);
    m_format = surface->format;
    m_pixels = surface->pixels;

    p_rayTracer = new RayTracer {};
}

App::~App()
{
    delete p_rayTracer;
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void App::Run()
{
    Render();
    HandleEvent();
}

void App::Render(void)
{
    std::cout << "Render Start\n";
    uint64_t start = SDL_GetTicks64();

    for (uint32_t y = 0; y < m_height; y++) {
        for (uint32_t x = 0; x < m_width; x++) {
            glm::vec3 worldPos = TransformScreenToWorld({ x, y });
            WritePixel(x, y, p_rayTracer->CastRay(worldPos));
        }
    }

    std::cout << "Render End: " << (SDL_GetTicks64() - start) * 0.001 << " sec\n";
    SDL_UpdateWindowSurface(m_window);
}

void App::HandleEvent(void)
{
    std::cout << "\nPress ESC to exit.\n";
    std::cout << "Press C to capture image to BMP.\n";

    SDL_Event event;
    while (1) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return;
            }

            const uint8_t* keyboardState = SDL_GetKeyboardState(NULL);

            if (keyboardState[SDL_SCANCODE_ESCAPE]) {
                return;
            }

            if (keyboardState[SDL_SCANCODE_C]) {
                CaptureImageToBMP();
                return;
            }
        }
    }
}

void App::WritePixel(uint32_t x, uint32_t y, const glm::ivec3& color)
{
    uint32_t index = x + y * m_width;
    uint32_t* pixels = static_cast<uint32_t*>(m_pixels);
    pixels[index] = SDL_MapRGB(m_format, color.r, color.g, color.b);
}

glm::vec3 App::TransformScreenToWorld(const glm::vec2& screenPos)
{
    float worldX = 2.0f * screenPos.x / m_width - 1.0f;
    worldX *= m_aspect;
    float worldY = 1.0f - 2.0f * screenPos.y / m_height;

    return glm::vec3(worldX, worldY, 0.0f);
}

void App::CaptureImageToBMP(void)
{
    std::string filename = "screenshot.bmp";

    if (SDL_SaveBMP(SDL_GetWindowSurface(m_window), filename.c_str()) == 0) {
        std::cout << filename << " created\n";
    } else {
        std::cout << "Failed to save image: " << SDL_GetError() << '\n';
    }
}
