all:client server

client:client.cc
	g++ $^ -std=c++0x -lpthread -o $@ -lboost_system -lboost_filesystem
server:server.cc
	g++ $^ -std=c++0x -lpthread -o $@ -lboost_system -lboost_filesystem

.PHONY:clean
clean:
	rm server
