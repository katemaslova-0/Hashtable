default rel
global SumAsciiHashFunc

section .text

SumAsciiHashFunc:
        movsx   edx, byte [rdi]
        test    dl, dl
        je      .L4
        xor     eax, eax
.L3:
        add     rdi, 1
        add     eax, edx
        movsx   edx, byte [rdi]
        test    dl, dl
        jne     .L3
        xor     edx, edx
        div     esi
        mov     eax, edx
        ret
.L4:
        xor     edx, edx
        mov     eax, edx
        ret