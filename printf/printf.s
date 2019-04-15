section .text


global _start
global str_len
global str_start
extern copy_until
extern tobin
extern todec
extern tohex


_start:
            push '!'
            push 100
            push 3802
            push substr_2

            push 0xf0f
            push 31415
            push 0xded32
            push substr_1

            push '#'
            push 146307
            push 0x1f
            push str_start
            call printf
            add rsp, 8*12

            jmp end_prog




    printf:
            push rbp
            mov rbp, rsp
            add rbp, 8*2

            mov rsi, [rbp]
            add rbp, 8
            mov rdi, print_buf


        print_cycle:


            mov al, '%'
            call copy_until

            mov al, 0
            cmp [rsi], al
            je end_func


            inc rsi
            push rsi

            mov al, 'b'
            cmp [rsi], al
            je print_b

            mov al, 'd'
            cmp [rsi], al
            je print_d

            mov al, 'x'
            cmp [rsi], al
            je print_x

            mov al, 'c'
            cmp [rsi], al
            je print_c

            mov al, '%'
            cmp [rsi], al
            je print_perc

            mov al, 's'
            cmp [rsi], al
            je print_s

        switch_end:
            pop rsi
            inc rsi
            add rbp, 8
            jmp print_cycle


        print_b:
            mov rdx, [rbp]
            call tobin
            jmp switch_end

        print_d:
            mov rdx, [rbp]
            call todec
            jmp switch_end

        print_x:
            mov rdx, [rbp]
            call tohex
            jmp switch_end

        print_c:
            mov byte al, [rbp]
            mov byte [rdi], al
            inc rdi
            jmp switch_end

        print_perc:
            mov byte [rdi], '%'
            inc rdi
            sub rbp, 8
            jmp switch_end

        print_s:
            mov al, 0
            mov rsi, [rbp]
            call copy_until
            jmp switch_end

    end_func:
        pop rbp
        ret



    print_now:
            push rdi
            push rsi


            mov rax, 0x01      ; write64 (rdi, rsi, rdx) ... r10, r8, r9
            mov rdx, rdi
            sub rdx, print_buf ; strlen (Msg)
            mov rdi, 1         ; stdout
            mov rsi, print_buf
            syscall

            mov rax, 0x01      ; write64 (rdi, rsi, rdx) ... r10, r8, r9
            mov rdi, 1         ; stdout
            mov rsi, slash_n
            mov rdx, 1         ; strlen
            syscall


            pop rsi
            pop rdi
    ret

	end_prog:

            call print_now
            mov rax, 0x3C      ; exit64 (rdi)
            xor rdi, rdi
            syscall


section         .data
str_start: 		db "abacaba %b qwer %d qwer %c qwer %s qwer%% %x fg %d %x, and I %s %x %d %% %c", 0
print_buf: 		times 1024 db "@", 0
slash_n:        db 0x0a
char:           db " "
substr_1:       db "*pervaya podstroka*", 0
substr_2:       db "love", 0
