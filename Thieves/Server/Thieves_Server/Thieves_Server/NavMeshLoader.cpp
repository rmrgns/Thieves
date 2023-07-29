#include "NavMeshLoader.h"
#include <iostream>
#include <cstring>
#include <fstream>

NavMeshLoader::NavMeshLoader() {
    navMesh = dtAllocNavMesh();
    navQuery = dtAllocNavMeshQuery();
}

NavMeshLoader::~NavMeshLoader() {
    dtFreeNavMesh(navMesh);
    dtFreeNavMeshQuery(navQuery);
}

bool NavMeshLoader::loadNavMesh(const char* navMeshFilePath) {
    FILE* fp;
    if (fopen_s(&fp, navMeshFilePath, "rb") != 0) {
        std::cout << "네비게이션 메시 파일 열기 실패" << std::endl;
        return false;
    }
    if (!fp) {
        std::cout << "네비게이션 메시 파일 열기 실패" << std::endl;
        return false;
    }

    NavMeshSetHeader header;
    if (fread(&header, sizeof(NavMeshSetHeader), 1, fp) != 1) {
        std::cout << "NavMeshSetHeader 읽기 실패" << std::endl;
        fclose(fp);
        return false;
    }

    dtStatus status = navMesh->init(&header.params);
    if (dtStatusFailed(status)) {
        std::cout << "DetourNavMesh 초기화 실패" << std::endl;
        fclose(fp);
        return false;
    }

    for (int i = 0; i < header.numTiles; ++i) {
        NavMeshTileHeader tileHeader;
        if (fread(&tileHeader, sizeof(NavMeshTileHeader), 1, fp) != 1) {
            std::cout << "NavMeshTileHeader 읽기 실패" << std::endl;
            fclose(fp);
            return false;
        }

        unsigned char* tileData = (unsigned char*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
        if (!tileData) {
            std::cout << "네비게이션 메시 타일 데이터 메모리 할당 실패" << std::endl;
            fclose(fp);
            return false;
        }

        if (fread(tileData, tileHeader.dataSize, 1, fp) != 1) {
            std::cout << "네비게이션 메시 타일 데이터 읽기 실패" << std::endl;
            dtFree(tileData);
            fclose(fp);
            return false;
        }

        status = navMesh->addTile(tileData, tileHeader.dataSize, DT_TILE_FREE_DATA, 0, 0);
        if (dtStatusFailed(status)) {
            std::cout << "네비게이션 메시 타일 추가 실패" << std::endl;
            dtFree(tileData);
            fclose(fp);
            return false;
        }
    }

    fclose(fp);
    return true;
}


bool NavMeshLoader::findPath(float startPos[3], float endPos[3], float halfExtents[3]) {
    dtQueryFilter filter;
    dtPolyRef startRef, endRef;
    float startNearest[3], endNearest[3];

    navQuery->findNearestPoly(startPos, halfExtents, &filter, &startRef, startNearest);
    navQuery->findNearestPoly(endPos, halfExtents, &filter, &endRef, endNearest);

    const int MAX_POLYS = 256;
    dtPolyRef path[MAX_POLYS];
    int pathCount;
    navQuery->findPath(startRef, endRef, startNearest, endNearest, &filter, path, &pathCount, MAX_POLYS);

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
        }

        return true;
    }
    else {
        std::cout << "경로를 찾지 못했습니다!" << std::endl;
        return false;
    }
}