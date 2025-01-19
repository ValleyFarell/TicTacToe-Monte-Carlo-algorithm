#include<vector>
#include<cmath>
#include<iostream>
#include<float.h>
#include<limits.h>
#include"Mechanics.h"

#ifndef MONTE_CARLO_TREE_SEARCH
#define MONTE_CARLO_TREE_SEARCH

struct Node {
    int moveX;
    int moveY;
    int games;
    int wins;
    int condition; // 0 - не финальное состояние, 1 - выиграл крестик, 2 - выиграл нолик, 3 - ничья
    Node * parent; // Узел, из которого пришли
    std::vector<Node*> children; // Потенциальные ходы
    int player;

    Node(int moveX, int moveY, int games, int wins, int condition, Node* parent, int player) : moveX(moveX), moveY(moveY),
    games(games), wins(wins), condition(condition), parent(parent), player(player) {}
};

// void DFS(int f[fieldSize][fieldSize], Node* node, int maxDepth, int depth = 0);

class MCT {
private:
    const double intelligenceParam = 0.5;
    const int simulationsCount = 100000;

    // Вычисление лучшего ребёнка
    Node* getBestChild(Node* node);

    // Вычисление перспективности
    double getUCB(Node* node);

    Mechanics* mech;
    
    // Выбор
    Node * leafSelection(int field[fieldSize][fieldSize], Node* node);

    // Расширение листа. Возвращает один из созданных детей узла
    Node * expansion(int field[fieldSize][fieldSize], Node * node);

    // Обновление статистики согласно результатам симуляции
    void backPropogation(Node* leaf, int result);

    void deleteTree(Node * node);
public:
    std::pair<int, int> getMove(int field[fieldSize][fieldSize], int currPlayer);
    MCT(Mechanics * mechanics) : mech(mechanics) {}
};

#endif