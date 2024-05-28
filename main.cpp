#include <SDL2/SDL.h>
#include <complex>
#include <iostream>

SDL_Color computeColor(std::complex<double> c) {
  std::complex<double> z = 0;
  const int maxIterations = 1000;
  int iterations = 0;

  while (std::abs(z) <= 2.0 && iterations < maxIterations) {
    z = z * z + c;
    iterations++;
  }

  SDL_Color color;
  if (iterations == maxIterations) {
    color = {0, 0, 0, 255};
  } else {
    color = {static_cast<Uint8>(iterations % 256),
             static_cast<Uint8>((iterations * 5) % 256),
             static_cast<Uint8>((iterations * 13) % 256), 255};
  }
  return color;
}

void drawMandelbrot(SDL_Renderer *renderer, int width, int height,
                    double centerX, double centerY, double zoom) {
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {

      double real = (x - width / 2.0) / zoom + centerX;
      double imag = (y - height / 2.0) / zoom + centerY;
      std::complex<double> c(real, imag);
      SDL_Color color = computeColor(c);
      SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
      SDL_RenderDrawPoint(renderer, x, y);
    }
  }
}

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  SDL_Window *window =
      SDL_CreateWindow("Mandelbrot", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, 1000, 1000, SDL_WINDOW_SHOWN);
  if (!window) {
    std::cerr << "SDL_CreateWindow error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    std::cerr << "SDL_CreateRenderer error: " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  bool quit = false;
  SDL_Event event;

  double centerX = -0.5;
  double centerY = 0.0;
  double zoom = 200.0; // Adjust this value for different zoom levels

  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    drawMandelbrot(renderer, 1000, 1000, centerX, centerY, zoom);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
