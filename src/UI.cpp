#include"../include/UI.h"

TicTacToeUI::TicTacToeUI() 
    : window(sf::VideoMode(windowSize, windowSize), "Крестики-Нолики"), 
      cellSize(windowSize / fieldSize), bot(&mech), unpressedSpace(true), unpressedL(true) {
            toeRadius = (double)cellSize / 2 / goldenRatio;
            outline = (double)toeRadius * outlinePercents;
            crossHeight = sqrt(2) * (double)cellSize / goldenRatio;
            crossWidth = (double) crossHeight / 4 / goldenRatio;
            gridWidth = cellSize * outlinePercents / 4;
            init();
      }

void TicTacToeUI::init() {
    mech.clearField();
    currentPlayer = 1;
    gameWinner = 0; 
    window.setTitle("TicTacToe");
}

void TicTacToeUI::run() {
    drawField();
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Space) {
                unpressedSpace = true;
            }
            if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::L) {
                unpressedL = true;
            }
            if (!gameWinner && (event.type == sf::Event::MouseButtonPressed || event.key.code == sf::Keyboard::Space)) {
                std::cout << currentPlayer << " player\n";
                handleInput(event);
                std::cout << currentPlayer << " player\n";
                drawField();
            }
            if (gameWinner) {
                if (gameWinner == 3) 
                window.setTitle("TicTacToe Draw!!");
                else window.setTitle((gameWinner == 1) ? "TicTacToe winner: Cross!!":"TicTacToe winner: Toe!!");
            }
            if ((event.key.code == sf::Keyboard::R)) {
                mech.clearField();
                currentPlayer = 1;
                gameWinner = 0;
                window.setTitle("TicTacToe");
                drawField();
            }
            if ((event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::L) && unpressedL) {
                std::cout << "change!!!\n";
                unpressedL = false;
                mech.field[mech.lastY][mech.lastX] = 0;
                currentPlayer = 3 - currentPlayer;
                window.setTitle("TicTacToe");
                drawField();
            }
        }
    }
}

void TicTacToeUI::handleInput(sf::Event event) {
    if (event.mouseButton.button == sf::Mouse::Left) { // User vs user
        int x = event.mouseButton.x / cellSize;
        int y = event.mouseButton.y / cellSize;
        if (x < fieldSize && y < fieldSize && mech.field[y][x] == 0) {
            mech.field[y][x] = currentPlayer;
            currentPlayer = 3 - currentPlayer; // Смена игрока
            gameWinner = mech.update(y, x);
            mech.lastX = x;
            mech.lastY = y;
            if (mech.isDraw()) gameWinner = 3;
        }
        std::cout << gameWinner  << " " << y << " " << x << "\n";
    } else if(event.key.code == sf::Keyboard::Space && unpressedSpace && event.type == sf::Event::KeyPressed) { // User vs bot
        std::pair<int, int> move = bot.getMove(mech.field, 3 - currentPlayer);
        mech.field[move.first][move.second] = currentPlayer;
        mech.lastX = move.second;
        mech.lastY = move.first;
        currentPlayer = 3 - currentPlayer;
        unpressedSpace = false;
        gameWinner = mech.update(move.first, move.second);
        if (mech.isDraw()) gameWinner = 3;
        std::cout << gameWinner << " " << move.first << " " << move.second << "\n";
    }
}

void TicTacToeUI::drawField() {
    window.clear(sf::Color::White);

    for (int i = 0; i < fieldSize; ++i) {
        for (int j = 0; j < fieldSize; ++j) {
            cellDraw(i, j);
            if (mech.field[i][j] == 1) {
                crossDraw(i, j);
            } else if (mech.field[i][j] == 2) {
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
