all:
	g++ -std=c++11 -Wall -O0 -g -I src/include -L src/lib -o main main.cpp -lmingw32 -lSDL2main -lSDL2