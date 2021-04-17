/* Declare constants for the multiboot header. */
.set ALIGN,    0x00000000             /* align loaded modules on page boundaries */
.set MEMINFO,  0x00000001             /* provide memory map */
.set VIDEO_MODE,  0x00000004
.set FLAGS,    ALIGN | MEMINFO | VIDEO_MODE  /* this is the Multiboot 'flag' field */
.set MAGIC,    0x1BADB002       /* 'magic number' lets bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */

.section .multiboot
	.align 4
	.long MAGIC
	.long FLAGS
	.long CHECKSUM
	.long 0, 0, 0, 0, 0
	.long 0 #set graphics mode
	.long 1024, 768, 32 #width, height, depth
 
.section .bss
	.align 16
	stack_bottom:
	.skip 16384 # 16 KiB
	stack_top:

.section .text
	.global _start, keyboard_interrupt
	.type _start, @function
	_start:
		
		mov $stack_top, %esp

		pushl %ebx /* pushes the Multiboot header addr to stack */
		pushl %eax /*pushes the magic number to the stack*/

		call start_kernel
	
		cli
	1:	hlt
		jmp 1b		
 
.size _start, . - _start
