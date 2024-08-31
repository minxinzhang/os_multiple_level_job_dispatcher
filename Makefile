CC = gcc
CFLAGS = -Wall

mlq: pcb.c mlq.c
	$(CC) $(CFLAGS) -o mlq mab.c pcb.c mlq.c
	./mlq tests/test_file_1
	./mlq tests/test_file_2

test:
	rm -f mlq
	$(CC) $(CFLAGS) -o mlq mab.c pcb.c mlq.c
	cp utility/process .
	./mlq tests/test_file_1
	./mlq tests/test_file_2

unittest: 
	$(CC) -o test mab.c pcb.c tests/test_mab.c
	./test
	
clean:
	rm -f process
	rm -f mlq
	rm -f test