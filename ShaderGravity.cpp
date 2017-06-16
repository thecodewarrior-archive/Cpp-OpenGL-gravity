//
//  ShaderGravity.cpp
//  Tutorials
//
//  Created by Pierce Corcoran on 5/13/17.
//
//
#define MAX_PARTICLE_TEX_DIMENSION __MAX_PARTICLE_TEX_SIZE
#define MAX_PARTICLE_SIZE MAX_PARTICLE_TEX_DIMENSION * MAX_PARTICLE_TEX_DIMENSION

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include <map>
#include <vector>
#include "body.cpp"

using namespace glm;

static int __MAX_PARTICLE_TEX_SIZE;

class ShaderGravity {
    std::vector<Body>* bodies;
    GLuint inputTex;
    GLuint outputTex;
    GLuint vertArray;
    GLuint shader;
    GLuint framebuffer;
public:
    ShaderGravity(std::vector<Body>* bodies) : bodies(bodies) {
    }
    
    void update(double deltatime) {
        int size = updateTexture();
    }
    
    void init() { _init(); }
    
private:
    void _init() {
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &__MAX_PARTICLE_TEX_SIZE);
        
        glGenTextures(1, &inputTex);
        glGenTextures(1, &outputTex);
        
    }
    
    int updateTexture() {
        int size = ceil(sqrt(bodies->size()));
        if(size < 1) size = 1;
        
        glBindTexture(GL_TEXTURE_2D, inputTex);
        
        std::vector<vec3> data;
        data.reserve(size*size);

        for(auto body = bodies->begin(); body != bodies->end(); body++) {
            data.push_back(vec3(body->pos.x, body->pos.y, body->mass));
        }
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, size, size, 0, GL_RGB, GL_FLOAT, data.data());
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        
        return size;
    }
};