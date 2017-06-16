//
//  body.cpp
//  Tutorials
//
//  Created by Pierce Corcoran on 5/13/17.
//
//

#include <stdio.h>
#include <glm/glm.hpp>

using namespace glm;

// mass is measured in in zettagrams (Zg) each being 10^18 kg
#define ZETTAGRAM 1e18
#define Zg *ZETTAGRAM
// length is measured in gigametres (Gm) each being 10^9 km
#define GIGAMETER 1e9
#define Gm *GIGAMETER

// Rho = (density in g/cm^3) x10^12 Zg/Gm^3
// Density of jupiter in g/cm^3 = 1.3
#define RHO 1.3e12

// g modified to be in Gm^3 • Zg^-1 • s^2
#define G 6.67431e-20

// mass of earth, jupiter, and sun in zettagrams
#define M_e 5.9742e6
#define M_j 1.898e9
#define M_s 1.988e12

// astronomical unit and lightyear
#define Au 149.597870700
#define Ly 9.4607e3

struct Body {
    Body(vec2 pos, vec2 vel, float mass) : pos(pos), vel(vel) {
        setMass(mass);
    }
    
    Body() : mass(0), radius(0), pos(vec2(0, 0)), vel(vec2(0, 0)) {}
    
    float mass;
    float radius;
    vec2 pos, vel;
    
    float getMass() const { return mass; }
    void setMass(float m) {
        mass = m;
        radius = cbrt((3*M_PI_4/RHO)*mass);
    }
    
    float getRadius() const { return radius; }
    
    vec3 getColor() const {
        return vec3(1, 1, 1);
    }
    
    void update(float deltatime) {
        pos += vel * deltatime;
    }
    
    void merge(Body& other) {
        float totalMass = this->mass + other.mass;
        vec2 newPos = (this->mass/totalMass)*this->pos + (other.mass/totalMass)*other.pos;
        vec2 newVel = (this->mass/totalMass)*this->vel + (other.mass/totalMass)*other.vel;
        
        
        this->setMass(totalMass);
        this->pos = newPos;
        this->vel = newVel;
    }
    
    void orbitAround(Body& other) {
        vec2 dPos = this->pos - other.pos;
        
        float length = sqrt(dot(dPos, dPos));
        if(length == 0) return;
        
        float velocity = sqrt( (G * other.mass) / length );
        
        vec2 normal = vec2(dPos.y, -dPos.x)/length;
        
        this->vel = normal * velocity + other.vel;
    }
    
    static float fastInvSqrt(float x) {
        float xhalf = 0.5f * x;
        int i = *(int*)&x;              // get bits for floating value
        i = 0x5f375a86 - (i >> 1);      // gives initial guess y0
        x = *(float*)&i;                // convert bits back to float
        x = x * (1.5f - xhalf * x * x); // Newton step, repeating increases accuracy
        return x;
    }
    
    bool testMerge(Body other, double deltatime) {
        float d_x = other.pos.x - this->pos.x;
        float d_y = other.pos.y - this->pos.y;
        
        float r2 = d_x*d_x + d_y*d_y;
        float threshold = (this->radius + other.radius) * (this->radius + other.radius);
        if(r2 == 0) return false;
        
        if(r2 < threshold)
            return true;
        else
            return false;
    }
    
};
