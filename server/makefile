server: server.o chat.o info.o
	gcc -o server server.o chat.o info.o
server.o: config.h server.c
	gcc -c server.c
chat.o: config.h chat.c
	gcc -c chat.c
info.o: config.h info.c
	gcc -c info.c
clean:
	rm -rf *.o