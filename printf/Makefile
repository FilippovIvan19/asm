PROG = printf
PROG_OBJ = printf.o TOBIN.o TOHEX.o TODEC.o CU.o
CC = nasm

all: $(PROG)

$(PROG) : $(PROG_OBJ)
	ld -o $@ $^

%.o : %.s
	$(CC) -f elf64 $^

clean : 
	rm -f *.o $(PROG_OBJ) $(PROG)