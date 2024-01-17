#include "pch.h"
#include "app.h"
#include "screen.h"
#include "camera.h"
#include "ray_tracer.h"

App::App(uint32_t width, uint32_t height, const std::string& title)
{
    p_screen = new Screen { width, height, title };
    p_camera = new Camera { p_screen };
    p_rayTracer = new RayTracer {};
}

App::~App()
{
    delete p_rayTracer;
    delete p_camera;
    delete p_screen;
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

    for (uint32_t y = 0; y < p_screen->GetHeight(); y++) {
        for (uint32_t x = 0; x < p_screen->GetWidth(); x++) {
            Ray ray = p_camera->GetRay({ x, y });
            p_screen->WritePixel(x, y, p_rayTracer->TraceRay(ray, 5));
        }
    }

    std::cout << "Render End: " << (SDL_GetTicks64() - start) * 0.001 << " sec\n";

    p_screen->UpdateSurface();
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
                p_screen->CaptureImageToBMP();
                return;
            }
        }
    }
}
