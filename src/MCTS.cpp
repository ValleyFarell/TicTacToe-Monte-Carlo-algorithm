#include"../include/MCTS.h"

// void DFS(int f[fieldSize][fieldSize], Node* node, int maxDepth, int depth = 0) {
//     std::cout << "----" << " moveX=" << node->moveX << " moveY=" << node->moveY << " games=" << node->games << " wins=" << node->wins << " condition=" << node->condition << " player=" << node->player << "\n";
//     f[node->moveX][node->moveY] = node->player;
//     for (int i = 0; i < fieldSize; ++i) {
//         std::cout << "----";
//         for (int j = 0; j < fieldSize; ++j) {
//             std::cout << f[i][j] << " ";
//         }
//         std::cout << "\n";
//     }
//     if (depth <= maxDepth) {
//         for (Node* z : node->children) {
//             DFS(f, z, maxDepth, depth+1);
//         }
//     }
//     f[node->moveX][node->moveY] = 0;
// }

void MCT::deleteTree(Node * node) {
    if (node->children.size() == 0) {
        delete node;
        return;
    }
    for (int i = 0; i < node->children.size(); ++i) {
        deleteTree(node->children[i]);
    }
    delete node;
}

double MCT::getUCB(Node* node) {
    if (node->games == 0) return DBL_MAX;
    return (double)node->wins / (double)node->games + \
        intelligenceParam * sqrt(log(node->parent->games) / (double)node->games);
}

Node* MCT::getBestChild(Node* node) {
    int mxInd = 0;
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

std::pair<int, int> MCT::getMove(int field[fieldSize][fieldSize], int currPlayer) {
    // Инициализация корневого узла
    std::vector<int> moves(fieldSize * fieldSize);
    for (int i = 0; i < fieldSize; ++i)
        for (int j = 0; j < fieldSize; ++j) {
            if(field[i][j] == 0) 
                moves.push_back(i * fieldSize + j);
        }
    Node * root = new Node(-1, -1, 0, 0, 0, nullptr, currPlayer);
    int currField[fieldSize][fieldSize];

    // Основной блок алгоритма
    for (int i = 0; i < simulationsCount; ++i) {
        // Копируем поле корневого состояния в поле-расходник
        memcpy(&currField[0][0], &field[0][0], fieldSize * fieldSize * sizeof(int));

        // Выбор листа
        Node* leaf = leafSelection(currField, root);

        if (leaf->condition == 0) {
            // Расширение листа
            Node* newLeaf = expansion(currField, leaf);

            // Случайная симуляция
            int simRes = mech->randomSimulation(currField, 3-leaf->player);
            
            // Обновление статистики
            backPropogation(newLeaf, simRes);
        } else {
            if (leaf->condition == leaf->player) leaf->wins = 1e9;
            if (leaf->condition == (3 - leaf->player)) leaf->wins = 1e9;
            if (leaf->condition == 3) {
                leaf->wins = 1e9 / 2;
                leaf->games = 1e9;
            }
            backPropogation(leaf, leaf->condition);
        }
    }
    // DFS(field, root, 2);

    int moveInd = 0;
    std::cout << "Stat:\n";
    for (int i = 0; i < root->children.size(); ++i) {
        if (root->children[i]->games > root->children[moveInd]->games) {
            moveInd = i;
        }
        std::cout << "    x=" << root->children[i]->moveX << " y=" << root->children[i]->moveY << " w=" << root->children[i]->wins << " g=" << root->children[i]->games << " c=" << root->children[i]->condition << " p=" << root->children[i]->player << "\n";
    }
    std::cout << "\n";
    std::pair<int, int> move = {root->children[moveInd]->moveX, root->children[moveInd]->moveY};
    deleteTree(root);
    return move;
}

Node* MCT::leafSelection(int field[fieldSize][fieldSize], Node* node) {
    while(node->children.size() != 0) {
        node = getBestChild(node);
        field[node->moveX][node->moveY] = node->player;
    }

    node->condition = mech->update(field, node->moveX, node->moveY);

    return node;
}

Node * MCT::expansion(int field[fieldSize][fieldSize], Node * node) {
    // Подсчёт допустимых ходов 
    for (int i = 0; i < fieldSize; ++i) {
        for (int j = 0; j < fieldSize; ++j) {
            if (!field[i][j]) {
                Node * child = new Node(i, j, 0, 0, 0, node, 3 - node->player);
                node->children.push_back(child);
            }
        }
    }
    if (node->children.size() == 0) return node;
    int childInd = rand() % node->children.size();
    field[node->children[childInd]->moveX][node->children[childInd]->moveY];
    return node->children[childInd];
}

void MCT::backPropogation(Node* leaf, int result) {
    while(leaf->parent) {
        if (leaf->player == result) leaf->wins++;
        leaf->games++;
        leaf = leaf->parent;
    }
    if (leaf->player == result) leaf->wins++;
    leaf->games++;
}