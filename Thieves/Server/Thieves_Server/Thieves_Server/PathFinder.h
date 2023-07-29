#pragma once
#include "Detour/Include/DetourNavMesh.h"
#include "Detour/Include/DetourNavMeshQuery.h"

class PathFinder {
public:
    PathFinder();
    ~PathFinder();

    bool findPath(float startPos[3], float endPos[3], float halfExtents[3]);

private:
    dtNavMeshQuery* navQuery;
};

