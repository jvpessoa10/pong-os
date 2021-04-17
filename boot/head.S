.section .text
    .global set_gdt, set_idt
    set_gdt:
        lgdt gdtr
        jmp $0x08, $flush_segments
    flush_segments:
        mov $0x10, %ax
        mov %ax, %ds
        mov %ax, %es
        mov %ax, %fs
        mov %ax, %gs
        mov %ax, %ss
        ret
    
    set_idt: 
        lidt idtr 
        sti
        ret
    