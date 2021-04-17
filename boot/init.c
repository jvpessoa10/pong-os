#include "init.h"
#include "io.h"

extern set_gdt();
extern set_idt();

struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdtr;

struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idtr;

gdt_desc * gdt_start = (gdt_desc *) GTD_BASE;

idt_desc * idt_start = (idt_desc *) IDT_BASE;

void init_gdt() {

    gdtr.base = gdt_start;
    gdtr.limit = (sizeof(gdt_desc) * 3 ) - 1;

    init_gdt_desc(0x0, 0x0, 0x0, 0x0, gdt_start); // Empty for compatibility 
    init_gdt_desc(0x0, 0xFFFFFFFF , 0x9A, 0xCF, gdt_start + 1); // Code
    init_gdt_desc(0x0, 0xFFFFFFFF, 0x92, 0xCF, gdt_start + 2); // Data

    set_gdt();
}   

void init_gdt_desc(uint32_t base, uint32_t limit, uint8_t access, uint8_t flags, gdt_desc * desc) {
    desc->limit0_15 = limit & 0xFFFF;
    desc->limit16_19 = (limit >> 16) & 0xF;
    
    desc->base0_15 = base & 0xFFFF;
    desc->base16_23 = (base >> 16) & 0xFF;
    desc->base24_31 = (base >> 24) & 0xFF;
    
    desc->access = access;
    desc->flags = flags & 0xF;
}

void init_idt() {
    idtr.base = idt_start;
    idtr.limit = (sizeof(idt_desc) * 33);

    init_idt_desc(0x08, &keyboard_handler, 0x8E, idt_start + 33);
    
    init_pic();
    set_idt();
    write_port(0x21 , 0xFD);
}

void init_pic() {
    /*     Ports
	*	 PIC1	PIC2
	*Command 0x20	0xA0
	*Data	 0x21	0xA1
	*/

	/* ICW1 - begin initialization */
	write_port(0x20 , 0x11);
	write_port(0xA0 , 0x11);

	/* ICW2 - remap offset address of IDT */
	/*
	* In x86 protected mode, we have to remap the PICs beyond 0x20 because
	* Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	*/
	write_port(0x21 , 0x20);
	write_port(0xA1 , 0x28);

	/* ICW3 - setup cascading */
	write_port(0x21 , 0x00);  
	write_port(0xA1 , 0x00);  

	/* ICW4 - environment info */
	write_port(0x21 , 0x01);
	write_port(0xA1 , 0x01);
	/* Initialization finished */

	/* mask interrupts */
	write_port(0x21 , 0xff);
	write_port(0xA1 , 0xff);
}

void init_idt_desc(uint16_t selector, uint32_t offset, uint16_t type, idt_desc * desc) {
    desc->offset0_15 = offset & 0xFFFF;
    desc->offset16_31 = (offset >> 16) & 0xFFFF;
    desc->selector = selector;
    desc->zero = 0;
    desc->type = type;
}
