#include<SFML/Graphics.hpp>
#include<vector>
#include<cmath>
#include<iostream>

#include"MCTS.h"

class TicTacToeUI {
public:
    TicTacToeUI();
    void run();
private:
    const int fieldSize = 3;
    const int windowSize = 600;
    const int targetSize = 3;
    
    int gridWidth;

    int toeRadius;
    int outline;
    const double goldenRatio = 1.618;
    const double outlinePercents = 0.25;
    const int widthK = 2; // Коэффициент ширины ноликов

    int crossWidth;
    int crossHeight;
    const int crossAngel = 45;

    bool isUnpressed;

    void init();
    void drawField();
    void handleInput(sf::Event event);

    void toeDraw(int i, int j);
    void crossDraw(int i, int j);
    void cellDraw(int i, int j);

    sf::RenderWindow window;
    std::vector<std::vector<char>> field; // '0' - пусто, '1' - крестик, '2' - нолик
    int currentPlayer;
    int cellSize;
    int gameWinner;

    MCT bot;
};

TicTacToeUI::TicTacToeUI() 
    : window(sf::VideoMode(windowSize, windowSize), "Крестики-Нолики"), 
      cellSize(windowSize / fieldSize), 
      isUnpressed(true) {
            toeRadius = (double)cellSize / 2 / goldenRatio;
            outline = (double)toeRadius * outlinePercents;
            crossHeight = sqrt(2) * (double)cellSize / goldenRatio;
            crossWidth = (double) crossHeight / 4 / goldenRatio;
            gridWidth = cellSize * outlinePercents / 4;
            init();
      }

void TicTacToeUI::init() {
    field = std::vector<std::vector<char>>(fieldSize, std::vector<char>(fieldSize, 0));
    currentPlayer = 1;
    gameWinner = -1; 
    window.setTitle("TicTacToe");
}

void TicTacToeUI::run() {
    drawField();
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::MouseButtonReleased || sf::Event::KeyReleased)
                isUnpressed = true;
            if (event.type == sf::Event::Closed)
                window.close();
            if ((event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::KeyPressed) && isUnpressed) {
                if (gameWinner == -1) {
                    isUnpressed = false;
                    handleInput(event);
                    gameWinner = bot.update(field, targetSize);
                    drawField();
                } else {
                    field = std::vector<std::vector<char>>(fieldSize, std::vector<char>(fieldSize, 0));
                    currentPlayer = 1;
                    gameWinner = -1;
                    window.setTitle("TicTacToe");
                    drawField();
                }
            }
        }
    }
}

void TicTacToeUI::handleInput(sf::Event event) {
    if (event.mouseButton.button == sf::Mouse::Left) { // User vs user
        int x = event.mouseButton.x / cellSize;
        int y = event.mouseButton.y / cellSize;
        if (x < fieldSize && y < fieldSize && field[y][x] == 0) {
            field[y][x] = currentPlayer;
            currentPlayer = 3 - currentPlayer; // Смена игрока
        }
    } else if(event.type == sf::Event::KeyPressed) { // User vs bot
        std::pair<int, int> move = bot.getMove(field, currentPlayer);
        field[move.first][move.second] = currentPlayer;
        currentPlayer = 3 - currentPlayer;
    }
}

void TicTacToeUI::drawField() {
    window.clear(sf::Color::White);

    for (int i = 0; i < fieldSize; ++i) {
        for (int j = 0; j < fieldSize; ++j) {
            cellDraw(i, j);
            if (field[i][j] == 1) {
                crossDraw(i, j);
            } else if (field[i][j] == 2) {
                toeDraw(i, j);
            }
        }
    }

    window.display();
}

void TicTacToeUI::cellDraw(int i, int j) {
    sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
    cell.setPosition(j * cellSize + gridWidth / 2, i * cellSize + gridWidth / 2);
    cell.setOutlineColor(sf::Color::Black);
    cell.setOutlineThickness(gridWidth);
    window.draw(cell);
}

void TicTacToeUI::toeDraw(int i, int j) {
    sf::CircleShape toe(toeRadius);
    toe.setPosition(j * cellSize + toeRadius - 3 * outline / 2, 
                      i * cellSize + toeRadius - 3 * outline / 2);
    toe.setOutlineColor(sf::Color::Red);
    toe.setOutlineThickness(outline * widthK);
    window.draw(toe);
}

void TicTacToeUI::crossDraw(int i, int j) {
    sf::RectangleShape rect1(sf::Vector2f(crossWidth, crossHeight));
    sf::Vector2f Position(rect1.getPosition().x + crossWidth / 2, rect1.getPosition().y + crossHeight / 2);
    rect1.setOrigin(Position);
    rect1.setPosition(j * cellSize + cellSize / 2, 
                      i * cellSize + cellSize / 2);
    rect1.rotate(180 - crossAngel);
    rect1.setOutlineColor(sf::Color::Blue);
    rect1.setOutlineThickness(outline);
    window.draw(rect1);
    rect1.rotate(45 + crossAngel);
    window.draw(rect1);
}

