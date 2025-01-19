#include"../include/Mechanics.h"

bool Mechanics::isDraw() {
    for (int i = 0; i < fieldSize; ++i) {
            for (int j = 0; j < fieldSize; ++j) {
                if (!field[i][j]) return false;
            }
        }
    return true;
}

int Mechanics::randomSimulation(int field_[fieldSize][fieldSize], int currPlayer) {
    for (int i = 0; i < fieldSize; ++i) {
        for (int j = 0; j < fieldSize; ++j) {
            trialField[i][j] = field_[i][j];
        }
    }
    std::vector<int> moves;

    for (int i = 0; i < fieldSize; ++i) {
        for (int j = 0; j < fieldSize; ++j) {
            if (trialField[i][j] == 0) {
                moves.push_back(i * fieldSize + j);
            }
        }
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(moves.begin(), moves.end(), g);
    //std::cout << " Moves: ";
    //for (auto i : moves) {std::cout << i << " " ;}
    //std::cout << "\n\n";

    for (int move : moves) {
        trialField[move / fieldSize][move % fieldSize] = currPlayer;
        currPlayer = 3 - currPlayer;
        int res = update(trialField, move / fieldSize, move % fieldSize);
        if (res) {
            //if (field_[1][2] != 0) {
            //    for (int i = 0; i < fieldSize; ++i) {
            //        for (int j = 0; j < fieldSize; ++j) {
            //            std::cout << trialField[i][j] << " ";
            //        }
            //        std::cout << "\n";
            //    }
            //    std::cout << std::endl;
            //}
            return res;
        }
    }

    return 3; // Ничья
}

int Mechanics::check(int x, int y, int dx, int dy) {
    int counter = 0;
    int currX = x, currY = y;
    while((((currX) < fieldSize) && ((currY) < fieldSize) && ((currX) >= 0) && ((currY) >= 0)) && (field[x][y] == field[currX][currY])) {
        currX += dx;
        currY += dy;
        counter ++;
    }
    if (counter == targetSize) return field[x][y];
    counter --;
    currX = x;
    currY = y;
    while((((currX) < fieldSize) && ((currY) < fieldSize) && ((currX) >= 0) && ((currY) >= 0)) && (field[x][y] == field[currX][currY])) {
        currX -= dx;
        currY -= dy;
        counter ++;
    }
    if (counter == targetSize) return field[x][y];
    return 0;
}
// 0 - ничья, 1 - крестики, 2 - нолики
int Mechanics::update(int x, int y) {
    if (check(x, y, 1, 1)) return field[x][y];
    if (check(x, y, 1, -1)) return field[x][y];
    if (check(x, y, 0, 1)) return field[x][y];
    if (check(x, y, 1, 0)) return field[x][y];

    return 0;
}

int Mechanics::check(int field[fieldSize][fieldSize], int x, int y, int dx, int dy) {
    int counter = 0;
    int currX = x, currY = y;
    while((((currX) < fieldSize) && ((currY) < fieldSize) && ((currX) >= 0) && ((currY) >= 0)) && (field[x][y] == field[currX][currY])) {
        currX += dx;
        currY += dy;
        counter ++;
    }
    if (counter == targetSize) return field[x][y];
    counter --;
    currX = x;
    currY = y;
    while((((currX) < fieldSize) && ((currY) < fieldSize) && ((currX) >= 0) && ((currY) >= 0)) && (field[x][y] == field[currX][currY])) {
        currX -= dx;
        currY -= dy;
        counter ++;
    }
    if (counter == targetSize) return field[x][y];
    return 0;
}

int Mechanics::update(int field[fieldSize][fieldSize], int x, int y) {
    if (check(field, x, y, 1, 1)) return field[x][y];
    if (check(field, x, y, 1, -1)) return field[x][y];
    if (check(field, x, y, 0, 1)) return field[x][y];
    if (check(field, x, y, 1, 0)) return field[x][y];

    return 0;
}