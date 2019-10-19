client: client.o make_client_socket.o
	gcc client.o make_client_socket.o -o client

client.o: client.c
	gcc -c client.c

make_client_socket.o: macro/make_client_socket.c
	gcc -c macro/make_client_socket.c

server: server.o make_server_socket.o server_functions.o split.o
	gcc server.o make_server_socket.o server_functions.o split.o -lpthread -o server

server.o: server.c
	gcc -c server.c

make_server_socket.o: macro/make_server_socket.c
	gcc -c macro/make_server_socket.c

server_functions.o: macro/server_functions.c
	gcc -c macro/server_functions.c

split.o: split/split.c
	gcc -c split/split.c

clean:
	rm *.o
	rm client server
