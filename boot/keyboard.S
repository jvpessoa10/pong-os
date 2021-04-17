.section .text
    .global keyboard_handler
    keyboard_handler:
        call keyboard_handler_main
        iret  # Acknologes interruption
    