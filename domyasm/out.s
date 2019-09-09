section .text

;-----------------------------------------------------------------------------
;	write number from rdx in dec format
;	entry:	rdx - number
;	        rdi - str-to   adress
;	exit	string of dec digits
;	destr	rax rbx rcx rdx rdi(increases by number of simbols) rsi
;-----------------------------------------------------------------------------


global _start

_start:

            push rax
            push rbx
            push rcx
            push rdx


            mov rdx, r15
            mov rbx, dec_out_buf

            mov rcx, 10000000000000000000

            mov r8, rdx

        for_out_1:
            mov rax, r8
            mov rdx, 0
            div rcx
            mov r8, rdx
            add rax, '0'

            mov byte [rbx], al

            inc rbx
            mov rax, rcx
            mov rdx, 0
            mov rcx, 10
            div rcx
            mov rcx, rax
            cmp rcx, 1
            jnl for_out_1




            mov al, '0'
            mov rdi, dec_out_buf
            cld
            mov rcx, 20
            repe scasb

            sub rcx, 2
            dec rdi

            mov rax, 0x01      ; write64 (rdi, rsi, rdx) ... r10, r8, r9
            mov rdx, rcx        ; strlen (Msg)
            mov rsi, rdi
            mov rdi, 1          ; stdout
            syscall


            pop rdx
            pop rcx
            pop rbx
            pop rax
        ret


section     .data
dec_out_buf:        times 20 db ' '
                db 0xa


