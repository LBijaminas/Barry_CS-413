org 0x7c00

mov dx, 0x1A4B
call print_addr_16 
call print_string

push 0xAAAA

mov bx, msg 
call print_string

mov dx, sp
call print_addr_16 
call print_string

msg:
	db "After pushing", 0x0A, 0x0D, "$"

jmp $

%include "print_addr_16.asm"
%include "print_string.asm"

times 510 - ($ - $$) db 0
dw 0xaa55
