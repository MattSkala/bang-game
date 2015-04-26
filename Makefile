all: compile doc

compile:
	g++ -Wall -pedantic -Wno-long-long -O0 -ggdb src/main.cpp src/Application.cpp src/Game.cpp src/controller/Controller.cpp src/controller/BootstrapController.cpp src/Card.cpp src/Player.cpp src/util/CardsParser.cpp -o skalamat

run:
	./skalamat

clean:
	rm -rf skalamat skalamat.dSYM doc

doc:
	doxygen Doxyfile