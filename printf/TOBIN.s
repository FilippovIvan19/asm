section .text

;-----------------------------------------------------------------------------
;	write number from rdx in bin format
;	entry:	rdx - number
;	        rdi - str-to   adress
;	exit	string of 0/1
;	destr	rax rbx rcx rdx rdi(increases by number of simbols) rsi
;-----------------------------------------------------------------------------
global tobin


	tobin:
            mov rcx, 0x8000000000000000

            mov rbx, bin_buf

        for_1:
            mov rax, rcx
            and rax, rdx

            cmp rax, 0
            jne print_1
            mov byte [rbx], '0'
            jmp end_print

        print_1:
            mov byte [rbx], '1'

        end_print:
            inc rbx
            shr rcx, 1
            cmp rcx, 1
            jnl for_1


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


section     .data
bin_buf:        times 64 db ' '

