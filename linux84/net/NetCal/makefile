.PHONY:all
all:calserver calclient

calserver:CalculatorServer.cc
	g++ -o $@ $^ -std=c++11 -lpthread -ljsoncpp
calclient:CalculatorClient.cc
	g++ -o $@ $^ -std=c++11 -ljsoncpp

.PHONY:clean
clean:
	rm -f calserver calclient