#include <iostream>
#include <ctime>
#include "CsvParserTest.cpp"
#include "CardsParserTest.cpp"
#include "GameServerTest.cpp"


int main(int argc, char * argv[]) {
    clock_t begin = clock();

    //testCsvParser();
    //testCardsParser();
    testGameServer();

    clock_t end = clock();
    double duration = (double(end - begin) / CLOCKS_PER_SEC) * 1000;
    std::cout << "Running tests finished successfully in " << duration << " ms" << std::endl;

    return EXIT_SUCCESS;
}
