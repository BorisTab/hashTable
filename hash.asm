.global     hash5asm
.intel_syntax noprefix

hash5asm:
        xor     rdx, rdx
        xor     rax, rax
        mov     ebx, 1
        shl     ebx, 0x1f

.loop:
        xor     al, [rdi]
        inc     rdi

        mov     ecx, eax
        and     ecx, ebx
        shr     ecx, 0x1f

        shl     eax, 1
        or      eax, ecx

        cmp     dl, [rdi]
        jne     .loop

        ret

.att_syntax prefix