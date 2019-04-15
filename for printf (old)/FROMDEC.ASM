.model tiny
.code      

org 100h

start:				
		mov bx, offset val      
		call fromdec
		jmp end_prog



;-----------------------------------------------------------------------------
;	makes a numder from string of dec digits
;	entry:	BX - str adress
;	exit:	DX - number
;	destr:	AX BX CX
;----------------------------------------------------------------------------- 
	fromdec proc
		mov cx, 10000d
		mov dx, 0

	for_1:
		mov byte ptr al, [bx]	
		mov ah, 0
		
		
		sub al, '0'
		push dx
		mul cx
		pop dx
		add dx, ax
		
		inc bx
		mov ax, cx
		mov cx, 10d
		push dx
		mov dx, 0
		div cx
		mov cx, ax
		pop dx
		cmp cx, 0
	
		jne for_1
		ret
		endp

                 
		                        
	end_prog:
		mov ax, 4c00h           
		int 21h                 

.data
val 		db '04572$'

end start