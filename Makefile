default: debug

Filename := main.cpp
Exec := $(basename $(Filename)).exe

build: $(Exec)

debug: $(Exec)
	./$^
#del $^

$(Exec): $(Filename)
	g++ -o $@ -D DEBUG -g -std=c++20 $^

