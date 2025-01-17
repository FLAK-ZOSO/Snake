#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <cstring>
#include <time.h>
// g++ snake.cpp -o snake.exe -std=c++2a
// g++ -S snake.cpp -std=c++2a
const char SNAKE = '$';
const char BORDER = '#';
const char APPLE = '0';
const std::string VERSION = "v0.3.0";


void gameover(char matrix[20][20], unsigned long long points) {
    for (int i = 0; i <20; i++) {
        if (matrix[i][0] == SNAKE || matrix[i][19] == SNAKE || matrix[0][i] == SNAKE || matrix[19][i] == SNAKE) {
            std::cout << "Game over!" << std::endl << "Your points: " << points << std::endl;
            exit(0);
        }
    }
}


bool appleCheck(char field[20][20], int x, int y) {
    return (field[x][y] == APPLE);
}


void addApple(char field[20][20], int x, int y) {
    int a, b;
    do {
        a = 1+rand()%17;
        b = 1+rand()%17;
    } while (a == x && b == y);
    field[a][b] = APPLE;
}


void move(char matrix[][20], char direction, int & x, int & y, unsigned long long points) {
    matrix[x][y] = ' ';
    if (direction == 'w')
        x--;
    else if (direction == 's')
        x++;        
    else if (direction == 'a')
        y--;
    else if (direction == 'd')
        y++;
    if(appleCheck(matrix, x, y))
        addApple(matrix, x, y);
    matrix[x][y]=SNAKE;
    gameover(matrix, points);
}


std::string inputMove() {
    std::string move;
    if (std::cin >> move)
        return move;
}


void print(char m[][20]) {
    system("cls");
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++)
            std::cout << m[i][j];
        std::cout << std::endl;
    }
}


void changeDirection(char & direction, std::string move) {
    if (move == "w")
        direction = 'w';
    else if (move == "s")
        direction = 's';
    else if (move == "a")
        direction = 'a';
    else if (move == "d")
        direction = 'd';
}


int main(int argc, char *argv[]) {
    float seconds = 0.2;
    if (argc == 1) {
        std::cout << "Period [seconds]: ";
        std::cin >> seconds;
    } else if (argc == 2 && !strcmp(argv[1], "--help")) {
        std::cout << "Usage: snake [--help] [--version] [--frequency | --delay]" << std::endl << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "  --help         Show this help message and exit" << std::endl;
        std::cout << "  --version      Show version and exit" << std::endl;
        std::cout << "  --frequency    Set the frame period in seconds" << std::endl;
        std::cout << "  --delay        Set the frame period in seconds" << std::endl;
        std::cout << "Examples:" << std::endl;
        std::cout << "  snake --help" << std::endl;
        std::cout << "  snake --frequency 0.2" << std::endl;
        std::cout << "  snake --delay 0.2" << std::endl;
        return 0;
    } else if (argc == 2 && !strcmp(argv[1], "--version")) {
        std::cout << VERSION << std::endl;
        return 0;
    } else if (argc == 3 && !strcmp(argv[1], "--frequency")) {
        seconds = atof(argv[2]);
    } else if (argc == 3 && !strcmp(argv[1], "--delay")) {
        seconds = atof(argv[2]);
    } else if (argc > 2) {
        std::cout << "Usage: snake [--help] [--version] [--frequency | --delay]" << std::endl;
        return 0;
    }
    std::chrono::duration<double> period(seconds);

    srand(time(NULL));
    unsigned long long points = 0;
    char field[20][20];
    int x=9, y=9;
    char direction = 'w';
    for (int i = 0; i <20; i++) {
        for (int j = 0; j <20; j++)
            field[i][j] = ' ';
    }
    for (int i=0; i<20; i++) {
        field[0][i] = BORDER;
        field[i][0] = BORDER;
        field[19][i] = BORDER;
        field[i][19] = BORDER;
        field[x][y] = SNAKE;
    }

    addApple(field, x, y);
    print(field);

    while (true) {
	    auto input = std::async(std::launch::async, inputMove);
		while (input.wait_for(period) != std::future_status::ready) {
			move(field, direction, x, y, points);
            print(field);
		}
        changeDirection(direction, input.get());
        points++;
    }
}