;simple boot sector that loops forever

;define a label we can repeatedly jump back to
loop:
	jmp loop

times 510-($-$$) db 0

dw 0xaa55

