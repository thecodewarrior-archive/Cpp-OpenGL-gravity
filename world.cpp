//
//  world.cpp
//  Tutorials
//
//  Created by Pierce Corcoran on 5/5/17.
//
//
#include <stdio.h>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <math.h>
#include <cstdlib>
#include <set>
#include <unordered_map>
#include <map>
#include <vector>
#include <functional>
#include <algorithm>

#include "ShaderGravity.cpp"

using namespace glm;

class World {
    
public:
    void initGL() { shaderGravity.init(); }
    
    std::vector<Body> bodies;
    ShaderGravity shaderGravity;
    
    World() : shaderGravity(&bodies) {
        add( Body(vec2(0, 0), vec2(0, 0), 1*M_e) );
        add( Body(vec2(0, 1), vec2(0, 0), 1*M_e) );
    }
    
    int add(Body body) {
        int id = bodies.size();
        bodies.push_back(body);
        
        return id;
    }
    
    int bodyCount() {
        return bodies.size();
    }
    
    void fillBuffers(GLuint posBuffer, GLuint colorBuffer) {
        std::vector<vec3> posData;
        posData.reserve(bodies.size());
        std::vector<vec4> otherData;
        otherData.reserve(bodies.size());
        
        
        for (auto iter = bodies.begin(); iter != bodies.end(); iter++) {
            posData.push_back(vec3(iter->pos.x, iter->pos.y, 0));
            
            vec3 color = iter->getColor();
            if(iter == bodies.begin())
                color = vec3(0, 1, 1);
            otherData.push_back(vec4(color.r, color.g, color.b, iter->getRadius()));
        }
        
        glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*posData.size(), posData.data(), GL_DYNAMIC_DRAW);
        
        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec4)*otherData.size(), otherData.data(), GL_DYNAMIC_DRAW);
        
    }
    
    void update(double deltatime) {

        for(auto iter = bodies.begin(); iter != bodies.end(); iter++) {
            iter->update(deltatime);
        }
    
        std::set<std::pair<int, int>> mergePairs;
        int i = 0;
        for(auto iter = bodies.begin(); iter != bodies.end(); iter++) {
            int j = i + 1;
            for(auto other = std::next(iter, 1); other != bodies.end(); other++) {
                if(iter->testMerge(*other, deltatime)) {
                    mergePairs.insert(std::make_pair(i, j));
                }
                j++;
            }
            i++;
        }
    
        
        std::vector<int> toremove;
        for (auto mergeIter = mergePairs.begin(); mergeIter != mergePairs.end(); mergeIter++) {
            int idA = mergeIter->first;
            int idB = mergeIter->second;
            auto bodyA = &bodies[idA];
            auto bodyB = &bodies[idB];
            
            bodyA->merge(*bodyB);
            toremove.push_back(idB);
        }

        if(toremove.size() > 0) {
            
            auto tail = bodies.end();
            tail--;
            
            std::sort(toremove.rbegin(), toremove.rend());
            
            for (auto iter = toremove.begin(); iter != toremove.end(); iter++) {
                auto nth = bodies.begin() + *iter;
                
                std::iter_swap(nth, tail);
                tail--;
            }
            
            bodies.erase(tail, bodies.end());
            
        }
        shaderGravity.update(deltatime);
    }
};