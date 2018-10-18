obj=main.o user_manager.o communication.o user_interface.o mytcp.o
CC=gcc
CFLAGS= -Wall -O2


ipmsg:$(obj)
	$(CC) -o $@ $^ -lpthread

%O:%C %h
	$(CC) -c -o $@ $^

clean:
	@rm $(obj) -rf main -rf
