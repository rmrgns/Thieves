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
//        // 경로를 찾을 수 없는 경우
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
//        // 두 노드 사이의 이동 비용 계산 (예: 맨해튼 거리, 유클리드 거
//
//        int dx = std::abs(nodeA->x - nodeB->x);
//        int dy = std::abs(nodeA->y - nodeB->y);
//        return dx + dy;
//    }
//
//    int calculateHeuristic(Node* node, Node* goalNode) {
//        // 현재 노드에서 목적지까지의 예상 비용 계산 (휴리스틱 함수)
//        return calculateCost(node, goalNode);
//    }
//
//    std::vector<Node*> getNeighbors(Node* node) {
//        // 현재 노드의 인접한 노드들을 가져옴 (예: 상, 하, 좌, 우)
//        std::vector<Node*> neighbors;
//
//        int numRows = grid.size();
//        int numCols = grid[0].size();
//
//        // 상
//        if (node->y > 0) {
//            neighbors.push_back(&grid[node->y - 1][node->x]);
//        }
//        // 하
//        if (node->y < numRows - 1) {
//            neighbors.push_back(&grid[node->y + 1][node->x]);
//        }
//        // 좌
//        if (node->x > 0) {
//            neighbors.push_back(&grid[node->y][node->x - 1]);
//        }
//        // 우
//        if (node->x < numCols - 1) {
//            neighbors.push_back(&grid[node->y][node->x + 1]);
//        }
//
//        return neighbors;
//    }
//
//    std::vector<Node*> reconstructPath(Node* node) {
//        // 경로 추적하여 최단 경로 반환
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
//    // 그래프 생성 (노드들의 2D 그리드)
//    std::vector<std::vector<Node>> grid;
//
//    // 그래프 초기화 (노드 생성, 장애물 설정 등)
//
//    // A* 객체 생성
//    AStar astar(grid);
//
//    // 출발 노드와 목적지 노드 설정
//    Node* startNode = &grid[startY][startX];
//    Node* goalNode = &grid[goalY][goalX];
//
//    // 경로 탐색
//    std::vector<Node*> path = astar.findPath(startNode, goalNode);
//
//    // 최단 경로 출력 또는 처리
//
//    return 0;
//}