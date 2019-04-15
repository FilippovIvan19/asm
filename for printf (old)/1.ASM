.model tiny                             
.code                                   

X1 = 1
X2 = 5
Y1 = 1
Y2 = 5


org 100h                               

start:		
		mov ax, 0b800h
		mov es, ax
		mov dh, 160	;    na eto budem delit'

		mov bx, (Y1-1)*80*2 + (X1-1)*2	
		jmp for_one

	for_two:
		add bx, 80*2 - X2*2 + X1*2 - 2	


	for_one:	
		mov word ptr es:[bx], 4e21h                 
		
		add bx, 2
		
		mov ax, bx
		div dh				;ah - mod
		
		cmp ah, X2*2
	jl for_one                       
		
		cmp al, Y2-1
	jl for_two

		mov bx, Y1*80*2 + X1*2
		jmp for_three

	for_four:
		add bx, 80*2 - (X2-1)*2 + (X1+1)*2 - 2

	for_three:
		mov word ptr es:[bx], 4e20h

		add bx, 2
		mov ax, bx
		div dh

		cmp ah, (X2-1)*2
	jl for_three

		cmp al, Y2-2
	jl for_four

		
		
	

		mov ax, 4c00h           
		int 21h                 

.data


end start