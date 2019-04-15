section .text
;nasm -f elf64 1-nasm.s  ;  ld -s -o 1-nasm 1-nasm.o

global asm_func
extern CSub
extern MYprintf

ASub:
            mov rax, rdi
            sub rax, rsi
    ret

asm_func:
            push rdi
            push rsi

            call ASub

            push rax
            push rsi
            push rdi
            mov rdi, print_str
            push rdi
            call MYprintf
            add rsp, 8*4


            pop rsi
            pop rdi
            call CSub
    ret



section .data
print_str: db "asm:  ASub: %d - %d = %d", 0
