#include "frontends.h"
#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define CELL_SIZE 20

SDL_Window *window;
SDL_Renderer *renderer;

void gui_frontend_init(void) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf(stderr, "Failed to initialize gui: %s", SDL_GetError());
    exit(1);
  }
  SDL_SetWindowTitle(window, "Jod8");

  SDL_CreateWindowAndRenderer(800, 600, 0, &window, &renderer);
}

void gui_frontend_draw_framebuffer(uint8_t framebuffer[32 * 64]) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  for (size_t y = 0; y < 32; y++) {
    for (size_t x = 0; x < 64; x++) {
      uint8_t byte = framebuffer[x + (y * 64)];
      for (size_t j = 0; j < 8; j++) {
        if (byte & (1 << (7 - j))) {
          SDL_Rect rect = (SDL_Rect){.h = CELL_SIZE,
                                     .w = CELL_SIZE,
                                     .x = (CELL_SIZE * (x + j)) + CELL_SIZE,
                                     .y = CELL_SIZE * y};
          SDL_RenderFillRect(renderer, &rect);
        }
      }
    }
  }
  SDL_RenderPresent(renderer);
}

void gui_frontend_cleanup(void) {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
