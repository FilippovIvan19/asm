.model tiny
.code      

X = 17d

org 100h   

start:		
		mov ax, 0b800h
		mov es, ax
		mov bx, 3*80*2
		mov cx, 128*256
		
		
	for_1:
		mov ax, cx
		and ax, X
		cmp ax, 0
		

		mov byte ptr es:[bx+1], 4eh
		mov byte ptr es:[bx], '0' 	
		jne print_1
		jmp end_print
	print_1:		
		mov byte ptr es:[bx], '1'
		

	end_print:
		add bx, 2
		shr cx, 1
		cmp cx, 1
		jnl for_1



		mov ax, 4c00h      
		int 21h            



end start
