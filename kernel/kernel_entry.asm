[bits 32]
[extern main] ; imports C main function

cli
call main ; call main
jmp $
