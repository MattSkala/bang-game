all: compile doc

compile:
	g++ -Wall -pedantic -Wno-long-long -O0 -ggdb -pthread --std=c++11 src/main.cpp src/Application.cpp src/Game.cpp src/controller/Controller.cpp src/controller/BootstrapController.cpp src/entity/Card.cpp src/entity/RoleCard.cpp src/entity/CharacterCard.cpp src/entity/InstantCard.cpp src/entity/PermanentCard.cpp src/entity/GunCard.cpp src/entity/Player.cpp src/util/CsvParser.cpp src/util/CardsParser.cpp src/util/utils.cpp src/net/GameServer.cpp src/net/GameClient.cpp -o skalamat

run:
	./skalamat res/cards.csv

valgrind:
	valgrind ./skalamat res/cards.csv

compileTest:
	g++ -Wall -pedantic -Wno-long-long -O0 -ggdb -pthread --std=c++11 test/test.cpp src/Application.cpp src/Game.cpp src/controller/Controller.cpp src/controller/BootstrapController.cpp src/entity/Card.cpp src/entity/RoleCard.cpp src/entity/CharacterCard.cpp src/entity/InstantCard.cpp src/entity/PermanentCard.cpp src/entity/GunCard.cpp src/entity/Player.cpp src/util/CsvParser.cpp src/util/CardsParser.cpp src/util/utils.cpp src/net/GameServer.cpp src/net/GameClient.cpp -o test.out

runTest:
	./test.out

test: compileTest runTest

clean:
	rm -rf skalamat skalamat.dSYM doc test.out

doc:
	doxygen Doxyfile
