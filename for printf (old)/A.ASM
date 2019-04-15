.model tiny                             
.code                                   

X1 = 1
X2 = 5
Y1 = 1
Y2 = 5


org 100h                               

start:
		push Y2
		push X2
		push Y1
		push X1
		call ramka		;ramka(x1, y1, x2, y2)
		add sp, 4*2
		jmp end_prog
		

	ramka proc
		push bp
		mov bp, sp
		
		
		mov ax, 0b800h
		mov es, ax
		mov dh, 160	;    na eto budem delit'

		mov ax, [bp+6]	;Y1
		mov cx, [bp+4]	;X1

		mov bx, ax
		sub bx, 1
		mov ax, bx
		mul dh
		mov bx, ax
		add bx, cx
		add bx, cx
		sub bx, 2	
		jmp for_one

	for_two:
		mov ax, [bp+8]	;X2
		mov cx, [bp+4]  ;X1
		shl ax, 1
		shl cx, 1
		add bx, 80*2 - 2
		sub bx, ax
		add bx, cx
		
		


	for_one:	
		mov word ptr es:[bx], 4e21h                 
		
		add bx, 2
		
		mov ax, bx
		div dh				;ah - mod
		
		mov ch, [bp+8]	;X2
		shl ch, 1
		cmp ah, ch
	jl for_one                       
		
		mov cx, [bp+10]	;Y2
		sub cl, 1
		cmp al, cl		;??????????ch
	jl for_two
		

		mov al, [bp+6]	;Y1
		mov cx, [bp+4]	;X1
		mov ah, 160d
		mul ah
		shl cx, 1
		mov bx, ax
		add bx, cx
		
		jmp for_three



	for_four:
		mov ax, [bp+4]	;X1
		mov cx, [bp+8]	;X2
		
		add ax, 1
		shl ax, 1
		sub cx, 1
		shl cx, 1
		
		add bx, 80*2 - 2
		add bx, ax
		sub bx, cx

	for_three:
		mov word ptr es:[bx], 4e20h

		add bx, 2
		mov ax, bx
		div dh

		mov cx, [bp+8]			;???????????ch
		sub cl, 1
		shl cl, 1
		cmp ah, cl
	jl for_three
		
		mov cx, [bp+10]
		sub cl, 2
		cmp al, cl			;????????????ch
	jl for_four

		mov sp, bp
		pop bp
		ret
	endp		
		
	end_prog:

		mov ax, 4c00h           
		int 21h                 

.data


end start