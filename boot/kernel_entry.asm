global _start;
[bits 32]

_start:
    [extern start] ; Define calling point. Must have same name as kernel.c 'main' function
    call start ; Calls the C function. The linker will know where it is placed in memory
    jmp $
