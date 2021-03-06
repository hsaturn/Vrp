[VertexShader]
varying vec3 v_V;
varying vec3 v_N;

void main() {
	gl_Position = ftransform();
	v_V = (gl_ModelViewMatrix * gl_Vertex).xyz;
	v_N = gl_NormalMatrix * gl_Normal;
}
[FragmentShader]
#ifdef GL_ES
precision mediump float;
#endif

#extension GL_OES_standard_derivatives : enable

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

#define ITERATIONS 10
#define SPEED 10.0
#define DISPLACEMENT 0.05
#define TIGHTNESS 10.0
#define YOFFSET 0.1
#define YSCALE 0.25
#define FLAMETONE vec3(50.0, 5.0, 1.0)

float shape(in vec2 pos) // a blob shape to distort
{
	return clamp( sin(pos.x*3.1416) - pos.y+YOFFSET, 0.0, 1.0 );
}

float hash( float n ) { return fract(sin(n)*753.5453123); }
 
// Slight modification of iq's noise function.
float noise( in vec2 x )
{
    vec2 p = floor(x);
    vec2 f = fract(x);
    f = f*f*(3.0-2.0*f);
    
    float n = p.x + p.y*157.0;
    return mix(
                    mix( hash(n+  0.0), hash(n+  1.0),f.x),
                    mix( hash(n+157.0), hash(n+158.0),f.x),
            f.y) * 2.0 - 1.0;
}

void main( void ) {

	vec2 uv = (gl_FragCoord.xy / resolution.xy);
	float nx = 0.0;
	float ny = 0.0;
	for (int i=1; i<ITERATIONS+1; i++)
	{
		float ii = pow(float(i), 2.0);
		float ifrac = float(i)/float(ITERATIONS);
		float t = ifrac * time * SPEED;
		float d = (1.0-ifrac) * DISPLACEMENT;
		nx += noise( vec2(uv.x*ii-time*ifrac, uv.y*YSCALE*ii-t)) * d * 2.0;
		ny += noise( vec2(uv.x*ii+time*ifrac, uv.y*YSCALE*ii-t)) * d;
	}
	float flame = shape( vec2(uv.x+nx, uv.y+ny) );
	vec3 col = pow(flame, TIGHTNESS) * FLAMETONE;
    
    // tonemapping
    col = col / (1.0+col);
    col = pow(col, vec3(1.0/2.2));
    col = clamp(col, 0.0, 1.0);
	gl_FragColor = vec4( col, 1.0 );
}
[Parameters]
vec2 resolution = vec2(640, 480);
