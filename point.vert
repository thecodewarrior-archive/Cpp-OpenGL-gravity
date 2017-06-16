#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec4 vertexData;

uniform vec2 resolution;
uniform vec2 origin;
uniform float scale;

out vec3 pointColor;
out float radius;

void main(){
    
    pointColor = vertexData.xyz;
    float scaledSize = vertexData.w * scale;
    
    if(scaledSize < 1) {
        gl_PointSize = 1;
        radius = 3;
    } else {
        gl_PointSize = scaledSize;
        radius = 1;
    }
    gl_Position.xyz = (vertexPosition_modelspace-vec3(origin, 0)) * scale / vec3(resolution.xy, 1);
    gl_Position.w = 1.0;
    
}
