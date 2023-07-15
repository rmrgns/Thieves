//#pragma once
//#include <iostream>
//#include <vector>
//#include <queue>
//#include "CNode.h"
//class AStar {
//public:
//    AStar(std::vector<std::vector<Node>>& grid) : grid(grid) {}
//
//    std::vector<Node*> findPath(Node* startNode, Node* goalNode) {
//        std::priority_queue<Node*, std::vector<Node*>, CompareNodes> openList;
//        std::vector<Node*> closedList;
//
//        startNode->updateCost(0, calculateHeuristic(startNode, goalNode));
//        openList.push(startNode);
//
//        while (!openList.empty()) {
//            Node* currentNode = openList.top();
//            openList.pop();
//
//            if (currentNode == goalNode) {
//                return reconstructPath(currentNode);
//            }
//
//            closedList.push_back(currentNode);
//
//            for (Node* neighbor : getNeighbors(currentNode)) {
//                if (std::find(closedList.begin(), closedList.end(), neighbor) != closedList.end() || neighbor->obstacle) {
//                    continue;
//                }
//
//                int newG = currentNode->g + calculateCost(currentNode, neighbor);
//                int newH = calculateHeuristic(neighbor, goalNode);
//                int newF = newG + newH;
//
//                if (newF < neighbor->f || std::find(openList.begin(), openList.end(), neighbor) == openList.end()) {
//                    neighbor->updateCost(newG, newH);
//                    neighbor->parent = currentNode;
//
//                    if (std::find(openList.begin(), openList.end(), neighbor) == openList.end()) {
//                        openList.push(neighbor);
//                    }
//                }
//            }
//        }
//
//        // ��θ� ã�� �� ���� ���
//        return std::vector<Node*>();
//    }
//
//private:
//    std::vector<std::vector<Node>>& grid;
//
//    struct CompareNodes {
//        bool operator()(const Node* a, const Node* b) {
//            return a->f > b->f;
//        }
//    };
//
//    int calculateCost(Node* nodeA, Node* nodeB) {
//        // �� ��� ������ �̵� ��� ��� (��: ����ư �Ÿ�, ��Ŭ���� ��
//
//        int dx = std::abs(nodeA->x - nodeB->x);
//        int dy = std::abs(nodeA->y - nodeB->y);
//        return dx + dy;
//    }
//
//    int calculateHeuristic(Node* node, Node* goalNode) {
//        // ���� ��忡�� ������������ ���� ��� ��� (�޸���ƽ �Լ�)
//        return calculateCost(node, goalNode);
//    }
//
//    std::vector<Node*> getNeighbors(Node* node) {
//        // ���� ����� ������ ������ ������ (��: ��, ��, ��, ��)
//        std::vector<Node*> neighbors;
//
//        int numRows = grid.size();
//        int numCols = grid[0].size();
//
//        // ��
//        if (node->y > 0) {
//            neighbors.push_back(&grid[node->y - 1][node->x]);
//        }
//        // ��
//        if (node->y < numRows - 1) {
//            neighbors.push_back(&grid[node->y + 1][node->x]);
//        }
//        // ��
//        if (node->x > 0) {
//            neighbors.push_back(&grid[node->y][node->x - 1]);
//        }
//        // ��
//        if (node->x < numCols - 1) {
//            neighbors.push_back(&grid[node->y][node->x + 1]);
//        }
//
//        return neighbors;
//    }
//
//    std::vector<Node*> reconstructPath(Node* node) {
//        // ��� �����Ͽ� �ִ� ��� ��ȯ
//        std::vector<Node*> path;
//        while (node != nullptr) {
//            path.push_back(node);
//            node = node->parent;
//        }
//        std::reverse(path.begin(), path.end());
//        return path;
//    }
//};
//
//int main() {
//    // �׷��� ���� (������ 2D �׸���)
//    std::vector<std::vector<Node>> grid;
//
//    // �׷��� �ʱ�ȭ (��� ����, ��ֹ� ���� ��)
//
//    // A* ��ü ����
//    AStar astar(grid);
//
//    // ��� ���� ������ ��� ����
//    Node* startNode = &grid[startY][startX];
//    Node* goalNode = &grid[goalY][goalX];
//
//    // ��� Ž��
//    std::vector<Node*> path = astar.findPath(startNode, goalNode);
//
//    // �ִ� ��� ��� �Ǵ� ó��
//
//    return 0;
//}