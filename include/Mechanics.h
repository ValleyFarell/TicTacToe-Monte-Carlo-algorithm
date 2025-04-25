#include<vector>
#include <cstring>
#include<algorithm>
#include<random>

#ifndef MECHANICS
#define MECHANICS

const int fieldSize = 9;
const int targetSize = 5;

class Mechanics {
private:
    int check(int x, int y, int dx, int dy);
    int check(int field[fieldSize][fieldSize], int x, int y, int dx, int dy);
public:
    int field[fieldSize][fieldSize]; // '0' - пусто, '1' - крестик, '2' - нолик
    int trialField[fieldSize][fieldSize]; // Экспериментальное поле для симуляций
    int lastX = 0;
    int lastY = 0;
    int update(int x, int y);
    int update(int field[fieldSize][fieldSize], int x, int y);
    int randomSimulation(int field[fieldSize][fieldSize], int currPlayer);
    void clearField() {
        for (int i = 0; i < fieldSize; ++i) {
            for (int j = 0; j < fieldSize; ++j) {
                field[i][j] = 0;
            }
        }
    }

    bool isDraw();
};

#endif