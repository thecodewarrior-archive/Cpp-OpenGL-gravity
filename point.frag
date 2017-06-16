#version 330 core

in vec3 pointColor;
in float radius;

// Ouput data
out vec3 color;

void main()
{
    vec2 circCoord = 2.0 * gl_PointCoord - 1.0;
    if (dot(circCoord, circCoord) > radius*radius) {
        discard;
    }
    // Output color = red
    color = pointColor;
    
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
