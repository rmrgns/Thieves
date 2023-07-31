#pragma once

#include "Detour/Include/DetourNavMesh.h"
#include "Detour/Include/DetourNavMeshQuery.h"

class NavMeshLoader {
public:
    NavMeshLoader();
    ~NavMeshLoader();

    bool loadNavMesh(const char* navMeshFilePath);
    bool findPath(const Vector3& startPos, const Vector3& endPos, const Vector3& halfExtents);
    

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