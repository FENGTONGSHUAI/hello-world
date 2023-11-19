test: tongshuai.o multiple_processes_writing_file_simultaneously.o
	gcc -Wall -g tongshuai.o multiple_processes_writing_file_simultaneously.o -o test
tongshuai.o: tongshuai.c
	gcc -c tongshuai.c -o tongshuai.o
multiple_processes_writing_file_simultaneously.o: multiple_processes_writing_file_simultaneously.c
	gcc -c multiple_processes_writing_file_simultaneously.c -o multiple_processes_writing_file_simultaneously.o
.PHONY:clean
clean:
	-rm -rf *.o test