; make sure we enter at the main

[bits 32]
[extern kernel_main]

call kernel_main
jmp $
