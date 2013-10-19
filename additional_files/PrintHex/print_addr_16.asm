;
; dx: value to convert
;

TABLE: 
	db "0123456789ABCDEF"

ADDR_STRING:
	db "ADDR: 0x", 0, 0, 0, 0, 0x0A, 0x0D, '$'

ADDR_OFFSET equ 8        ;our address's offset inside the string

print_addr_16:
	lea bx, [TABLE]
	
	;get the most significant byte
	mov ax, dx      ;copy value into ax
	shr ax, 8
	mov ah, al
	shr ah, 4       ;isolate the high nibble in ah
	and al, 0x0F    ;isolate the low nibble in al
	xlat            ;look up al in our table
	xchg al, ah     ;flip them so we can look up the high nibble
	xlat            ;look up the high nibble now in al

	;load our string so we can stick the high byte in
	lea bx, [ADDR_STRING]
	mov [bx + ADDR_OFFSET], ax

	;load our table again for the lower byte
	lea bx, [TABLE]

	;get the least significant byte
	mov ax, dx      ;copy the value again into ax
	mov ah, al
	shr ah, 4
	and al, 0x0F
	xlat
	xchg al, ah
	xlat
	
	;load our string so we can move all the values into it
	lea bx, [ADDR_STRING + 2]

	;move the lower byte to our string
	mov [bx + ADDR_OFFSET], ax

	sub bx, 2
	
	ret	
