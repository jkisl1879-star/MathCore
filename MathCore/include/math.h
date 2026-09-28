
#pragma once
#ifndef WG_MATH_H
#define WG_MATH_H

#include <cmath>
#include <string>
#include <sstream>
#include <algorithm>

namespace WG{

constexpr float PI=3.14159265f;
inline float rad(float d){return d*PI/180.f;}

struct Vec2{
    float x=0,y=0;
    Vec2()=default;
    Vec2(float x,float y):x(x),y(y){}
    Vec2 operator+(Vec2 v)const{return {x+v.x,y+v.y};}
    Vec2 operator-(Vec2 v)const{return {x-v.x,y-v.y};}
    Vec2 operator*(float s)const{return {x*s,y*s};}
    float length()const{return sqrtf(x*x+y*y);}
};

struct Vec3{
    float x=0,y=0,z=0;
    Vec3()=default;
    Vec3(float x,float y,float z):x(x),y(y),z(z){}
    Vec3 operator+(Vec3 v)const{return {x+v.x,y+v.y,z+v.z};}
    Vec3 operator-(Vec3 v)const{return {x-v.x,y-v.y,z-v.z};}
	Vec3 operator-()const{return {-x,-y,-z};}
    Vec3 operator*(float s)const{return {x*s,y*s,z*s};}
    Vec3 operator/(float s)const{return {x/s,y/s,z/s};}
    float dot(Vec3 v)const{return x*v.x+y*v.y+z*v.z;}
    Vec3 cross(Vec3 v)const{return {y*v.z-z*v.y,z*v.x-x*v.z,x*v.y-y*v.x};}
    float length()const{return sqrtf(dot(*this));}
    Vec3 normalized()const{return length()>0?*this/length():Vec3{};}
};

struct Rotation{
    float x=0,y=0,z=0;
    Rotation()=default;
    Rotation(float x,float y,float z):x(x),y(y),z(z){}

    Vec3 rotate(Vec3 v)const{
        float a=rad(x),b=rad(y),c=rad(z);
        v={v.x,v.y*cosf(a)-v.z*sinf(a),v.y*sinf(a)+v.z*cosf(a)};
        v={v.x*cosf(b)+v.z*sinf(b),v.y,-v.x*sinf(b)+v.z*cosf(b)};
        return {v.x*cosf(c)-v.y*sinf(c),v.x*sinf(c)+v.y*cosf(c),v.z};
    }

    Vec3 right()const{return rotate({1,0,0});}
    Vec3 up()const{return rotate({0,1,0});}
    Vec3 forward()const{return rotate({0,0,1});}
};

struct AABB{
    Vec3 min{},max{};
    AABB()=default;
    AABB(Vec3 a,Vec3 b):min(a),max(b){}

    static AABB fromCenter(Vec3 p,Vec3 s){return {p-s*.5f,p+s*.5f};}
    Vec3 center()const{return (min+max)*.5f;}
    Vec3 size()const{return max-min;}

    bool contains(Vec3 p)const{
        return p.x>=min.x&&p.x<=max.x&&p.y>=min.y&&p.y<=max.y&&p.z>=min.z&&p.z<=max.z;
    }

    bool intersects(const AABB& b)const{
        return min.x<=b.max.x&&max.x>=b.min.x&&min.y<=b.max.y&&max.y>=b.min.y&&min.z<=b.max.z&&max.z>=b.min.z;
    }
};

struct OBB{
    Vec3 position{},size{1,1,1};
    Rotation rotation{};

    OBB()=default;
    OBB(Vec3 p,Vec3 s,Rotation r={}):position(p),size(s),rotation(r){}

    Vec3 axis(int i)const{return i==0?rotation.right():i==1?rotation.up():rotation.forward();}
    Vec3 corner(int i)const{
        Vec3 h=size*.5f;
        return position+axis(0)*(i&1?h.x:-h.x)+axis(1)*(i&2?h.y:-h.y)+axis(2)*(i&4?h.z:-h.z);
    }

