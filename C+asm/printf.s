section .text


global MYprintf



    MYprintf:
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
        call print_now
        ret


    print_now:
            push rdi
            push rsi


            mov rax, 0x01      ; write64 (rdi, rsi, rdx) ... r10, r8, r9
            mov rdx, rdi
            mov rdi, print_buf ; strlen (Msg)
            sub rdx, rdi
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


;-----------------------------------------------------------------------------
;	write number from rdx in hex format
;	entry:	rdx - number
;	        rdi - str-to   adress
;	exit	string of 0/1
;	destr	rax rbx rcx rdx rdi(increases by number of simbols) rsi
;-----------------------------------------------------------------------------
	tohex:
            mov rbx, hex_buf
            mov cl, 60
        for_hex_1:
            mov rax, 0xf
            shl rax, cl
            and rax, rdx
            shr rax, cl

            add al, '0'
            mov byte [rbx], al

            mov ah, 0
            add ah, '9'
            cmp ah, al
            jl print_A
            jmp end_print_hex
        print_A:
            mov rax, 'A' - '9' - 1
            add [rbx], rax


        end_print_hex:
            inc rbx
            sub cl, 4
            mov ch, 0
            cmp ch, cl
            jle for_hex_1


            mov al, '0'
            push rdi
            mov rdi, hex_buf
            cld
            mov rcx, 16
            repe scasb

            mov rsi, rdi
            dec rsi
            pop rdi
            inc rcx

            rep movsb
    ret


;-----------------------------------------------------------------------------
;	write number from rdx in bin format
;	entry:	rdx - number
;	        rdi - str-to   adress
;	exit	string of 0/1
;	destr	rax rbx rcx rdx rdi(increases by number of simbols) rsi
;-----------------------------------------------------------------------------
	tobin:
            mov rcx, 0x8000000000000000

            mov rbx, bin_buf

        for_bin_1:
            mov rax, rcx
            and rax, rdx

            cmp rax, 0
            jne print_1
            mov byte [rbx], '0'
            jmp end_print_bin

        print_1:
            mov byte [rbx], '1'

        end_print_bin:
            inc rbx
            shr rcx, 1
            cmp rcx, 1
            jnl for_bin_1


            mov al, '0'
            push rdi
            mov rdi, bin_buf
            cld
            mov rcx, 64
            repe scasb

            mov rsi, rdi
            dec rsi
            pop rdi
            inc rcx

            rep movsb
    ret


;-----------------------------------------------------------------------------
;	write number from rdx in dec format
;	entry:	rdx - number
;	        rdi - str-to   adress
;	exit	string of dec digits
;	destr	rax rbx rcx rdx rdi(increases by number of simbols) rsi
;-----------------------------------------------------------------------------
    todec:

            mov rbx, dec_buf

            mov rcx, 10000000000000000000

            mov r8, rdx

        for_dec_1:
            mov rax, r8
            mov rdx, 0
            div rcx
            mov r8, rdx
            add rax, '0'

            mov byte [rbx], al

            inc rbx
            mov rax, rcx
            mov rdx, 0
            mov rcx, 10d
            div rcx
            mov rcx, rax
            cmp rcx, 1
            jnl for_dec_1


            mov al, '0'
            push rdi
            mov rdi, dec_buf
            cld
            mov rcx, 20
            repe scasb

            mov rsi, rdi
            dec rsi
            pop rdi
            inc rcx
            rep movsb

	ret


;-----------------------------------------------------------------------------
;	copy simbols until meets 0 or al
;	entry:  rsi - str-from adress
;	        rdi - str-to   adress
;           al  - stop simbol
;	destr:	rcx rsi rdi(increases by number of simbols) rax rdx
;-----------------------------------------------------------------------------
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



section     .data
hex_buf:        times 16 db '#'
dec_buf:        times 20 db '#'
bin_buf:        times 64 db '#'
print_buf: 		times 1024 db "@", 0
slash_n:        db 0x0a
char:           db " "
