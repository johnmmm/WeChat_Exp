server: server.o chat.o
	gcc -o server server.o chat.o
server.o: config.h server.c
	gcc -c server.c
chat.o: config.h chat.c
	gcc -c chat.c
clean:
	rm -rf *.o