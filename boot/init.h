#include <stdint.h>

#define GTD_BASE 0x00000800

#define IDT_BASE 0x00000000

typedef struct {
    uint16_t limit0_15;
    uint16_t base0_15;
    uint8_t base16_23;
    uint8_t access;
    uint8_t limit16_19 : 4;
    uint8_t flags : 4;
    uint8_t base24_31;
} __attribute__((packed)) gdt_desc;

typedef struct {
    uint16_t offset0_15;
    uint16_t selector;
    uint8_t zero;
    uint8_t type;
    uint16_t offset16_31
} __attribute__((packed)) idt_desc;

void init_gdt();
void init_gdt_desc(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags, gdt_desc * desc);

void init_idt();
void init_idt_desc(uint16_t selector, uint32_t offset, uint16_t type, idt_desc * desc);

extern void keyboard_handler(void);
