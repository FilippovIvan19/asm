.model tiny
.code      

X = 3802d

org 100h   

start:		
		mov ax, 0b800h
		mov es, ax
		mov bx, 3*80*2
		mov cl, 12
		mov dx, X		
		call tohex
		jmp end_prog

;-----------------------------------------------------------------------------
;	write number from dx in hex format
;	entry:	DX - number
;	exit	string of hex digits in video memory
;	destr	AX BX CX DX
;-----------------------------------------------------------------------------
	tohex proc
	for_1:
		mov ax, 000fh
		shl ax, cl
		and ax, dx
		shr ax, cl
		
		add al, '0'
		mov byte ptr es:[bx], al
		mov byte ptr es:[bx+1], 4eh

		mov ah, 0
		add ah, '9'
		cmp ah, al 	
		jl print_1
		jmp end_print
	print_1:		
		mov ax, 'A' - '9' - 1
		add es:[bx], ax
		

	end_print:
		add bx, 2
		sub cl, 4
		mov ch, 0 
		cmp ch, cl
		jle for_1
		ret
		endp


	end_prog:
		mov ax, 4c00h      
		int 21h            



end start
