/#pragma once
//struct Node {
//    int x, y;          // 노드의 좌표
//    bool obstacle;     // 장애물 여부
//    int g;             // 출발 노드로부터의 비용
//    int h;             // 목적지까지의 예상 비용
//    int f;             // 총 비용 (g + h)
//    Node* parent;      // 이전 노드의 포인터 (경로 추적을 위해 사용)
//
//    Node(int x, int y) : x(x), y(y), obstacle(false), g(0), h(0), f(0), parent(nullptr) {}
//
//    // 비용 갱신
//    void updateCost(int newG, int newH) {
//        g = newG;
//        h = newH;
//        f = g + h;
//    }
//};
