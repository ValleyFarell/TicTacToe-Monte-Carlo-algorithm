#include<vector>
#include<cmath>
#include<iostream>
#include<float.h>

#ifndef MONTE_CARLO_TREE_SEARCH
#define MONTE_CARLO_TREE_SEARCH

struct Node {
    std::pair<int, int> move;
    std::vector<std::vector<char>> field;
    int simulationsCount;
    int victoriesCount;
    char player;
    Node *parent;
    std::vector<Node*> children;
    Node(int sCount, int vCount, char player, \
         std::vector<std::vector<char>> field, std::pair<int, int> move, \
         Node *parent) : simulationsCount(sCount), victoriesCount(vCount), 
         player(player), parent(parent), field(field), move(move) {};
};

class MCT {
private:
    const double intelligenceParam = sqrt(2);
    const int simulationsCount = 15;

    const int fieldSize = 3;
    const int targetSize = 3;

    // Вычисление лучшего кандидата
    Node* getBestChild(Node* node);

    // Вычисление перспективности данного узла
    double getUCB(Node* node);

    // Получение допустимых ходов
    std::vector<std::pair<int, int>> getAllowedMoves(const std::vector<std::vector<char>>& field);
    
    Node* leafSelection(Node* node);

    char simulation(Node* node);

    void backPropogation(Node* leaf, char result, bool isUselessCon);

    void branchDelete(Node* leaf, char result);

    int checkDiagWinner(const std::vector<std::vector<char>>& field, int target, int x, int y);
    int checkSideDiagWinner(const std::vector<std::vector<char>>& field, int target, int x, int y);
    int checkVertWinner(const std::vector<std::vector<char>>& field, int target, int x, int y);
    int checkGorWinner(const std::vector<std::vector<char>>& field, int target, int x, int y);
public:
    std::pair<int, int> getMove(std::vector<std::vector<char>>& field, char currPlayer);
    int update(const std::vector<std::vector<char>>& field, int target);
    
};

std::vector<std::pair<int, int>> MCT::getAllowedMoves(const std::vector<std::vector<char>>& field) {
    std::vector<std::pair<int, int>> moves;
    for (int i = 0; i < field.size(); ++i) {
        for (int j = 0; j < field[i].size(); ++j) {
            if (field[i][j] == 0) {
                moves.push_back({i, j});
            }
        }
    }
    
    return moves;
}

double MCT::getUCB(Node* node) {
    if (node->children.size() == 0) return DBL_MAX;
    return (double)node->victoriesCount / (double)node->simulationsCount + \
        intelligenceParam * (log(node->parent->simulationsCount) / (double)node->simulationsCount);
}

Node* MCT::getBestChild(Node* node) {
    int mxInd = rand() % node->children.size();
    double mx = getUCB(node->children[mxInd]);
    for (int i = 0; i < node->children.size(); ++i) {
        double tmp = getUCB(node->children[i]);
        if (mx < tmp) {
            mx = tmp;
            mxInd = i;
        }
    }
    return node->children[mxInd];
}

std::pair<int, int> MCT::getMove(std::vector<std::vector<char>>& field, char currPlayer) {
    Node * root = new Node(0, 0, 3 - currPlayer, field, {-1, -1}, nullptr);
    std::vector<std::pair<int, int>> allowedMoves = getAllowedMoves(field);

    for (int i = 0; i < allowedMoves.size(); ++i) {
        Node * child = new Node(0, 0, currPlayer, field, allowedMoves[i], root);
        child->field[allowedMoves[i].first][allowedMoves[i].second] = currPlayer;
        root->children.push_back(child);
    }

    for (int sim = 0; sim < simulationsCount; ++sim) {
        Node * leaf = leafSelection(root);
        char leafResult = update(leaf->field, targetSize);
        if (leafResult != -1) {
            
        } else {
            char simulationResult = simulation(leaf);
            backPropogation(leaf, simulationResult, false);
        }
    }
    return getBestChild(root)->move;
}

