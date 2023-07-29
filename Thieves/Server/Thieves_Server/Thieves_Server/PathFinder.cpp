#include "PathFinder.h"
#include <iostream>
#include <cstring>
#include <fstream>

PathFinder::PathFinder() {
    navQuery = dtAllocNavMeshQuery();
}

PathFinder::~PathFinder() {
    dtFreeNavMeshQuery(navQuery);
}

bool PathFinder::findPath(float startPos[3], float endPos[3], float halfExtents[3]) {
    dtQueryFilter filter;
    dtPolyRef startRef, endRef;
    float startNearest[3], endNearest[3];

    // �������� ������ ���� ����� �ٰ����� ������ ã��
    navQuery->findNearestPoly(startPos, halfExtents, &filter, &startRef, startNearest);
    navQuery->findNearestPoly(endPos, halfExtents, &filter, &endRef, endNearest);

    // �ִ� �ٰ��� ����
    const int MAX_POLYS = 256;
    dtPolyRef path[MAX_POLYS];
    int pathCount;

    // �� ã�� �˰��� ����
    navQuery->findPath(startRef, endRef, startNearest, endNearest, &filter, path, &pathCount, MAX_POLYS);

    if (pathCount > 0) {
        std::cout << "��θ� ã�ҽ��ϴ�!" << std::endl;
        for (int i = 0; i < pathCount; ++i) {
            dtPolyRef polyRef = path[i];
            const dtMeshTile* tile;
            const dtPoly* poly;
            navQuery->getAttachedNavMesh()->getTileAndPolyByRefUnsafe(polyRef, &tile, &poly);

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
        std::cout << "��θ� ã�� ���߽��ϴ�!" << std::endl;
        return false;
    }
}