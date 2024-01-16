.PHONY:all
all:Client Server

Client:Client.cc
	g++ -o $@ $^ -std=c++11
Server:Server.cc
	g++ -o $@ $^ -std=c++11
.PHONY:clean
clean:
	rm -rf Client Server