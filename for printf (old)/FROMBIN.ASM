.model tiny
.code      

org 100h

start:				
		mov bx, offset val      
		call frombin
		jmp end_prog



;-----------------------------------------------------------------------------
;	makes a numder from string of 0/1
;	entry:	BX - str adress
;	exit:	DX - number
;	destr:	AX BX CX
;----------------------------------------------------------------------------- 
	frombin proc		
		mov cx, 16d
		mov dx, 0

	for_1:
		mov byte ptr al, [bx]	
		sub al, '0'
		mov ah, 0
		shl dx, 1
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
val 		db '1000001000011111$'

end start