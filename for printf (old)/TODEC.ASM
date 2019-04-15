.model tiny
.code      

X = 65535d

org 100h   

start:		
		mov ax, 0b800h
		mov es, ax
		push X
		call todec
		dec sp
		jmp end_prog		

;-----------------------------------------------------------------------------
;	write number from dx in dec format
;	entry:	DX - number
;	exit	string of dec digits in video memory
;	destr	AX BX CX DX
;-----------------------------------------------------------------------------
	todec proc

		mov bx, 3*80*2
		mov cx, 10000
				
		mov bp, sp
		add bp, 2
	for_1:
		mov ax, [bp]
		mov dx, 0
		div cx
		mov [bp], dx
		add ax, '0'		

		mov byte ptr es:[bx+1], 4eh
		mov byte ptr es:[bx], al 	
		
		add bx, 2
		mov ax, cx
		mov dx, 0
		mov cx, 10d
		div cx
		mov cx, ax
		cmp cx, 1
		jnl for_1
	
	ret
	endp

	end_prog:
		mov ax, 4c00h      
		int 21h            



end start
