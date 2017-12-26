server: server.o chat.o info.o alert.o
	gcc -o server server.o chat.o info.o alert.o -lpthread
server.o: config.h server.c
	gcc -c server.c
chat.o: config.h chat.c
	gcc -c chat.c
info.o: config.h info.c
	gcc -c info.c
alert.o: config.h alert.c
	gcc -c alert.c
clean:
	rm -rf *.o