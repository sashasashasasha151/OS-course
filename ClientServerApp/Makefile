.PHONY: all clean run

all:
	$(info Use 'run' to run server and client, 'run_server' to run server, 'run client' to run client)
	exit

run: compil run_server run_client

compil: client server

run_server: server
	gnome-terminal -- "./server"

run_client: client
	gnome-terminal -- "./client"

client: client.cpp
	g++ -o client client.cpp

server: server.cpp
	g++ -o server server.cpp
