.PHONY:all
all:kill mysignal
kill:kill.cc
	g++ -o $@ $^ -std=c++11
mysignal:mysignal.cc
	g++ -o $@ $^ -std=c++11
.PHONY:clean
clean:
	rm -rf mysignal kill