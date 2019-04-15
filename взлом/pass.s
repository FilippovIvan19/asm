global _start

section .text

_start:

            mov rax, 1
            mov rdi, 1
            mov rsi, msg
            mov rdx, msg_len
            syscall

            mov rax, 0
            mov rdi, 1
            mov rsi, read_pwrd
            mov rdx, 22
            syscall

            mov rcx, 10
            mov rdx, 1
            mov rdi, read_pwrd
        loop_1:
            mov rax, [rdi]
            inc rax
            mov [rdi], rax
            inc rdi
            loop loop_1

            cld
            mov rcx, 11
            mov rdi, read_pwrd
            mov rsi, password
            repe cmpsb

            cmp rcx, 0
            je access_granted

            mov rax, 1
            mov rdi, 1
            mov rsi, str_denied
            mov rdx, 14
            syscall

            mov rax, 0x3C
            mov rdi, 0
            syscall



    access_granted:

            mov rax, 1
            mov rdi, 1
            mov rsi, str_granted
            mov rdx, 15
            syscall

            mov rax, 0x3C
            mov rdi, 0
            syscall
    ret



section .data
str_denied:     db "access denied", 0xa
str_granted:    db "access granted", 0xa
read_pwrd:      db "0123456789", 0xa
password:       db "555555555", 0xff, 0xa
msg:            db "enter 10 digit password", 0xa
msg_len:        equ $-msg
