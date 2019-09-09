section .text


global _start

_start:
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

            std
            mov rdi, read_val + 19
            mov al, 0xa
            mov rcx, 20
            repe scasb
            mov rcx, rdi
            sub rcx, read_val
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

            pop rdx
            pop rcx
            pop rbx
            pop rax
        ret



section .data
val 		times 20 db "0"
            db 0x0
read_val	times 20 db 0xa
            db 0x0

