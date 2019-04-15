section .text

;-----------------------------------------------------------------------------
;	write number from rdx in hex format
;	entry:	rdx - number
;	        rdi - str-to   adress
;	exit	string of 0/1
;	destr	rax rbx rcx rdx rdi(increases by number of simbols) rsi
;-----------------------------------------------------------------------------
global tohex


	tohex:
            mov rbx, hex_buf
            mov cl, 60
        for_1:
            mov rax, 0xf
            shl rax, cl
            and rax, rdx
            shr rax, cl

            add al, '0'
            mov byte [rbx], al

            mov ah, 0
            add ah, '9'
            cmp ah, al
            jl print_1
            jmp end_print
        print_1:
            mov rax, 'A' - '9' - 1
            add [rbx], rax


        end_print:
            inc rbx
            sub cl, 4
            mov ch, 0
            cmp ch, cl
            jle for_1


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



section     .data
hex_buf:        times 16 db '#'
