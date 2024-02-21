CC=gcc
target=main

obj=main.o

# gcc all.obj -o main
$(target):$(obj)
	time $(CC) $(obj) ../lua_lib/liblua53.so -o $(target) -lm -ldl

%.o:%.c
	$(CC) -c $< -o $@ 

.PHONY:clean
clean:
	rm -rf $(obj) $(target)