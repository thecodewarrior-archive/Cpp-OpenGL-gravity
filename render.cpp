//
//  render.cpp
//  Tutorials
//
//  Created by Pierce Corcoran on 5/5/17.
//
//
#include <stdio.h>
#include <GL/glew.h>
#include <glfw3.h>
#include <common/shader.hpp>
#include <glm/glm.hpp>
#include "world.cpp"

#define timestep 3600 * 24

using namespace glm;

static float frand() {
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

struct point {
    GLfloat x, y, z;
    GLfloat r, g, b, radius;
};

class Render {
public:
    GLuint vertexArrayID, vertexArrayID_f;
    GLuint posBuffer, colorSizeBuffer, fullPosBuffer;
    GLuint pointID, fullID;
    GLuint bufferID;
    GLuint resolutionUniformID, originUniformID, scaleUniformID;
    GLuint resolutionUniformID_f, originUniformID_f, scaleUniformID_f;
    
    World world;
    
    vec2 mouse;
    vec2 mouseWorld;
    ivec2 resolution = ivec2(1024, 768);
    float scale = 50;
    vec2 origin;
public:
    void init() {
        GLuint tmp = 0;
        GLuint* ptr = &tmp;
        glGenVertexArrays(1, ptr);
        vertexArrayID = tmp;
        
        glGenVertexArrays(1, ptr);
        vertexArrayID_f = tmp;
        
        glGenBuffers(1, &tmp);
        posBuffer = tmp;
        
        glGenBuffers(1, &tmp);
        colorSizeBuffer = tmp;
        
        glGenBuffers(1, &tmp);
        fullPosBuffer = tmp;
        
        // Create and compile our GLSL program from the shaders
        pointID = LoadShaders( "point.vert", "point.frag" );
        
        resolutionUniformID = glGetUniformLocation(pointID, "resolution");
        originUniformID = glGetUniformLocation(pointID, "origin");
        scaleUniformID = glGetUniformLocation(pointID, "scale");
        
        
        fullID = LoadShaders( "full.vert", "full.frag" );
        
        resolutionUniformID_f = glGetUniformLocation(fullID, "resolution");
        originUniformID_f = glGetUniformLocation(fullID, "origin");
        scaleUniformID_f = glGetUniformLocation(fullID, "scale");
        
        // setup vertex arrays
        
        // points
        glBindVertexArray(vertexArrayID);
        // (X, Y, Z)
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
        glVertexAttribPointer(
                              0,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              (void*)0
                              );
        
        // (R, G, B, RADIUS)
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorSizeBuffer);
        glVertexAttribPointer(
                              1,
                              4,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              (void*)0
                              );
        
        // full
        glBindVertexArray(vertexArrayID_f);
        // (X, Y, Z)
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, fullPosBuffer);
        glVertexAttribPointer(
                              0,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              0,
                              (void*)0
                              );
        
        glBindBuffer(GL_ARRAY_BUFFER, fullPosBuffer);
        float arr[] = {
            -1.0f, -1.0f, /**/ -1.0f, 1.0f, /**/ 1.0f, -1.0f,
             1.0f, -1.0f, /**/ -1.0f, 1.0f, /**/ 1.0f,  1.0f,
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*2*6, arr, GL_STATIC_DRAW);
        
        world.initGL();
    }
    
    void draw() {
        
        glBindVertexArray(vertexArrayID_f);
        
        // Use our shader
        glUseProgram(fullID);
        
        
        glUniform2f(resolutionUniformID_f, resolution.x, resolution.y);
        glUniform2f(originUniformID_f, origin.x, origin.y);
        if(scale < 0) {
            glUniform1f(scaleUniformID_f, -1/scale);
        } else {
            glUniform1f(scaleUniformID_f, scale);
        }
        
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        glBindVertexArray(vertexArrayID);
        glEnable(GL_PROGRAM_POINT_SIZE);
        
        // Use our shader
        glUseProgram(pointID);
        
        
        glUniform2f(resolutionUniformID, resolution.x, resolution.y);
        glUniform2f(originUniformID, origin.x, origin.y);
        if(scale < 0) {
            glUniform1f(scaleUniformID, -1/scale);
        } else {
            glUniform1f(scaleUniformID, scale);
        }

        world.fillBuffers(posBuffer, colorSizeBuffer);
        
        glDrawArrays(GL_POINTS, 0, world.bodyCount());
        
    }
    
    void clean() {
        glDeleteBuffers(1, &posBuffer);
        glDeleteBuffers(1, &colorSizeBuffer);
        glDeleteVertexArrays(1, &vertexArrayID);
        glDeleteProgram(pointID);
    }
    
    void update(double deltatime) {
        world.update(deltatime * timestep);
    }
    
    vec2 creating;
    bool isCreating;
    
    vec2 draggingStart;
    bool isDragging;

    void keyDown(int key, int scancode, int mods) {
        if(key == GLFW_KEY_O) {
            Body creating = Body(mouseWorld, vec2(), 0.15*M_e);
            
            if(world.bodies.size() != 0) {
                creating.orbitAround(world.bodies[0]);
            }
            
            world.add(creating);
        }
        if(key == GLFW_KEY_C) {
            creating = mouseWorld;
            isCreating = true;
        }
        if(key == GLFW_KEY_S) {
            if(world.bodies.size() > 0) {
                float closestDistance = std::numeric_limits<float>::infinity();
                int closestID = 0;
                int i = 0;
                for(auto iter = world.bodies.begin(); iter != world.bodies.end(); iter++) {
                    vec2 dPos = iter->pos - mouseWorld;
                    float r2 = dot(dPos, dPos);
                    
                    if(r2 < closestDistance) {
                        closestDistance = r2;
                        closestID = i;
                    }
                    i++;
                }
                std::iter_swap(world.bodies.begin(), world.bodies.begin() + closestID);
            }
        }
        if(key == GLFW_KEY_P) {
            if(world.bodies.size() != 0) {
                Body* selected = &world.bodies[0];
                
                float radius2 = dot(selected->pos - mouseWorld, selected->pos - mouseWorld);
                float radius = sqrt(radius2);
                
                float area = M_PI * radius2;
                float density = 64;
                float baseMass = selected->mass;
                float bodyMass = 0.01*M_e;
                
                Body fakeOrbit = Body(selected->pos, selected->vel, baseMass);
                
                for(int iters = area*density; iters > 0; iters--) {
                    float theta = 2*M_PI*frand();
                    float r = frand()+frand();
                    if(r > 1) {
                        r = 2-r;
                    }
                    r *= radius;
                    
                    vec2 pos = selected->pos + vec2(r*cos(theta), r*sin(theta));
                    Body body = Body(pos, vec2(), bodyMass);
                    
                    fakeOrbit.mass = baseMass + (M_PI*r*r * density * bodyMass);
                    body.orbitAround(fakeOrbit);
                    
                    world.add(body);
                }
                
                
            }
        }
    }
    void keyUp(int key, int scancode, int mods) {
        if(key == GLFW_KEY_C && isCreating) {
            vec2 vel = creating - mouseWorld;
            
            vel /= 10*timestep;
            
            world.add(Body(creating, vel, 0.15*M_e));
            isCreating = false;
        }
    }
    
    void mouseDown(int button, int mods) {
        if(button == GLFW_MOUSE_BUTTON_MIDDLE) {
            draggingStart = mouseWorld;
            isDragging = true;
        }
    }
    
    void mouseUp(int button, int mods) {
        if(button == GLFW_MOUSE_BUTTON_MIDDLE) {
            isDragging = false;
        }
    }
    
    void mousePos(double x, double y) {
        mouse = vec2(x, y);
        
        mouseWorld = screen2World(mouse);

        if(isDragging) {
            vec2 dPos = mouseWorld - draggingStart;
            origin -= dPos;
        }
        
    }
    
    void mouseWheel(double xOffset, double yOffset) {
        scale += yOffset;
        if(scale == 0)
            scale += yOffset;
    }
    
    vec2 screen2World(vec2 pos) {
        vec2 glPos = (pos/vec2(resolution/2))-vec2(1, 1);
        glPos.y *= -1;
        return (glPos * (vec2(resolution)/(scale < 0 ? -1/scale : scale))) + origin;
    }
};