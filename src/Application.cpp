#include "Application.h"
#include "controller/BootstrapController.h"
#include "util/CardsParser.h"
#include "signal.h"


Application::~Application() {
    if (server_pid_ != 0) {
        kill(server_pid_, SIGTERM);
    }
}

void Application::init(string filename) {
    // Load cards pack from file
    vector<Card *> cards = CardsParser().parseFile(filename);
    game_.setPack(cards);

    // Render welcome screen
    BootstrapController(game_, client_, server_pid_).renderWelcome();
}
