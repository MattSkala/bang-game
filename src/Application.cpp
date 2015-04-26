#include "Application.h"
#include "controller/BootstrapController.h"
#include "util/CardsParser.h"


void Application::init() {
    // Load cards pack from file
    vector<Card *> cards = CardsParser().parseFile("../res/cards.csv");
    game_.setPack(cards);

    // Render welcome screen
    BootstrapController(game_).renderWelcome();
}
