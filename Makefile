OUTPUT=skalamat
TEST_OUTPUT=test.out
BIN_DIR=bin

CXX=g++
LD=g++
CFLAGS=-Wall -pedantic -Wno-long-long -O0 -ggdb -pthread --std=c++11

OBJECTS=\
	$(BIN_DIR)/Application.o\
	$(BIN_DIR)/Exception.o\
	$(BIN_DIR)/Game.o\
	$(BIN_DIR)/BootstrapController.o\
	$(BIN_DIR)/Controller.o\
	$(BIN_DIR)/GameController.o\
	$(BIN_DIR)/BangCard.o\
	$(BIN_DIR)/BeerCard.o\
	$(BIN_DIR)/Bot.o\
	$(BIN_DIR)/Card.o\
	$(BIN_DIR)/CharacterCard.o\
	$(BIN_DIR)/DiscardCard.o\
	$(BIN_DIR)/DrawCard.o\
	$(BIN_DIR)/GunCard.o\
	$(BIN_DIR)/InstantCard.o\
	$(BIN_DIR)/MissCard.o\
	$(BIN_DIR)/PermanentCard.o\
	$(BIN_DIR)/PlayableCard.o\
	$(BIN_DIR)/Player.o\
	$(BIN_DIR)/RoleCard.o\
	$(BIN_DIR)/GameClient.o\
	$(BIN_DIR)/GameServer.o\
	$(BIN_DIR)/CardsParser.o\
	$(BIN_DIR)/CsvParser.o\
	$(BIN_DIR)/utils.o

all: compile doc

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

compile: $(BIN_DIR) $(OBJECTS) $(BIN_DIR)/main.o
	$(LD) $(CFLAGS) -o $(OUTPUT) $(OBJECTS) $(BIN_DIR)/main.o

run:
	./$(OUTPUT) res/cards.csv

compileTest: $(BIN_DIR) $(OBJECTS) $(BIN_DIR)/test.o
	$(LD) $(CFLAGS) -o $(TEST_OUTPUT) $(OBJECTS) $(BIN_DIR)/test.o

runTest:
	./$(TEST_OUTPUT)

test: compileTest runTest

clean:
	rm -rf $(OUTPUT) $(OUTPUT).dSYM $(TEST_OUTPUT) $(TES_OUTPUT).dSYM $(BIN_DIR) doc

doc:
	doxygen Doxyfile


