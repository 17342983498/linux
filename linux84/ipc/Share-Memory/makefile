.PHONY:all
all:shmclient shmserver

shmclient:client.cc
	g++ -o $@ $^ -std=c++11
shmserver:server.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -f shmserver shmclient