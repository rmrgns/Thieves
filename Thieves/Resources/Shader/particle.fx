
#ifndef _PARTICLE_FX_
#define _PARTICLE_FX_

#include "params.fx"
#include "utils.fx"

struct Particle
{
    float3  worldPos;
    float   curTime;
    float3  worldDir;
    float   lifeTime;
    int     alive;
    float3  padding;
};

StructuredBuffer<Particle> g_data : register(t9);

struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    uint id : SV_InstanceID;
};

struct VS_OUT
{
    float4 viewPos : POSITION;
    float2 uv : TEXCOORD;
    float id : ID;
};

// VS_MAIN
// g_float_0    : Start Scale
// g_float_1    : End Scale
// g_tex_0      : Particle Texture

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output = (VS_OUT)0.f;

    float3 worldPos = mul(float4(input.pos, 1.f), g_matWorld).xyz;
    worldPos += g_data[input.id].worldPos;

    output.viewPos = mul(float4(worldPos, 1.f), g_matView);
    output.uv = input.uv;
    output.id = input.id;

    return output;
}

struct GS_OUT
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD;
    uint id : SV_InstanceID;
};

[maxvertexcount(6)]
void GS_Main(point VS_OUT input[1], inout TriangleStream<GS_OUT> outputStream)
{
    GS_OUT output[4] =
    {
        (GS_OUT)0.f, (GS_OUT)0.f, (GS_OUT)0.f, (GS_OUT)0.f
    };

    VS_OUT vtx = input[0];
    uint id = (uint)vtx.id;
    if (0 == g_data[id].alive)
        return;

    float ratio = g_data[id].curTime / g_data[id].lifeTime;
    float scale = ((g_float_1 - g_float_0) * ratio + g_float_0) / 2.f;

    // View Space
    output[0].position = vtx.viewPos + float4(-scale, scale, 0.f, 0.f);
    output[1].position = vtx.viewPos + float4(scale, scale, 0.f, 0.f);
    output[2].position = vtx.viewPos + float4(scale, -scale, 0.f, 0.f);
    output[3].position = vtx.viewPos + float4(-scale, -scale, 0.f, 0.f);

    // Projection Space
    output[0].position = mul(output[0].position, g_matProjection);
    output[1].position = mul(output[1].position, g_matProjection);
    output[2].position = mul(output[2].position, g_matProjection);
    output[3].position = mul(output[3].position, g_matProjection);

    output[0].uv = float2(0.f, 0.f);
    output[1].uv = float2(1.f, 0.f);
    output[2].uv = float2(1.f, 1.f);
    output[3].uv = float2(0.f, 1.f);

    output[0].id = id;
    output[1].id = id;
    output[2].id = id;
    output[3].id = id;

    outputStream.Append(output[0]);
    outputStream.Append(output[1]);
    outputStream.Append(output[2]);
    outputStream.RestartStrip();

    outputStream.Append(output[0]);
    outputStream.Append(output[2]);
    outputStream.Append(output[3]);
    outputStream.RestartStrip();
}

float4 PS_Main(GS_OUT input) : SV_Target
{
    return g_tex_0.Sample(g_sam_0, input.uv);
}

struct ComputeShared
{
    int addCount;
    float3 padding;
};

RWStructuredBuffer<Particle> g_particle : register(u0);
RWStructuredBuffer<ComputeShared> g_shared : register(u1);

