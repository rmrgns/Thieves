#pragma once
#include "define.h"

#include "Recast/Include/Recast.h"
#include "Detour/Include/DetourNavMesh.h"
#include "Detour/Include/DetourNavMeshBuilder.h"
#include "Detour/Include/DetourNavMeshQuery.h"
#include <cstring>
#include <cstdio>
#include <fstream>
#include <vector>

class NavMeshPathfinding {
public:
    NavMeshPathfinding() {};
    ~NavMeshPathfinding() {
        if (navMesh)
            dtFreeNavMesh(navMesh);
        if (navQuery)
            dtFreeNavMeshQuery(navQuery);
        if (navData)
            dtFree(navData);
    }

    bool LoadNavMesh(const char* navMeshFilePath);
    Vector3 FindAndPrintPath(const float start[3], const float end[3]);
private:
    dtNavMesh* navMesh;
    dtNavMeshQuery* navQuery;
    unsigned char* navData;
    long navDataSize;
};


//int main() {
//    // NavMesh 파일 경로
//    const char* navMeshFilePath = ".\\navmesh.bin";
//
//    // NavMesh 데이터를 DetourNavMesh 객체로 로드
//    dtNavMesh* navMesh = dtAllocNavMesh();
//    if (!navMesh) {
//        std::cout << "Failed to allocate DetourNavMesh" << std::endl;
//        return -1;
//    }
//
//    FILE* fp = fopen(navMeshFilePath, "rb");
//    if (!fp) {
//        std::cout << "Failed to open NavMesh file" << std::endl;
//        return -1;
//    }
//
//    fseek(fp, 0, SEEK_END);
//    long navDataSize = ftell(fp);
//    fseek(fp, 0, SEEK_SET);
//
//    unsigned char* navData = (unsigned char*)dtAlloc(navDataSize, DT_ALLOC_PERM);
//    if (!navData) {
//        std::cout << "Failed to allocate memory for NavMesh data" << std::endl;
//        return -1;
//    }
//
//    if (fread(navData, sizeof(unsigned char), navDataSize, fp) != navDataSize) {
//        std::cout << "Failed to read NavMesh data from file" << std::endl;
//        return -1;
//    }
//
//    fclose(fp);
//
//    dtStatus status = navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
//    if (dtStatusFailed(status)) {
//        std::cout << "Failed to initialize DetourNavMesh" << std::endl;
//        return -1;
//    }
//
//    // 길 찾기 알고리즘을 위한 객체 생성
//    dtNavMeshQuery* navQuery = dtAllocNavMeshQuery();
//    if (!navQuery) {
//        std::cout << "Failed to allocate DetourNavMeshQuery" << std::endl;
//        return -1;
//    }
//
//    status = navQuery->init(navMesh, 2048);
//    if (dtStatusFailed(status)) {
//        std::cout << "Failed to initialize DetourNavMeshQuery" << std::endl;
//        return -1;
//    }
//
//    // 시작점과 끝점 설정
//    float startPos[3] = { -9.0f, 0.0f, -9.0f };
//    float endPos[3] = { 9.0f, 0.0f, 9.0f };
//
//    // 시작점과 끝점에서 가장 가까운 네비게이션 메시 지점 찾기
//    dtQueryFilter filter;
//    dtPolyRef startRef, endRef;
//    float startNearest[3], endNearest[3];
//    const float halfExtents[3] = { 2.0f, 2.0f, 2.0f };
//    navQuery->findNearestPoly(startPos, halfExtents, &filter, &startRef, startNearest);
//    navQuery->findNearestPoly(endPos, halfExtents, &filter, &endRef, endNearest);
//   
//    // 길 찾기 실행
//    const int MAX_POLYS = 256;
//    dtPolyRef path[MAX_POLYS];
//    int pathCount;
//    navQuery->findPath(startRef, endRef, startNearest, endNearest, &filter, path, &pathCount, MAX_POLYS);
//
//    // 결과 출력
//    if (pathCount > 0) {
//        std::cout << "Path found!" << std::endl;
//        for (int i = 0; i < pathCount; ++i) {
//            dtPolyRef polyRef = path[i];
//            const dtMeshTile* tile;
//            const dtPoly* poly;
//            navMesh->getTileAndPolyByRef(polyRef, &tile, &poly);
//
//            float pt[3] = { 0.0f, 0.0f, 0.0f };
//            for (int j = 0; j < poly->vertCount; ++j) {
//                const float* v = &tile->verts[poly->verts[j] * 3];
//                pt[0] += v[0];
//                pt[1] += v[1];
//                pt[2] += v[2];
//            }
//
//            const float invVertCount = 1.0f / poly->vertCount;
//            pt[0] *= invVertCount;
//            pt[1] *= invVertCount;
//            pt[2] *= invVertCount;
//
//            std::cout << "Point " << i << ": (" << pt[0] << ", " << pt[1] << ", " << pt[2] << ")" << std::endl;
//        }
//    }
//    else {
//        std::cout << "No path found!" << std::endl;
//    }
//
//    // 메모리 해제
//    dtFreeNavMesh(navMesh);
//    dtFreeNavMeshQuery(navQuery);
//    dtFree(navData);
//
//    return 0;
//}