    AABB bounds()const{
        Vec3 a=corner(0),b=a;
        for(int i=1;i<8;i++){
            Vec3 p=corner(i);
            a={std::min(a.x,p.x),std::min(a.y,p.y),std::min(a.z,p.z)};
            b={std::max(b.x,p.x),std::max(b.y,p.y),std::max(b.z,p.z)};
        }
        return {a,b};
    }
};

struct CollisionInfo{
    bool hit=false,touching=false;
    Vec3 point{},normal{},overlap{};
    float depth=0;
    std::string axis="None";
};

inline CollisionInfo checkCollision(const OBB& a,const OBB& b){
    CollisionInfo c;
    Vec3 A[3]={a.axis(0),a.axis(1),a.axis(2)},B[3]={b.axis(0),b.axis(1),b.axis(2)};
    Vec3 h=a.size*.5f,k=b.size*.5f,d=b.position-a.position;
    float best=1e30f;
    int id=0;

    auto test=[&](Vec3 n,int i){
        float l=n.length();if(l<1e-6f)return true;
        n=n/l;
        float x=h.x*fabsf(A[0].dot(n))+h.y*fabsf(A[1].dot(n))+h.z*fabsf(A[2].dot(n));
        float y=k.x*fabsf(B[0].dot(n))+k.y*fabsf(B[1].dot(n))+k.z*fabsf(B[2].dot(n));
        float o=x+y-fabsf(d.dot(n));if(o<0)return false;
        if(o<best){best=o;c.normal=d.dot(n)>=0?n:-n;c.axis=std::to_string(i);}
        return true;
    };

    for(int i=0;i<3;i++)if(!test(A[i],id++))return c;
    for(int i=0;i<3;i++)if(!test(B[i],id++))return c;
    for(int i=0;i<3;i++)for(int j=0;j<3;j++)if(!test(A[i].cross(B[j]),id++))return c;

    c.hit=true;c.depth=best;c.touching=best<1e-5f;
    c.point=(a.position+b.position)*.5f;
    return c;
}

inline CollisionInfo checkCollision(const AABB& a,const AABB& b){
    CollisionInfo c;
    if(!a.intersects(b))return c;
    c.hit=true;
    c.overlap={
        std::min(a.max.x,b.max.x)-std::max(a.min.x,b.min.x),
        std::min(a.max.y,b.max.y)-std::max(a.min.y,b.min.y),
        std::min(a.max.z,b.max.z)-std::max(a.min.z,b.min.z)
    };
    c.depth=c.overlap.x;c.axis="X";
    if(c.overlap.y<c.depth)c.depth=c.overlap.y,c.axis="Y";
    if(c.overlap.z<c.depth)c.depth=c.overlap.z,c.axis="Z";
    Vec3 d=b.center()-a.center();
    c.normal=c.axis=="X"?Vec3{d.x>=0?1.f:-1.f,0,0}:c.axis=="Y"?Vec3{0,d.y>=0?1.f:-1.f,0}:Vec3{0,0,d.z>=0?1.f:-1.f};
    c.touching=c.depth<1e-5f;
    c.point=(a.center()+b.center())*.5f;
    return c;
}

inline std::string debugVec(Vec3 v){
    std::ostringstream s;
    s<<"("<<v.x<<", "<<v.y<<", "<<v.z<<")";
    return s.str();
}

inline std::string debugCollision(const OBB& a,const OBB& b){
    auto c=checkCollision(a,b);
    std::ostringstream s;
    s<<"Hit: "<<c.hit<<"\nDepth: "<<c.depth<<"\nAxis: "<<c.axis
     <<"\nNormal: "<<debugVec(c.normal)<<"\nPoint: "<<debugVec(c.point);
    for(int i=0;i<8;i++)s<<"\nA corner "<<i<<": "<<debugVec(a.corner(i));
    for(int i=0;i<8;i++)s<<"\nB corner "<<i<<": "<<debugVec(b.corner(i));
    return s.str();
}

}

#endif