$(BIN_DIR)/Application.o: src/Application.cpp src/Application.h src/Game.h \
	src/entity/Player.h src/entity/RoleCard.h src/entity/Card.h \
	src/entity/CharacterCard.h src/entity/PlayableCard.h \
	src/entity/PermanentCard.h src/net/GameClient.h \
	src/controller/BootstrapController.h src/controller/Controller.h \
	src/controller/GameController.h src/controller/../net/GameServer.h \
	src/net/../entity/Bot.h src/util/CardsParser.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/Exception.o: src/Exception.cpp src/Exception.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/Game.o: src/Game.cpp src/Game.h src/entity/Player.h src/entity/RoleCard.h \
	src/entity/Card.h src/entity/CharacterCard.h src/entity/PlayableCard.h \
	src/entity/PermanentCard.h src/entity/Bot.h src/net/GameServer.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/main.o: src/main.cpp src/Exception.h src/Application.h src/Game.h \
	src/entity/Player.h src/entity/RoleCard.h src/entity/Card.h \
	src/entity/CharacterCard.h src/entity/PlayableCard.h \
	src/entity/PermanentCard.h src/net/GameClient.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/BootstrapController.o: src/controller/BootstrapController.cpp \
	src/controller/BootstrapController.h src/controller/Controller.h \
	src/controller/../Game.h src/controller/../entity/Player.h \
	src/controller/../entity/RoleCard.h src/controller/../entity/Card.h \
	src/controller/../entity/CharacterCard.h \
	src/controller/../entity/PlayableCard.h \
	src/controller/../entity/PermanentCard.h \
	src/controller/../net/GameClient.h src/controller/../Exception.h \
	src/controller/../net/GameServer.h \
	src/controller/../net/../entity/Bot.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/Controller.o: src/controller/Controller.cpp src/controller/Controller.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/GameController.o: src/controller/GameController.cpp \
	src/controller/../util/utils.h src/controller/GameController.h \
	src/controller/Controller.h src/controller/../Game.h \
	src/controller/../entity/Player.h src/controller/../entity/RoleCard.h \
	src/controller/../entity/Card.h \
	src/controller/../entity/CharacterCard.h \
	src/controller/../entity/PlayableCard.h \
	src/controller/../entity/PermanentCard.h \
	src/controller/../net/GameClient.h src/controller/../net/GameServer.h \
	src/controller/../net/../entity/Bot.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/BangCard.o: src/entity/BangCard.cpp src/entity/BangCard.h \
	src/entity/InstantCard.h src/entity/PlayableCard.h src/entity/Card.h \
	src/entity/../Game.h src/entity/../entity/Player.h \
	src/entity/RoleCard.h src/entity/CharacterCard.h \
	src/entity/PermanentCard.h src/entity/../net/GameServer.h \
	src/entity/../net/../entity/Bot.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/BeerCard.o: src/entity/BeerCard.cpp src/entity/BeerCard.h \
	src/entity/InstantCard.h src/entity/PlayableCard.h src/entity/Card.h \
	src/entity/../Game.h src/entity/../entity/Player.h \
	src/entity/RoleCard.h src/entity/CharacterCard.h \
	src/entity/PermanentCard.h src/entity/../net/GameServer.h \
	src/entity/../net/../entity/Bot.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/Bot.o: src/entity/Bot.cpp src/entity/Bot.h src/entity/Player.h \
	src/entity/RoleCard.h src/entity/Card.h src/entity/CharacterCard.h \
	src/entity/PlayableCard.h src/entity/PermanentCard.h \
	src/entity/MissCard.h src/entity/InstantCard.h src/entity/../Game.h \
	src/entity/BeerCard.h src/entity/../net/GameServer.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/Card.o: src/entity/Card.cpp src/entity/Card.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/CharacterCard.o: src/entity/CharacterCard.cpp src/entity/CharacterCard.h \
	src/entity/Card.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/DiscardCard.o: src/entity/DiscardCard.cpp src/entity/DiscardCard.h \
	src/entity/InstantCard.h src/entity/PlayableCard.h src/entity/Card.h \
	src/entity/../Game.h src/entity/../entity/Player.h \
	src/entity/RoleCard.h src/entity/CharacterCard.h \
	src/entity/PermanentCard.h src/entity/../net/GameServer.h \
	src/entity/../net/../entity/Bot.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/DrawCard.o: src/entity/DrawCard.cpp src/entity/DrawCard.h \
	src/entity/InstantCard.h src/entity/PlayableCard.h src/entity/Card.h \
	src/entity/../Game.h src/entity/../entity/Player.h \
	src/entity/RoleCard.h src/entity/CharacterCard.h \
	src/entity/PermanentCard.h src/entity/../net/GameServer.h \
	src/entity/../net/../entity/Bot.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/GunCard.o: src/entity/GunCard.cpp src/entity/GunCard.h \
	src/entity/PermanentCard.h src/entity/PlayableCard.h src/entity/Card.h \
	src/entity/Player.h src/entity/RoleCard.h src/entity/CharacterCard.h \
	src/entity/../net/GameServer.h src/entity/../net/../Game.h \
	src/entity/../net/../entity/Bot.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/InstantCard.o: src/entity/InstantCard.cpp src/entity/InstantCard.h \
	src/entity/PlayableCard.h src/entity/Card.h \
	src/entity/../net/GameServer.h src/entity/../net/../Game.h \
	src/entity/../net/../entity/Player.h src/entity/RoleCard.h \
	src/entity/CharacterCard.h src/entity/PermanentCard.h \
	src/entity/../net/../entity/Bot.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/MissCard.o: src/entity/MissCard.cpp src/entity/MissCard.h \
	src/entity/InstantCard.h src/entity/PlayableCard.h src/entity/Card.h \
	src/entity/../Game.h src/entity/../entity/Player.h \
	src/entity/RoleCard.h src/entity/CharacterCard.h \
	src/entity/PermanentCard.h src/entity/../net/GameServer.h \
	src/entity/../net/../entity/Bot.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/PermanentCard.o: src/entity/PermanentCard.cpp src/entity/PermanentCard.h \
	src/entity/PlayableCard.h src/entity/Card.h src/entity/Player.h \
	src/entity/RoleCard.h src/entity/CharacterCard.h \
	src/entity/../net/GameServer.h src/entity/../net/../Game.h \
	src/entity/../net/../entity/Bot.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/PlayableCard.o: src/entity/PlayableCard.cpp src/entity/PlayableCard.h \
	src/entity/Card.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/Player.o: src/entity/Player.cpp src/entity/Player.h src/entity/RoleCard.h \
	src/entity/Card.h src/entity/CharacterCard.h src/entity/PlayableCard.h \
	src/entity/PermanentCard.h src/entity/GunCard.h src/entity/BeerCard.h \
	src/entity/InstantCard.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/RoleCard.o: src/entity/RoleCard.cpp src/entity/RoleCard.h \
	src/entity/Card.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/GameClient.o: src/net/GameClient.cpp src/net/GameServer.h \
	src/net/../Game.h src/net/../entity/Player.h \
	src/net/../entity/RoleCard.h src/net/../entity/Card.h \
	src/net/../entity/CharacterCard.h src/net/../entity/PlayableCard.h \
	src/net/../entity/PermanentCard.h src/net/../entity/Bot.h \
	src/net/../Exception.h src/net/../util/utils.h src/net/GameClient.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/GameServer.o: src/net/GameServer.cpp src/net/../Exception.h \
	src/net/../util/utils.h src/net/../entity/Bot.h \
	src/net/../entity/Player.h src/net/../entity/RoleCard.h \
	src/net/../entity/Card.h src/net/../entity/CharacterCard.h \
	src/net/../entity/PlayableCard.h src/net/../entity/PermanentCard.h \
	src/net/GameServer.h src/net/../Game.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/CardsParser.o: src/util/CardsParser.cpp src/util/CardsParser.h \
	src/util/../entity/Card.h src/util/CsvParser.h \
	src/util/../entity/RoleCard.h src/util/../entity/InstantCard.h \
	src/util/../entity/PlayableCard.h src/util/../entity/PermanentCard.h \
	src/util/../entity/GunCard.h src/util/../entity/CharacterCard.h \
	src/util/../entity/BangCard.h src/util/../entity/MissCard.h \
	src/util/../entity/../Game.h src/util/../entity/../entity/Player.h \
	src/util/../entity/BeerCard.h src/util/../entity/DrawCard.h \
	src/util/../entity/DiscardCard.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/CsvParser.o: src/util/CsvParser.cpp src/util/CsvParser.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/utils.o: src/util/utils.cpp src/util/utils.h
	$(CXX) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/test.o: examples/test.cpp examples/CsvParserTest.cpp \
	examples/../src/util/CsvParser.h examples/CardsParserTest.cpp \
	examples/../src/util/CardsParser.h \
	examples/../src/util/../entity/Card.h \
	examples/../src/entity/RoleCard.h \
	examples/../src/entity/CharacterCard.h \
	examples/../src/entity/GunCard.h \
	examples/../src/util/../entity/PermanentCard.h \
	examples/../src/util/../entity/PlayableCard.h \
	examples/GameServerTest.cpp examples/../src/net/GameServer.h \
	examples/../src/net/../Game.h examples/../src/net/../entity/Player.h \
	examples/../src/net/../entity/Bot.h examples/../src/net/GameClient.h \
	examples/../src/Exception.h examples/GameTest.cpp \
	examples/../src/entity/BangCard.h \
	examples/../src/util/../entity/InstantCard.h \
	examples/../src/entity/DiscardCard.h examples/../src/entity/DrawCard.h
	$(CXX) $(CFLAGS) -c -o $@ $<
