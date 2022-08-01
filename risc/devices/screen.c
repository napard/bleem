/*
 * screen.c
 * Screen output device.
 *
 * 24 jul 2022 -- 11:44 -03
 * Notes:
 */

#include "risc.h"

static const char* THIS_FILE = "screen.c";

extern risc_vm_t* g_cpu;

static void move_cursor(uint32_t pX, uint32_t pY) {
    //printf("\x1b[%d;%dH", pY, pX);
}

static void clear_screen() {
    //printf("\x1b[2J");
}

static uint8_t SCREEN_initialize() {
#ifdef RISC_SCREEN_DEVICE_SDL2
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        risc_panic(g_cpu, "unable to initialize SDL2 video! SDL_Error: %s\n",
            THIS_FILE, __LINE__, SDL_GetError());
    }
    
    g_cpu->display.sdl2_window = SDL_CreateWindow("",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        RISC_SCREEN_DEVICE_COLUMNS * RISC_SCREEN_TILE_PIXELS,
        RISC_SCREEN_DEVICE_ROWS * RISC_SCREEN_TILE_PIXELS,
        SDL_WINDOW_SHOWN);
    
    if(!g_cpu->display.sdl2_window) {
        risc_panic(g_cpu, "display window could not be created! SDL_Error: %s\n",
            THIS_FILE, __LINE__, SDL_GetError());
    }
    
    g_cpu->display.sdl2_renderer = SDL_CreateRenderer(g_cpu->display.sdl2_window, -1,
        RISC_SDL2_RENDERER_FLAGS);
    g_cpu->display.sdl2_window_surface = SDL_GetWindowSurface(g_cpu->display.sdl2_window);

    SDL_SetRenderDrawColor(g_cpu->display.sdl2_renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(g_cpu->display.sdl2_renderer);
    SDL_RenderPresent(g_cpu->display.sdl2_renderer);

    // Test...
    uint32_t* pixels = g_cpu->display.sdl2_window_surface->pixels;
    for (uint32_t x = 0; x < 8; x++) {
        for (uint32_t y = 0; y < 8; y++) {
            pixels[x + y * RISC_SCREEN_DEVICE_COLUMNS * RISC_SCREEN_TILE_PIXELS] =
                SDL_MapRGBA(g_cpu->display.sdl2_window_surface->format, 255, 255, 255, 255);
        }
    }
    SDL_UpdateWindowSurface(g_cpu->display.sdl2_window);
#endif /* RISC_SCREEN_DEVICE_SDL2 */

    return 0;
}

static uint8_t SCREEN_finalize() {
    SDL_DestroyRenderer(g_cpu->display.sdl2_renderer);
    SDL_DestroyWindow(g_cpu->display.sdl2_window);
    SDL_Quit();
    return 0;
}

uint8_t SCREEN_read_byte(VMWORD pAddr) {
    return 0;
}

void SCREEN_write_byte(uint8_t pData, VMWORD pAddr) {

}

VMWORD SCREEN_read_word(VMWORD pAddr) {
    return 0;
}

void SCREEN_write_word(VMWORD pData, VMWORD pAddr) {
    /*uint8_t c = pData & 0xff;
    uint8_t cmd = (pData >> 8 & 0xff);
    
    switch(cmd) {
    case 0xff:
        clear_screen();
        break;
    
    default:
        break;
    }
    
    uint8_t x = (pAddr % RISC_SCREEN_DEVICE_COLUMNS) + 1;
    uint8_t y = (pAddr / RISC_SCREEN_DEVICE_COLUMNS) + 1;
    move_cursor(x * 2, y);
    fwrite(&c, 1, 1, stdout);*/
}

risc_device_t device_SCREEN = {
    "SCREEN",
    SCREEN_initialize,
    SCREEN_finalize,
    SCREEN_read_byte,
    SCREEN_write_byte,
    SCREEN_read_word,
    SCREEN_write_word
};

