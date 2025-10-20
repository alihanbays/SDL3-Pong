#include <SDL3/SDL.h>
#include <string>

constexpr int ScreenWidth {640};
constexpr int ScreenHeight {480};
SDL_Window* Window {nullptr}; 
SDL_Surface* ScreenSurface {nullptr};
SDL_Surface* HelloWorld {nullptr};


bool init() {
    bool success {true};

    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        SDL_Log("SDL could not be initilized! SDL error: %s\n", SDL_GetError());
        success = false;
    } else {
        Window = SDL_CreateWindow("Pong", ScreenWidth, ScreenHeight, 0);

        if (Window == nullptr) {
            SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
        }

        ScreenSurface = SDL_GetWindowSurface(Window);
    }

    return success;
}

bool loadMedia() {
    bool success {true};

    std::string imagePath {"./hello-sdl3.bmp"};

    HelloWorld = SDL_LoadBMP(imagePath.c_str()) ;

    if (HelloWorld == nullptr) {
        SDL_Log("Unable to load image! SDL error: %s\n", SDL_GetError());
    }

    return success;
}

void close() {
    SDL_DestroySurface(HelloWorld);
    HelloWorld = nullptr;

    SDL_DestroyWindow(Window);
    Window = nullptr;
    ScreenSurface = nullptr;

    SDL_Quit();
}

int main(int argc, char* args[]) {
    int exitCode {0};

    if (init() == false) {
        SDL_Log("Unable to initialize program!\n");
        exitCode = 1;
        close();
        return exitCode;
    }

    if (loadMedia() == false) {
        SDL_Log("Unable to load media!\n");
        exitCode = 2;
        close();
        return exitCode;
    }
    
    bool quit {false};
    SDL_Event event;
    SDL_zero(event);

    while (quit == false) {
        while (SDL_PollEvent(&event) == true) { // If there are events to process
            if (event.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        // Color it white and add the picture
        uint32_t colorWhite = SDL_MapSurfaceRGB(ScreenSurface, 0xFF, 0xFF, 0xFF);

        SDL_FillSurfaceRect(ScreenSurface, nullptr, colorWhite);
        SDL_BlitSurface(HelloWorld, nullptr, ScreenSurface, nullptr);

        SDL_UpdateWindowSurface(Window);
    }

    close();
    return exitCode;
}