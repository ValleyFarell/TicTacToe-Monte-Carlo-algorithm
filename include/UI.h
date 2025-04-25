#include<SFML/Graphics.hpp>
#include<vector>
#include<cmath>
#include<iostream>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include"MCTS.h"
#include"Mechanics.h"

#ifndef UI
#define UI

class TicTacToeUI {
public:
    TicTacToeUI();
    void run();
private:
    const int windowSize = 1000;
    
    int gridWidth;

    int toeRadius;
    int outline;
    const double goldenRatio = 1.618;
    const double outlinePercents = 0.25;
    const int widthK = 2; // Коэффициент ширины ноликов

    int crossWidth;
    int crossHeight;
    const int crossAngel = 45;

    void init();
    void drawField();
    void handleInput(sf::Event event);

    void toeDraw(int i, int j);
    void crossDraw(int i, int j);
    void cellDraw(int i, int j);
    
    int textRectWidth = 300;
    int textRectHeight = 150;

    sf::RenderWindow window;
    int currentPlayer;
    int cellSize;
    int gameWinner;

    bool unpressedSpace;
    bool unpressedL;
    
    Mechanics mech;
    MCT bot;
};

#endif
