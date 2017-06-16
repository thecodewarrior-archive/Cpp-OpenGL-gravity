#version 330 core

in vec2 pos;
in float primaryIncrement;
in float secondaryIncrement;


uniform float scale;


// Ouput data
out vec3 color;

bool checkInc(float val, float inc) {
    float t = 2/scale;
    return abs(mod(val, inc) - t) < t;
}

void main()
{
    if(secondaryIncrement != 0) {
        if(checkInc(pos.x, primaryIncrement) || checkInc(pos.y, primaryIncrement))
            color = vec3(0.2);
        else if(checkInc(pos.x, secondaryIncrement) || checkInc(pos.y, secondaryIncrement))
            color = vec3(0.15);
        else
            discard;
    } else {
        if(checkInc(pos.x, primaryIncrement) || checkInc(pos.y, primaryIncrement))
            color = vec3(0.2);
        else
            discard;
    }
}

/*

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 uv = (2.0*(fragCoord.xy / iResolution.xy)) - 1.0;
    uv.x = uv.x * (iResolution.x / iResolution.y);
    float len = sqrt(dot(uv, uv));
    vec3 normal = vec3(uv, cos(len));
    normal = normal / sqrt(dot(normal, normal));
    if (dot(uv, uv) > 1.0) {
        fragColor = vec4(1, 0, 0, 1);
    } else {
		vec4 color = vec4(1, 1, 1,1.0);
        float spread = 0.2;
        
        vec3 light = vec3(sin(iGlobalTime), cos(iGlobalTime), 0);
        
        float cosTheta = clamp( dot( normal, light ) + spread, 0.0, 1.0);
        
	fragColor = color * cosTheta;
    }
}

*/
