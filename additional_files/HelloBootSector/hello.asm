;print a message to the screen using a bios routine

mov ah, 0xe

mov al, 'H'
int 0x10       ;invoke the interrupt, multiplexed with the high byte of ax

mov al, 'e'
int 0x10

mov al, 'l'
int 0x10

mov al, 'l'
int 0x10

mov al, 'o'
int 0x10

jmp $      ;jump to the current address (creating an infinite loop)

;padding and magic bios number

times 510 - ($ - $$) db 0

dw 0xaa55
