#pragma once

#include <string>

#define dti_size_assert(T, size) static_assert(sizeof(T) == size, "Size of " #T " is not " #size " bytes")
#define dti_offset_assert(T, member, offset) static_assert(offsetof(T, member) == offset, "Offset of " #T "::" #member " is not at offset " #offset)

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;
typedef float f32;
typedef double f64;

typedef u32 color; // 0xRRGGBBAA
typedef f32 matrix44[4][4]; // [R][C]
typedef f32 float2[2];
typedef f32 float3[3];
typedef f32 float4[4];
typedef char* cstring;
typedef char* string;
typedef f32 float3x3[3][3];
typedef f32 float4x3[4][3];
typedef f32 float3x4[3][4];
typedef f32 float4x4[4][4];
typedef f32 matrix33[3][3]; // [R][C]
typedef void* classref;
typedef u64 time_;

struct point
{
    u32 x, y;
};
struct size
{
    u32 x, y;
};
struct rect
{
    u32 x, y, w, h;
};
struct vector3
{
    f32 x, y, z;
};
struct vector4
{
    f32 x, y, z, w;
};
struct quaternion
{
    union {
        f32 x, y, z, w;
        float4 values;
    };
};

struct easecurve
{
    union {
        float2 floats;
        f64 longfloat;
    };
};
struct line
{
    vector3 from, dir;
};
typedef line ray;

struct linesegment
{
    vector3 p0, p1;
};
typedef linesegment linesegment4[4];
struct Plane
{
    vector3 normal;
};
struct sphere
{
    float3 pos;
    f32 r;
};
struct capsule
{
    vector3 p0, p1;
    f32 r;
};
struct aabb
{
    vector3 minpos, maxpos;
};
typedef aabb aabb4[4];
struct obb
{
    matrix44 coord;
    vector3 extent;
};
struct cylinder
{
    vector3 p0;
    vector3 p1;
    f32 r;
};
struct triangle
{
    vector3 p0, p1, p2;
};
struct range
{
    s32 min, max;
};
struct rangef
{
    f32 min, max;
};
struct rangeu16
{
    u16 min, max;
};
struct hermitecurve
{
    f32 values[16];
};
struct vector2
{
    f32 x, y;
};
typedef vector2 pointf;
typedef vector2 sizef;
struct rectf
{
    f32 x, y, w, h;
};
struct plane_xz
{
    vector3 normal;
};
struct pthread_mutex;
typedef pthread_mutex* pthread_mutex_t;


using MtVector2 = vector2;
using MtVector3 = vector3;
using MtVector4 = vector4;
using MtQuaternion = MtVector4;
using MtFloat3 = MtVector3;
using MtFloat4 = MtVector4;

union MtColor
{
    u32 rgba;
    struct {
        u32 r : 8;
        u32 g : 8;
        u32 b : 8;
        u32 a : 8;
    };
};
struct MtMatrix
{
    MtVector4 m[4];
};
struct MtSphere
{
    MtFloat3 pos;
    f32 r;
};
struct MtAABB
{
    MtVector3 minpos;
    MtVector3 maxpos;
};
struct MtOBB
{
    MtMatrix coord;
    MtVector3 extent;
};
struct MtColorF
{
    f32 r, g, b, a;
};
struct MtEaseCurve
{
    f32 p1, p2;
};
struct MtCriticalSection
{
    pthread_mutex_t mCSection;
};
union MtPoint
{
    u64 xy;
    struct { u32 x, y; };
};
struct MtLineSegment
{
    MtVector3 p0, p1;
};
struct MtHalf4
{
    s64 x, y, z, w;
};
struct MtHalf2
{
    s64 x, y;
};



template<class T = void>
class MtDTI
{
public:
    cstring mName;
    MtDTI<T>* mpNext;
    MtDTI<T>* mpChild;
    MtDTI<T>* mpParent;
    MtDTI<T>* mpLink;

    union 
    {
        struct
        {
            u32 mSize : 23;
            u32 mAllocatorIndex : 6;
            u32 mAttr : 3;
        };
        u32 mFlags;
    };
    
    u32 mCRC;

    virtual ~MtDTI() = 0;
    virtual T* NewInstance() = 0;
    virtual T* CtorInstance(T* obj) = 0;
    virtual T* CtorInstanceArray(T* obj, s64 count) = 0;

    size_t GetSize() const
    {
        return static_cast<u64>(mSize) << 2;
    }
    bool InheritsFrom(const std::string& type) const
    {
	    for(auto dti = this->mpParent; dti != nullptr; dti = dti->mpParent)
	    {
            if (type == dti->mName)
                return true;
	    }

        return false;
    }
    bool InheritsFrom(u32 crc) const
    {
        for (auto dti = this->mpParent; dti != nullptr; dti = dti->mpParent)
        {
            if (crc == dti->mCRC)
                return true;
        }

        return false;
    }
    template<class X>
    bool InheritsFrom(const MtDTI<X>* other) const
    {
        for (auto dti = this->mpParent; dti != nullptr; dti = dti->mpParent)
        {
            if (other->mCRC == dti->mCRC)
                return true;
        }

        return false;
    }
};

using AnyDTI = MtDTI<void>;
