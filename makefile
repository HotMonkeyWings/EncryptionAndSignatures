CXX = g++

all: compile

compile: 
	g++ -g -O2 -std=c++1z -pthread -march=native src/$(file).cpp -o build/$(file) -lntl -lgmp -lm

clean:
	rm build/*
