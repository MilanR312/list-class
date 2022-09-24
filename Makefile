.PHONY: build run clean

default: run

Filename := test.cpp
Exec := $(basename $(Filename)).exe

Flags := -D DEBUG -g -std=c++20
files := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
DIR = $(CURDIR)


build: $(Exec) clean

run: $(Exec) clean
	./$<

$(Exec): $(OBJS)
	g++ $^ -o $@ $(Flags)

%.o: %.cpp
	g++ -c $< -o $@ $(Flags)

clean:
	rm *.o