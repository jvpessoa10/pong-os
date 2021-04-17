#include "multiboot.h"
#include "boot/init.h"

#define PLAYER_HEIGTH 128
#define PLAYER_WIDTH 32

#define BALL_SIZE 16

static int player_1_offset = 0;
static int player_2_offset = 0;

multiboot_info_t * mb_info;

void drawPlayers();
void drawBall();
void drawPixel(int x, int y, multiboot_uint32_t color);

void start_kernel(unsigned long magic, unsigned long multiboot_addr) {
    mb_info = (multiboot_info_t *) multiboot_addr;
    
    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        //TODO implement printf
        return;
    }

    init_gdt();
    init_idt();
        
    drawPlayers();
}

void drawPlayers() {
    int height_offset = (mb_info->framebuffer_height - PLAYER_HEIGTH) / 2;
    
    for (int x = 0; x < PLAYER_WIDTH; x++) {
        for (int y = 0; y < PLAYER_HEIGTH; y++) {
            drawPixel(x, y + height_offset + player_1_offset, 0xFFFFFFFF); //Player 1
            drawPixel(
                mb_info->framebuffer_width - x,
                y + height_offset + player_2_offset
                , 0xFFFFFFFF); //Player 2
        }
    }
}

void drawBall() {
    int height_offset = (mb_info->framebuffer_height - BALL_SIZE) / 2;
    int width_offset = (mb_info->framebuffer_width - BALL_SIZE) / 2;

    for (int x = 0; x < BALL_SIZE; x++) {
        for (int y = 0; y < BALL_SIZE; y++) {
            drawPixel(x + width_offset, y + height_offset, 0xFFFFFFFF);
        }
    }
    
}

void drawPixel(int x, int y, multiboot_uint32_t color) {
    void * fb_addr = (void *) mb_info->framebuffer_addr;

    switch (mb_info->framebuffer_bpp) {
        case 8: {
            multiboot_uint8_t * pixel =
                fb_addr + mb_info->framebuffer_pitch * y + x;
            *pixel = color;
            break;
            }
        case 15:
        case 16: {
            multiboot_uint16_t * pixel =
                fb_addr + mb_info->framebuffer_pitch * y + 2 * x;
            *pixel = color;
            break;
        }
        case 32: {
            multiboot_uint32_t * pixel =
                fb_addr + mb_info->framebuffer_pitch * y + 4 * x;
            *pixel = color;
            break;
        }
    }
}

void keyboard_handler_main() {
    drawBall();
}