// CS_Main
// g_vec2_1 : DeltaTime / AccTime
// g_int_0  : Particle Max Count
// g_int_1  : AddCount
// g_vec4_0 : MinLifeTime / MaxLifeTime / MinSpeed / MaxSpeed
[numthreads(1024, 1, 1)]
void CS_Main(int3 threadIndex : SV_DispatchThreadID)
{
    if (threadIndex.x >= g_int_0)
        return;

    int maxCount = g_int_0;
    int addCount = g_int_1;
    //int frameNumber = g_int_2;
    float deltaTime = g_vec2_1.x;
    float accTime = g_vec2_1.y;
    float minLifeTime = g_vec4_0.x;
    float maxLifeTime = g_vec4_0.y;
    float minSpeed = g_vec4_0.z;
    float maxSpeed = g_vec4_0.w;

    float3 look = { g_vec4_1.x, g_vec4_1.y, g_vec4_1.z };

    g_shared[0].addCount = addCount;
    GroupMemoryBarrierWithGroupSync();

    int numParticlesToCreate = maxCount;
    int isInitialized = g_int_3;
    int effectNum = g_int_2;

    {
        //if (g_particle[threadIndex.x].alive == 0)
        //{
        //    while (true)
        //    {
        //        int remaining = g_shared[0].addCount;
        //        if (remaining <= 0)
        //            break;
        //        int expected = remaining;
        //        int desired = remaining - 1;
        //        int originalValue;
        //        InterlockedCompareExchange(g_shared[0].addCount, expected, desired, originalValue);
        //        if (originalValue == expected)
        //        {
        //            g_particle[threadIndex.x].alive = 1;
        //            break;
        //        }
        //    }

        //    if (g_particle[threadIndex.x].alive == 1)
        //    {
        //        float x = ((float)threadIndex.x / (float)maxCount) + accTime;
        //        float r1 = Rand(float2(x, accTime));
        //        float r2 = Rand(float2(x * accTime, accTime));
        //        float r3 = Rand(float2(x * accTime * accTime, accTime * accTime));
        //        // [0.5~1] -> [0~1]
        //        float3 noise =
        //        {
        //            2 * r1 - 1,
        //            2 * r2 - 1,
        //            2 * r3 - 1
        //        };
        //        // [0~1] -> [-1~1]
        //        float3 dir = (noise - 0.5f) * 2.f;
        //        g_particle[threadIndex.x].worldDir = normalize(dir);
        //        g_particle[threadIndex.x].worldPos = (noise.xyz - 0.5f) * 25;
        //        g_particle[threadIndex.x].lifeTime = ((maxLifeTime - minLifeTime) * noise.x) + minLifeTime;
        //        g_particle[threadIndex.x].curTime = 0.f;
        //    }
        //}
    }
    if (effectNum == 0) // 디폴트값 (변경x)
    {
        if (isInitialized == 0 && threadIndex.x < numParticlesToCreate)
        {
            float x = ((float)threadIndex.x / (float)numParticlesToCreate) + accTime;

            float r1 = Rand(float2(x, accTime));
            float r2 = Rand(float2(x * accTime, accTime));
            float r3 = Rand(float2(x * accTime * accTime, accTime * accTime));

            // [0.5~1] -> [0~1]
            float3 noise =
            {
                2 * r1 - 1,
                2 * r2 - 1,
                2 * r3 - 1
            };

            // [0~1] -> [-1~1]
            float3 dir = (noise - 0.5f) * 2.f;
            dir.y = noise.y;
            g_particle[threadIndex.x].alive = 1;
            g_particle[threadIndex.x].worldDir = normalize(dir);
            g_particle[threadIndex.x].worldPos = float3(0, 0, 0);
            g_particle[threadIndex.x].lifeTime = ((maxLifeTime - minLifeTime) * noise.x) + minLifeTime;
            g_particle[threadIndex.x].curTime = 0.f;
        }
        else
        {
            g_particle[threadIndex.x].curTime += deltaTime;
            if (g_particle[threadIndex.x].lifeTime < g_particle[threadIndex.x].curTime)
            {
                g_particle[threadIndex.x].alive = 0;
                return;
            }

            float ratio = g_particle[threadIndex.x].curTime / g_particle[threadIndex.x].lifeTime;
            float speed = (maxSpeed - minSpeed) * ratio + minSpeed;
            g_particle[threadIndex.x].worldPos += g_particle[threadIndex.x].worldDir * speed * deltaTime;
        }
    }
    else if (effectNum == 1) // 공격 파티클
    {
        if (isInitialized == 0 && threadIndex.x < numParticlesToCreate)
        {
            float x = ((float)threadIndex.x / (float)numParticlesToCreate) + accTime;

            float r1 = Rand(float2(x, accTime));
            float r2 = Rand(float2(x * accTime, accTime));
            float r3 = Rand(float2(x * accTime * accTime, accTime * accTime));

            // [0.5~1] -> [0~1]
            float3 noise =
            {
                2 * r1 - 1,
                2 * r2 - 1,
                2 * r3 - 1
            };

            // [0~1] -> [-1~1]
            float3 dir = (noise - 0.5f) * 2.f;
            // look -> [-1~1] dir -> [-1~1]
            dir = (dir + look) * 0.5f;
            dir.y *= 0.2f;
            g_particle[threadIndex.x].alive = 1;
            g_particle[threadIndex.x].worldDir = normalize(dir);

            g_particle[threadIndex.x].worldPos = float3(0, 0, 0);
            g_particle[threadIndex.x].lifeTime = ((maxLifeTime - minLifeTime) * noise.x) + minLifeTime;
            g_particle[threadIndex.x].curTime = 0.f;
        }
        else
        {
            g_particle[threadIndex.x].curTime += deltaTime;
            if (g_particle[threadIndex.x].lifeTime < g_particle[threadIndex.x].curTime)
            {
                g_particle[threadIndex.x].alive = 0;
                return;
            }

            float ratio = g_particle[threadIndex.x].curTime / g_particle[threadIndex.x].lifeTime;
            float speed = (maxSpeed - minSpeed) * ratio + minSpeed;
            g_particle[threadIndex.x].worldPos += g_particle[threadIndex.x].worldDir * speed * deltaTime;
        }
    }
    else if (effectNum == 2) // 아이템 획득 파티클
    {
        if (isInitialized == 0 && threadIndex.x < numParticlesToCreate)
        {
            float x = ((float)threadIndex.x / (float)numParticlesToCreate) + accTime;

            float r1 = Rand(float2(x, accTime));
            float r2 = Rand(float2(x * accTime, accTime));
            float r3 = Rand(float2(x * accTime * accTime, accTime * accTime));

            // [0.5~1] -> [0~1]
            float3 noise =
            {
                2 * r1 - 1,
                2 * r2 - 1,
                2 * r3 - 1
            };

            // [0~1] -> [-1~1]
            float3 dir = (noise - 0.5f) * 2.f;
            float3 pos = (noise - 0.5f) * 25.f;
            float range = 25.f;
            dir.y = noise.y + 0.2f;
            pos.y = 0.f;
            //if (abs(dir.x) > abs(dir.z))
            //{
            //    if (dir.x > 0.f)
            //    {
            //        //pos.x = 0.5f * range;
            //        //pos.z = (noise.z - 0.5f) * range;
            //        dir.x *= 0.2f;
            //        dir.z *= 0.2f;
            //    }
            //    else
            //    {
            //        //pos.x = -0.5f * range;
            //        //pos.z = (noise.z - 0.5f) * range;
            //        dir.x *= 0.2f;
            //        dir.z *= 0.2f;
            //    }
            //}
            //else
            //{
            //    if (dir.z > 0.f)
            //    {
            //        //pos.x = (noise.x - 0.5f) * range;
            //        //pos.z = 0.5f * range;
            //        dir.x *= 0.2f;
            //        dir.z *= 0.2f;
            //    }
            //    else
            //    {
            //        //pos.x = (noise.x - 0.5f) * range;
            //        //pos.z = -0.5f * range;
            //        
            //    }
            //}
            dir.x *= 0.2f;
            dir.z *= 0.2f;
            g_particle[threadIndex.x].alive = 1;

            g_particle[threadIndex.x].worldDir = normalize(dir);
            g_particle[threadIndex.x].worldPos = float3(0.f, 0.f, 0.f);

            g_particle[threadIndex.x].lifeTime = ((maxLifeTime - minLifeTime) * noise.x) + minLifeTime;
            g_particle[threadIndex.x].curTime = 0.f;
        }
        else
        {
            g_particle[threadIndex.x].curTime += deltaTime;
            if (g_particle[threadIndex.x].lifeTime < g_particle[threadIndex.x].curTime)
            {
                g_particle[threadIndex.x].alive = 0;
                return;
            }

            float ratio = g_particle[threadIndex.x].curTime / g_particle[threadIndex.x].lifeTime;
            float speed = (maxSpeed - minSpeed) * ratio + minSpeed;

            if (g_particle[threadIndex.x].lifeTime < g_particle[threadIndex.x].curTime * 2.f)
            {
                g_particle[threadIndex.x].worldDir.y -= ratio;
            }
            else
            {
                g_particle[threadIndex.x].worldDir.y -= ratio / 2.f;
            }




            g_particle[threadIndex.x].worldPos += g_particle[threadIndex.x].worldDir * speed * deltaTime;
        }
    }
    else if (effectNum == 3) // 총 발사 파티클
    {
        if (isInitialized == 0 && threadIndex.x < numParticlesToCreate)
        {
            float x = ((float)threadIndex.x / (float)numParticlesToCreate) + accTime;

            float r1 = Rand(float2(x, accTime));
            float r2 = Rand(float2(x * accTime, accTime));
            float r3 = Rand(float2(x * accTime * accTime, accTime * accTime));

            // [0.5~1] -> [0~1]
            float3 noise =
            {
                r1 * 2 - 1.5f,
                r2 * 2 - 1.5f,
                r3 * 2 - 1.5f
            };


            look = normalize(look);
            float Angle = 25.f * 3.141592 / 180.f;

            float3 dir = normalize(look + Angle * noise);

            g_particle[threadIndex.x].alive = 1;
            g_particle[threadIndex.x].worldDir = normalize(dir);

            g_particle[threadIndex.x].worldPos = float3(0, 0, 0);
            g_particle[threadIndex.x].lifeTime = ((maxLifeTime - minLifeTime) * noise.x) + minLifeTime;
            g_particle[threadIndex.x].curTime = 0.f;
        }
        else
        {
            g_particle[threadIndex.x].curTime += deltaTime;
            if (g_particle[threadIndex.x].lifeTime < g_particle[threadIndex.x].curTime)
            {
                g_particle[threadIndex.x].alive = 0;
                return;
            }

            float ratio = g_particle[threadIndex.x].curTime / g_particle[threadIndex.x].lifeTime;
            float speed = (maxSpeed - minSpeed) * ratio + minSpeed;
            g_particle[threadIndex.x].worldPos += g_particle[threadIndex.x].worldDir * speed * deltaTime;
        }
    }
    else if (effectNum == 4) // 덫 피격 파티클
    {
        if (g_particle[threadIndex.x].alive == 0)
        {
            while (true)
            {
                int remaining = g_shared[0].addCount;
                if (remaining <= 0)
                    break;
                int expected = remaining;
                int desired = remaining - 1;
                int originalValue;
                InterlockedCompareExchange(g_shared[0].addCount, expected, desired, originalValue);
                if (originalValue == expected)
                {
                    g_particle[threadIndex.x].alive = 1;
                    break;
                }
            }

            if (g_particle[threadIndex.x].alive == 1)
            {
                float x = ((float)threadIndex.x / (float)maxCount) + accTime;
                float r1 = Rand(float2(x, accTime));
                float r2 = Rand(float2(x * accTime, accTime));
                float r3 = Rand(float2(x * accTime * accTime, accTime * accTime));
                // [0.5~1] -> [0~1]
                float3 noise =
                {
                    2 * r1 - 1,
                    2 * r2 - 1,
                    2 * r3 - 1
                };
                // [0~1] -> [-1~1]
                float3 dir = (noise - 0.5f) * 2.f;
                dir.y = 0.7f;

                float3 pos = (noise.xyz - 0.5f) * 25;
                pos.y = -10.f;
                if (pos.x > 0)
                {
                    if (pos.z > 0)
                    {
                        dir.x = abs(dir.x);
                        dir.z = abs(dir.z);
                    }
                    else if (pos.z < 0)
                    {
                        dir.x = abs(dir.x);
                        dir.z = abs(dir.z);
                        dir.z *= -1.f;
                    }
                    else
                    {
                        dir.x = abs(dir.x);
                        dir.z = 0.f;
                    }
                }
                else if (pos.x < 0)
                {
                    if (pos.z > 0)
                    {
                        dir.x = abs(dir.x);
                        dir.x *= -1.f;
                        dir.z = abs(dir.z);
                    }
                    else if (pos.z < 0)
                    {
                        dir.x = abs(dir.x);
                        dir.x *= -1.f;
                        dir.z = abs(dir.z);
                        dir.z *= -1.f;
                    }
                    else
                    {
                        dir.x = abs(dir.x);
                        dir.x *= -1.f;
                        dir.z = 0.f;
                    }
                }
                else
                {
                    if (pos.z > 0)
                    {
                        dir.x = 0.f;
                        dir.z = abs(dir.z);
                    }
                    else if (pos.z < 0)
                    {
                        dir.x = 0.f;
                        dir.z = abs(dir.z);
                        dir.z *= -1.f;
                    }
                    else
                    {
                        dir.x = 0.f;
                        dir.z = 0.f;
                    }
                }


                g_particle[threadIndex.x].worldDir = normalize(dir);
                g_particle[threadIndex.x].worldPos = pos;
                g_particle[threadIndex.x].lifeTime = ((maxLifeTime - minLifeTime) * noise.x) + minLifeTime;
                g_particle[threadIndex.x].curTime = 0.f;
            }
            
        }
        else
        {
            g_particle[threadIndex.x].curTime += deltaTime;
            if (g_particle[threadIndex.x].lifeTime < g_particle[threadIndex.x].curTime)
            {
                g_particle[threadIndex.x].alive = 0;
                return;
            }

            

            float ratio = g_particle[threadIndex.x].curTime / g_particle[threadIndex.x].lifeTime;
            if (g_particle[threadIndex.x].lifeTime < g_particle[threadIndex.x].curTime * 2.f)
            {
                g_particle[threadIndex.x].worldDir.y -= ratio;
            }
            else
            {
                g_particle[threadIndex.x].worldDir.y -= ratio / 2.f;
            }
            float speed = (maxSpeed - minSpeed) * ratio + minSpeed;
            
            g_particle[threadIndex.x].worldPos += g_particle[threadIndex.x].worldDir * speed * deltaTime;
        }
    }

}

#endif