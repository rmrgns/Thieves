#pragma once

#include "Detour/Include/DetourNavMesh.h"
#include "Detour/Include/DetourNavMeshQuery.h"

class NavMeshLoader {
public:
    NavMeshLoader();
    ~NavMeshLoader();

    bool loadNavMesh(const char* navMeshFilePath);
    bool findPath(int enemy_id, float startPos[3], float endPos[3], float halfExtents[3]);
    

private:
    dtNavMesh* navMesh;
    dtNavMeshQuery* navQuery;
};

struct NavMeshSetHeader
{
    int magic;
    int version;
    int numTiles;
    dtNavMeshParams params;
};
struct NavMeshTileHeader
{
    dtTileRef tileRef;
    int dataSize;
};