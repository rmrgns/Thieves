#include "pch.h"
#include "recast_astar.h"




bool NavMeshPathfinding::LoadNavMesh(const char* navMeshFilePath) {
    // std::ifstream을 사용하여 파일 열기
    std::ifstream file(navMeshFilePath, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Failed to open NavMesh file" << std::endl;
        return false;
    }

    // 파일 크기 구하기
    file.seekg(0, std::ios::end);
    navDataSize = static_cast<long>(file.tellg());
    file.seekg(0, std::ios::beg);

    // 메모리 할당 및 파일 내용 읽기
    navData = (unsigned char*)dtAlloc(navDataSize, DT_ALLOC_PERM);
    if (!navData) {
        std::cout << "Failed to allocate memory for NavMesh data" << std::endl;
        return false;
    }
    file.read(reinterpret_cast<char*>(navData), navDataSize);

    // 파일 닫기
    file.close();

    navMesh = dtAllocNavMesh();
    if (!navMesh) {
        std::cout << "Failed to allocate DetourNavMesh" << std::endl;
        return false;
    }

    dtStatus status = navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
    if (dtStatusFailed(status)) {
        std::cout << "Failed to initialize DetourNavMesh" << std::endl;
        return false;
    }

    navQuery = dtAllocNavMeshQuery();
    if (!navQuery) {
        std::cout << "Failed to allocate DetourNavMeshQuery" << std::endl;
        return false;
    }

    status = navQuery->init(navMesh, 2048);
    if (dtStatusFailed(status)) {
        std::cout << "Failed to initialize DetourNavMeshQuery" << std::endl;
        return false;
    }

    return true;
}
Vector3 NavMeshPathfinding::FindAndPrintPath(const float startPos[3], const float endPos[3]) {
    // 시작점과 끝점 설정
        // 시작점과 끝점에서 가장 가까운 네비게이션 메시 지점 찾기
    dtQueryFilter filter;
    dtPolyRef startRef, endRef;
    Vector3 pathPos = {0.f,0.f,0.f};
    float startNearest[3], endNearest[3];
    const float halfExtents[3] = { 25.0f, 75.0f, 25.0f };
    navQuery->findNearestPoly(startPos, halfExtents, &filter, &startRef, startNearest);
    navQuery->findNearestPoly(endPos, halfExtents, &filter, &endRef, endNearest);

    // 길 찾기 실행
    const int MAX_POLYS = 256;
    dtPolyRef path[MAX_POLYS];
    int pathCount;
    navQuery->findPath(startRef, endRef, startNearest, endNearest, &filter, path, &pathCount, MAX_POLYS);

    // 결과 출력
    if (pathCount > 0) {
        std::cout << "Path found!" << std::endl;
        for (int i = 0; i < pathCount; ++i) {
            dtPolyRef polyRef = path[i];
            const dtMeshTile* tile;
            const dtPoly* poly;
            navMesh->getTileAndPolyByRef(polyRef, &tile, &poly);

            float pt[3] = { 0.0f, 0.0f, 0.0f };
            for (int j = 0; j < poly->vertCount; ++j) {
                const float* v = &tile->verts[poly->verts[j] * 3];
                pt[0] += v[0];
                pt[1] += v[1];
                pt[2] += v[2];
            }

            const float invVertCount = 1.0f / poly->vertCount;
            pt[0] *= invVertCount;
            pt[1] *= invVertCount;
            pt[2] *= invVertCount;

            std::cout << "Point " << i << ": (" << pt[0] << ", " << pt[1] << ", " << pt[2] << ")" << std::endl;
            
            
            return pathPos;
        }
    }
    else {
            std::cout << "No path found!" << std::endl;
    }

    return pathPos;
}
//void NavMeshPathfinding::FindAndPrintPath() {
//    // ... The rest of the code for finding and printing the path ...
//    // (Please copy the relevant code for finding and printing the path from your original main function to this function)
//    // Remember to update the variables to use class members, e.g., 'navMesh' instead of 'navMesh', 'navQuery' instead of 'navQuery', etc.
//}