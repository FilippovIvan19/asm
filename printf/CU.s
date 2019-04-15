section .text

extern str_len
extern str_start

;-----------------------------------------------------------------------------
;	copy simbols until meets 0 or al
;	entry:  rsi - str-from adress
;	        rdi - str-to   adress
;           al  - stop simbol
;	destr:	rcx rsi rdi(increases by number of simbols) rax rdx
;-----------------------------------------------------------------------------
global copy_until


    copy_until:
            cld
            push rdi
            push rax

            mov rdi, rsi
            mov rcx, -1
            mov al, 0
            repne scasb
            inc rcx
            neg rcx

            mov rdx, rcx; number of simbols before the end of str

            mov rdi, rsi
            pop rax
            repne scasb

            neg rcx
            add rcx, rdx
            dec rcx

            pop rdi

            rep movsb
        ret
