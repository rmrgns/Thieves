#include "pch.h"
#include<algorithm>
#include "collision_checker.h"

bool CollisionChecker::CheckCollisions(BoxCollision& a, BoxCollision& b)
{
    return (a.GetMinPos().x <= b.GetMaxPos().x && a.GetMaxPos().x >= b.GetMinPos().x) &&
        //(a.GetMinPos().y <= b.GetMaxPos().y && a.GetMaxPos().y >= b.GetMinPos().y) &&
        (a.GetMinPos().z <= b.GetMaxPos().z && a.GetMaxPos().z >= b.GetMinPos().z);
}

bool CollisionChecker::CheckInRange(float x, float z, const Vector3& min_pos, const Vector3& max_pos)
{
    return (min_pos.x <= x && x <= max_pos.x && min_pos.z <= z && z <= max_pos.z);
}

bool CollisionChecker::CheckCollisions(SphereCollison& a, BoxCollision& b)
{
    float x = std::fmaxf(b.GetMinPos().x, fminf(a.GetCenterPos().x, b.GetCenterPos().x));
    float y = std::fmaxf(b.GetMinPos().y, fminf(a.GetCenterPos().y, b.GetCenterPos().y));
    float z = std::fmaxf(b.GetMinPos().z, fminf(a.GetCenterPos().z, b.GetCenterPos().z));
    float distance = sqrt((x - a.GetCenterPos().x) * (x - a.GetCenterPos().x) +
        (y - a.GetCenterPos().y) * (y - a.GetCenterPos().y) +
        (z - a.GetCenterPos().z) * (z - a.GetCenterPos().z));
    return distance < a.GetRadius();
}

bool CollisionChecker::CheckInRange(BoxCollision& range_obj, SphereCollison& move_obj)
{
    return false;
}

bool CollisionChecker::lineIntersection(Vector2 a, Vector2 b, Vector2 c, Vector2 d, Vector2& x)
{
    float det = (b - a).cross(d - c);
    //�μ��� ������ ��� 
    if (fabs(det) < EPSILON) return false;
    x = a + (b - a) * ((c - a).cross(d - c) / det);
    return true;
}

float CollisionChecker::ccw(Vector2 a, Vector2 b)
{
    return a.cross(b);
}

float CollisionChecker::ccw(Vector2 p, Vector2 a, Vector2 b)
{
    return ccw(a - p, b - p);
}

bool CollisionChecker::paralleSegments(Vector2 a, Vector2 b, Vector2 c, Vector2 d, Vector2& p)
{
    if (b < a) std::swap(a, b); if (d < c) std::swap(c, d);
    //�� �������� ���ų� �� ������ ��ġ�� �ʴ� ��츦 �켱 �ɷ�����. ������ 1�� ������ ����̴�. 
    if (ccw(a, b, c) != 0 || b < c || d < a)
        return false;
    //�� ������ Ȯ���� ��ģ�ٸ� ������ �ϳ��� ã�´�. 
    a < c ? p = c : p = a;

    return true;


}

bool CollisionChecker::inBoundingRectangle(Vector2 p, Vector2 a, Vector2 b)
{
    if (b < a) std::swap(a, b);
    return p == a || p == b || (a < p && p < b);
}

bool CollisionChecker::segmentIntersection(Vector2 a, Vector2 b, Vector2 c, Vector2 d, Vector2& p)
{
    if (!lineIntersection(a, b, c, d, p))
        return paralleSegments(a, b, c, d, p);
    //p�� �� ���п� ���ԵǾ� �ִ� ��쿡�� ���� ��ȯ�Ѵ�. 
    return inBoundingRectangle(p, a, b) && inBoundingRectangle(p, c, d);
}
