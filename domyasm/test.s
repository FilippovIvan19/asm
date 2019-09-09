section .text



global _start

_start:

            push 3

            pop r14
            pop r15
            push rax
            push rdx
            mov rax, r15
            mov rdx, 0
            div r14
            mov r15, rax
            pop rdx
            pop rax
            push r15




            pop r14
            pop r15
            push rax
            push rdx
            mov rax, r15
            mov rdx, 0
            mul r14
            mov r15, rax
            pop rdx
            pop rax
            push r15

            ja char
            jae char
            jb char
            jbe char
            je char
            call char
            ret

            pop r14
            pop r15
            cmp r15, r14

            jb end_prog



	end_prog:

            mov rax, 0x3C      ; exit64 (rdi)
            xor rdi, rdi
            syscall
            ;ret


section         .data
print_buf: 		times 10 db "@", 0
char:           db 0xea
                db 0x56
                db 0x57
                db 0x69
                db 0xff
                db 0xca
                db 0xcb
                db 0xcc
                db 0xcd
                jmp end_prog
