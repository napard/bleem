/*
 * video.c
 * Video output device.
 *
 * 24 jul 2022 -- 11:44 -03
 * Notes:
 */

#include "risc.h"

static const char* THIS_FILE = "video.c";

extern risc_vm_t* g_cpu;

static void refresh() {
    SDL_UpdateTexture(g_cpu->display.sdl2_screen_texture,
        NULL, g_cpu->display.sdl2_screen_texture,
        RISC_VIDEO_DEVICE_COLUMNS * RISC_VIDEO_TILE_PIXELS * sizeof(uint32_t));
    SDL_Rect sr = {
        .x = 0,
        .y = 0,
        .w = RISC_VIDEO_DEVICE_COLUMNS * RISC_VIDEO_TILE_PIXELS,
        .h = RISC_VIDEO_DEVICE_ROWS * RISC_VIDEO_TILE_PIXELS
    };
    SDL_Rect dr = {
        .x = 0,
        .y = 0,
        .w = RISC_VIDEO_DEVICE_COLUMNS * RISC_VIDEO_TILE_PIXELS * RISC_VIDEO_SCALER,
        .h = RISC_VIDEO_DEVICE_ROWS * RISC_VIDEO_TILE_PIXELS * RISC_VIDEO_SCALER
    };
    SDL_RenderCopyEx(g_cpu->display.sdl2_renderer,
        g_cpu->display.sdl2_screen_texture, &sr, &dr, 0, NULL,
        SDL_FLIP_NONE);
    SDL_RenderPresent(g_cpu->display.sdl2_renderer);
}

/* ---------------------------------------------------------------------- */

static uint8_t VIDEORAM_initialize() {
#ifdef RISC_VIDEO_DEVICE_SDL2
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        risc_panic(g_cpu, "unable to initialize SDL2 video! SDL_Error: %s\n",
            THIS_FILE, __LINE__, SDL_GetError());
    }
    
    g_cpu->display.sdl2_window = SDL_CreateWindow("",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        RISC_VIDEO_DEVICE_COLUMNS * RISC_VIDEO_TILE_PIXELS * RISC_VIDEO_SCALER,
        RISC_VIDEO_DEVICE_ROWS * RISC_VIDEO_TILE_PIXELS * RISC_VIDEO_SCALER,
        SDL_WINDOW_SHOWN);
    
    if(!g_cpu->display.sdl2_window) {
        risc_panic(g_cpu, "display window could not be created! SDL_Error: %s\n",
            THIS_FILE, __LINE__, SDL_GetError());
    }
    
    g_cpu->display.sdl2_renderer = SDL_CreateRenderer(g_cpu->display.sdl2_window, -1,
        RISC_SDL2_RENDERER_FLAGS);
    g_cpu->display.sdl2_screen_texture = SDL_CreateTexture(g_cpu->display.sdl2_renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        RISC_VIDEO_DEVICE_COLUMNS * RISC_VIDEO_TILE_PIXELS,
        RISC_VIDEO_DEVICE_ROWS * RISC_VIDEO_TILE_PIXELS);
    g_cpu->display.screen_pixels = RISC_MALLOC(
        RISC_VIDEO_DEVICE_COLUMNS * RISC_VIDEO_TILE_PIXELS *
        RISC_VIDEO_DEVICE_ROWS * RISC_VIDEO_TILE_PIXELS * sizeof(uint32_t));

    SDL_SetRenderDrawColor(g_cpu->display.sdl2_renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(g_cpu->display.sdl2_renderer);
    SDL_RenderPresent(g_cpu->display.sdl2_renderer);
#endif /* RISC_VIDEO_DEVICE_SDL2 */

    return 0;
}

static uint8_t VIDEORAM_finalize() {
    RISC_FREE(g_cpu->display.screen_pixels);
    SDL_DestroyTexture(g_cpu->display.sdl2_screen_texture);
    SDL_DestroyRenderer(g_cpu->display.sdl2_renderer);
    SDL_DestroyWindow(g_cpu->display.sdl2_window);
    SDL_Quit();
    return 0;
}

static uint8_t VIDEORAM_read_byte(VMWORD pAddr) {
    return 0;
}

static void VIDEORAM_write_byte(uint8_t pData, VMWORD pAddr) {

}

static VMWORD VIDEORAM_read_word(VMWORD pAddr) {
    return 0;
}

static void VIDEORAM_write_word(VMWORD pData, VMWORD pAddr) {
}

risc_device_t device_VIDEORAM = {
    "VIDEORAM",
    VIDEORAM_initialize,
    VIDEORAM_finalize,
    VIDEORAM_read_byte,
    VIDEORAM_write_byte,
    VIDEORAM_read_word,
    VIDEORAM_write_word
};

static uint8_t VIDEOCTL_initialize() {
    return 0;
}

static uint8_t VIDEOCTL_finalize() {
    return 0;
}

static uint8_t VIDEOCTL_read_byte(VMWORD pAddr) {
    return 0;
}

static void VIDEOCTL_write_byte(uint8_t pData, VMWORD pAddr) {

}

static VMWORD VIDEOCTL_read_word(VMWORD pAddr) {
    return 0;
}

static void VIDEOCTL_write_word(VMWORD pData, VMWORD pAddr) {
    switch(pData) {
    case 1:
        refresh();
        break;

    default:
        risc_log_warn(g_cpu, "invalid VIDEOCTL function id: %d", THIS_FILE, __LINE__, pData);
        break;
    }
}

risc_device_t device_VIDEOCTL = {
    "VIDEOCTL",
    VIDEOCTL_initialize,
    VIDEOCTL_finalize,
    VIDEOCTL_read_byte,
    VIDEOCTL_write_byte,
    VIDEOCTL_read_word,
    VIDEOCTL_write_word
};
