build:
	g++ -Wall ./src/*.cpp ./src/*.h -lSDL2 -o game
run:
	./game
clear:
	rm game


