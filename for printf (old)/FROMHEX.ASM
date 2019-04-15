.model tiny
.code      

org 100h

start:				
		mov bx, offset val      
		call fromhex
		jmp end_prog



;-----------------------------------------------------------------------------
;	makes a numder from string of hex digits
;	entry:	BX - str adress
;	exit:	DX - number
;	destr:	AX BX CX
;----------------------------------------------------------------------------- 
	fromhex proc		
		mov cx, 4d
		mov dx, 0

	for_1:
		mov byte ptr al, [bx]	
		mov ah, 0
		shl dx, 4
		cmp al, 'A'
		jb digit
		sub al, 'A' - '0' - 10d
	digit:
		sub al, '0'


		add dx, ax
		
		inc bx
		dec cx
		cmp cx, 0
	
		jne for_1
		ret
		endp

                 
		                        
	end_prog:
		mov ax, 4c00h           
		int 21h                 

.data
val 		db 'BE05$'

end start