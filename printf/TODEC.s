section .text

;-----------------------------------------------------------------------------
;	write number from rdx in dec format
;	entry:	rdx - number
;	        rdi - str-to   adress
;	exit	string of dec digits
;	destr	rax rbx rcx rdx rdi(increases by number of simbols) rsi
;-----------------------------------------------------------------------------
global todec


	todec:

            mov rbx, dec_buf

            mov rcx, 10000000000000000000

            mov r8, rdx

        for_1:
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
            jnl for_1


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


section     .data
dec_buf:        times 20 db ' '
