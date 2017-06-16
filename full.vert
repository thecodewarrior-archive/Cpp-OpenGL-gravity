#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec2 scrPos;


uniform vec2 resolution;
uniform vec2 origin;
uniform float scale;

out vec2 pos;
out float primaryIncrement;
out float secondaryIncrement;

void main(){
    pos = (scrPos*resolution/scale)+origin;

    float lastInc = 0;
    float inc = 1;
    
    while(inc*scale < 100) { lastInc = inc; inc *= 2; }
    
    primaryIncrement = inc;
    secondaryIncrement = lastInc;
    
    gl_Position.xy = scrPos;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;
    
}
