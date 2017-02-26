[VertexShader]
varying vec3 v_V;
varying vec3 v_N;

void main() {
	gl_Position = ftransform();
	v_V = (gl_ModelViewMatrix * gl_Vertex).xyz;
	v_N = gl_NormalMatrix * gl_Normal;
}
[FragmentShader]
// Digitized
// By: Brandon Fogerty
// bfogerty@gmail.com

#ifdef GL_ES
precision mediump float;
#endif

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

float random( vec2 p )
{	
	return fract( sin( fract( sin( p.x ) ) + p.y) * 42.17563);
}

float worley( vec2 p, float timeSpeed )
{
	float d = 10.0;
	for( int xo = -1; xo <= 1; xo++ )
	{
		for( int yo = -1; yo <= 1; yo++ )
		{
			vec2 test_cell = floor(p) + vec2( xo, yo );
			
			float f1 = random( test_cell );
			float f2 = random( test_cell + vec2(1.0,83.0) );
			
			float xp = mix( f1, f2, sin(time*timeSpeed) );
			float yp = mix( f1, f2, cos(time*timeSpeed) );
			
			vec2 c = test_cell + vec2(xp,yp);
			
			vec2 cTop = p - c;
			d = min( d, dot(cTop,cTop) );
		}
	}
	return d;
}

float worley2( vec2 p )
{
	float d = 10.0;
	for( int xo = -1; xo <= 1; xo++ )
	{
		for( int yo = -1; yo <= 1; yo++ )
		{
			vec2 test_cell = floor(p) + vec2( xo, yo );
			
			vec2 c = test_cell;
			
			vec2 cTop = p - c;
			d = min( d, dot(cTop,cTop) );
		}
	}
	return d;
}


void main( void ) 
{
	vec2 uv = (gl_FragCoord.xy / resolution.xy) * 2.0 - 1.0;

	float t = worley( gl_FragCoord.xy / 20.0,1. );
	vec3 finalColor = vec3( t,t,0) * 2.0;
	

	
	finalColor *= smoothstep(1.0, 0.0, length(uv.y) * 0.5 );
	
	gl_FragColor = vec4( finalColor, 1.0 );
}
[Parameters]
vec2 resolution = vec2(640, 480);
