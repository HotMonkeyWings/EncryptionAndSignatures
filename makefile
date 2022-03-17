CXX = g++

all: compile

compile: 
	g++ -g -O2 -std=c++11 -pthread -march=native src/$(file).cpp -o outputs/$(file).o -lntl -lgmp -lm

clean:
	rm *.o
