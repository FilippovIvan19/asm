section .text


global _start

_start:

            nop
            nop
            nop
            nop

            call in
            ret

            call out
            ret

            call end_prog
            ret

;-----------------------------------------------------------------------------
;	read number from input in dec format
;	exit	r15
;	destr	rdi rsi r14
;-----------------------------------------------------------------------------
    in:
            push rax
            push rbx
            push rcx
            push rdx


            mov rax, 0
            mov rdi, 1
            mov rsi, read_val
            mov rdx, 20
            syscall;read


            mov rdi, read_val + 19
            mov al, 0xa
            mov rcx, 20
            std
            repe scasb
            mov rcx, rdi
            mov r14, read_val
            sub rcx, r14
            add rcx, 2

            mov rsi, rdi
            inc rsi
            mov rdi, val + 19
            rep movsb
            cld


            mov rbx, val
            mov rcx, 10000000000000000000
            mov rdx, 0


        for_in_1:
            mov rax, 0
            mov byte al, [rbx]
            mov ah, 0
            sub al, '0'

            push rdx
            mul rcx
            pop rdx
            add rdx, rax

            inc rbx
            mov rax, rcx
            mov rcx, 10
            push rdx
            mov rdx, 0
            div rcx
            mov rcx, rax
            pop rdx

            cmp rcx, 0
            jne for_in_1

            mov r15, rdx

            mov al, "0"
            mov rdi, val
            mov rcx, 20
            rep stosb

            pop rdx
            pop rcx
            pop rbx
            pop rax
    ret


;-----------------------------------------------------------------------------
;	write number from r15 in dec format
;	destr       rdi rsi r14
;-----------------------------------------------------------------------------
    out:
            push rax
            push rbx
            push rcx
            push rdx


            mov rdx, r15
            mov rbx, dec_out_buf

            mov rcx, 10000000000000000000

            mov r14, rdx

        for_out_1:
            mov rax, r14
            mov rdx, 0
            div rcx
            mov r14, rdx
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

            add rcx, 2
            dec rdi

            mov rax, 0x01       ; write
            mov rdx, rcx        ; strlen (Msg)
            mov rsi, rdi
            mov rdi, 1          ; stdout
            syscall


            pop rdx
            pop rcx
            pop rbx
            pop rax
    ret



	end_prog:

            mov rax, 0x3C
            xor rdi, rdi
            syscall


section .data
                nop
                nop
                nop
                nop
                    dq $
val 		        times 20 db "0"
                    db 0x0
read_val	        times 20 db 0xa
                    db 0x0
dec_out_buf:        times 20 db ' '
                    db 0xa, 0x0

    nop
    nop
    nop
    nop