Node* MCT::leafSelection(Node* node) {
    while(node->children.size() != 0) {
        node = getBestChild(node);
    }
    std::vector<std::pair<int, int>> allowedMoves = getAllowedMoves(node->field);

    for (int i = 0; i < allowedMoves.size(); ++i) {
        Node * child = new Node(0, 0, 3 - node->player, node->field, allowedMoves[i], node);
        child->field[allowedMoves[i].first][allowedMoves[i].second] = node->player;
        node->children.push_back(child);
    }
    
    return node->children[rand() % node->children.size()];
}

char MCT::simulation(Node* node) {
    std::vector<std::pair<int, int>> allowedMoves = getAllowedMoves(node->field);
    std::vector<std::vector<char>> simField = node->field;

    char currPlayer = 3 - node->player;
    // Simulation
    char result = update(simField, targetSize);
    while(result == -1) {
        if (allowedMoves.size() == 0) {
            return 0;
        }
        int moveInd = rand() % allowedMoves.size();
        simField[allowedMoves[moveInd].first][allowedMoves[moveInd].second] = currPlayer;
        allowedMoves.erase(allowedMoves.begin() + moveInd);
        currPlayer = 3 - currPlayer;
        result = update(simField, targetSize);
    }
    return 3 - currPlayer;
}

void MCT::branchDelete(Node* leaf, char result) {
    Node* parent = leaf->parent;
    if (parent->children.size() == 1) {

    } else {
        
    }
}

void MCT::backPropogation(Node* leaf, char result, bool isUselessCon) {
    if (isUselessCon && result != 0) {
        if (result == leaf->player) {
            branchDelete(leaf->parent, result);
        } else {
            branchDelete(leaf, result);
        }
    } else {
        Node* currNode = leaf;
        char currPlayer = currNode->player;
        while (currNode->parent) {
            currNode->simulationsCount ++;
            currNode->victoriesCount += (currPlayer == result);
            currPlayer = 3 - currPlayer;
            currNode = currNode->parent;
        }
    }
}

int MCT::checkDiagWinner(const std::vector<std::vector<char>>& field, int target, int x, int y) {
    bool flag = true;
    if (field[y][x] == 0) return 0;

    char first = field[y][x];
    for (int i = 0; i < targetSize; ++i) {
        if (field[y + i][x + i] != first) flag = false;
    }
    if (flag) return first; 
    else return 0;
}

int MCT::checkSideDiagWinner(const std::vector<std::vector<char>>& field, int target, int x, int y) {
    bool flag = true;
    if (field[y][x] == 0) return 0;

    char first = field[y][x];
    for (int i = 0; i < targetSize; ++i) {
        if (field[y + i][x - i] != first) flag = false;
    }
    if (flag) return first;
    else return 0;
}

int MCT::checkVertWinner(const std::vector<std::vector<char>>& field, int target, int x, int y) {
    if (field[y][x] == 0) return 0;

    char first = field[y][x];
    for (int i = 0; i < targetSize; ++i) {
        if (field[y + i][x] != first) return 0;
    }
    return first;
}

int MCT::checkGorWinner(const std::vector<std::vector<char>>& field, int target, int x, int y) {
    if (field[y][x] == 0) return 0;

    char first = field[y][x];
    for (int j = 0; j < targetSize; ++j) {
        if (field[y][x + j] != first) return 0;
    }
    return first;
}

int MCT::update(const std::vector<std::vector<char>>& field, int target) {
    bool drawFlag = true;


    for (int y = 0; y < field.size(); ++y) {
        for (int x = 0; x < field.size(); ++x) {
            if (!field[y][x]) drawFlag = false;
            int winner = (x <= field.size() - target && y  <= field.size() - target) ? checkDiagWinner(field, target, x, y): 0;
            if (winner) {
                return winner;
            }

            winner = (x >= target && y  <= field.size() - target) ? checkSideDiagWinner(field, target, x, y) : 0;
            if (winner) {
                return winner;
            }

            winner = (x <= field.size() - target) ? checkGorWinner(field, target, x, y) : 0;
            if (winner) {
                return winner;
            }

            winner = (y <= field.size() - target) ? checkVertWinner(field, target, x, y): 0;
            if (winner) {
                return winner;
            }
        }
    }
    if (drawFlag) return 0;
    return -1;
}

#endif