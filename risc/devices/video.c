/*
 * video.c
 * Video output device.
 *
 * 24 jul 2022 -- 11:44 -03
 * Notes:
 */

#include "risc.h"

/* Convert from pixel format to RGB bytes. */
#define COLOR_R(v) ((v >> 5) * 255 / 7)
#define COLOR_G(v) (((v >> 2) & 7) * 255 / 7)
#define COLOR_B(v) ((v & 3) * 255 / 3)

#define COLOR_RGB(r, g, b) \
    ((r & 0xff) << 24) | \
    ((g & 0xff) << 16) | \
    ((b & 0xff) << 8) | \
    0xff

/* Convert from RGB bytes to pixel format. */
#define COLOR_PIXEL(r, g, b) \
    (((r * 7 / 255) & 7) << 5) | \
    (((g * 7 / 255) & 7) << 2) | \
    ((b * 3 / 255) & 3)

static const char* THIS_FILE = "video.c";

extern risc_vm_t* g_cpu;

typedef struct _risc_videochip_t
{
    uint32_t fg_color;
    uint32_t bg_color;
} risc_videochip_t;

static risc_videochip_t g_videochip = {
    .fg_color = COLOR_RGB(255, 255, 255),
    .bg_color = COLOR_RGB(100, 100, 100)
};

static void refresh() {
    SDL_UpdateTexture(g_cpu->display.sdl2_screen_texture,
        NULL, g_cpu->display.screen_pixels,
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

static void clear() {
    for(uint32_t i = 0; i < RISC_VIDEO_DEVICE_COLUMNS * RISC_VIDEO_TILE_PIXELS *
        RISC_VIDEO_DEVICE_ROWS * RISC_VIDEO_TILE_PIXELS; i++) {
            g_cpu->display.screen_pixels[i] = g_videochip.bg_color;
    }
    refresh();
}

point2_t point2_normalize(point2_t pP) {
    float m = sqrt(pP.x * pP.x + pP.y * pP.y);
    point2_t ret = {
        .x = pP.x / m,
        .y = pP.y / m
    };
    
    return ret;
}

#define DRAW_PIXEL(x, y, pix) \
   if(x >= 0 && x < RISC_VIDEO_DEVICE_COLUMNS * RISC_VIDEO_TILE_PIXELS && \
       y >= 0 && y < RISC_VIDEO_DEVICE_ROWS * RISC_VIDEO_TILE_PIXELS) { \
        g_cpu->display.screen_pixels[y * RISC_VIDEO_DEVICE_COLUMNS * RISC_VIDEO_TILE_PIXELS + \
            x * RISC_VIDEO_TILE_PIXELS] = pix; \
    }

#if 0
static void draw_line() {
    point2_t pt0 = {
        g_cpu->registers[reg_R5],
        g_cpu->registers[reg_R6]
    };
    point2_t pt1 = {
        g_cpu->registers[reg_R7],
        g_cpu->registers[reg_R8]
    };

    int dx = abs(pt1.x - pt0.x), sx = pt0.x < pt1.x ? 1 : -1;
    int dy = -abs(pt1.y - pt0.y), sy = pt0.y < pt1.y ? 1 : -1;
    int err = dx + dy, e2; /* error value e_xy */

    for (;;)
    {
        DRAW_PIXEL(pt0.x, pt0.y, g_videochip.fg_color)
        if (pt0.x == pt1.x && pt0.y == pt1.y)
            break;
        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            pt0.x += sx;
        } /* e_xy+e_x > 0 */
        if (e2 <= dx)
        {
            err += dx;
            pt0.y += sy;
        } /* e_xy+e_y < 0 */
    }
}
#endif

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
    switch(pAddr) {
    case RISC_VIDEO_BGCOL_REG:
        g_videochip.bg_color = COLOR_RGB(
            COLOR_R(pData), COLOR_G(pData), COLOR_B(pData));
        break;
    
    case RISC_VIDEO_FGCOL_REG:
        g_videochip.fg_color = COLOR_RGB(
            COLOR_R(pData), COLOR_G(pData), COLOR_B(pData));
        break;

    default:
        break;
    }
}

static VMWORD VIDEOCTL_read_word(VMWORD pAddr) {
    return 0;
}

static void VIDEOCTL_write_word(VMWORD pData, VMWORD pAddr) {
    switch(pAddr) {
    case RISC_VIDEO_CMD_REG:
        switch(pData) {
        case 1:
            refresh();
            break;

        case 2:
            clear();
            break;

        default:
            risc_log_warn(g_cpu, "invalid VIDEOCTL function id: %d", THIS_FILE, __LINE__, pData);
            break;
        }
        break;

    default:
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
