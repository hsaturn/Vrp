[VertexShader]
varying vec3 v_V;
varying vec3 v_N;
varying vec4 v4color;

void main () {
	v4color = gl_Color;
	gl_Position = ftransform();
	v_V = (gl_ModelViewMatrix * gl_Vertex).xyz;
	v_N = gl_NormalMatrix * gl_Normal;
}
[FragmentShader]
varying vec3 v_V;
varying vec3 v_N;
varying vec4 v4color;
// uniform float amb;


void main () {
	float amb = 0.1;
	//vec3 color = vec3(0, 1.0 , 0);
	vec3 color = vec3(v4color[0], v4color[1], v4color[2]);
	vec3 N = normalize(v_N);
	vec3 V = normalize(v_V);
	vec3 R = reflect(V, N);
	vec3 L = normalize(vec3(gl_LightSource[0].position));

	vec3 ambient = color * amb;
	vec3 diffuse = color * (1.0 - amb) * max(dot(L, N), 0.0);
	vec3 specular = vec3(1.0, 1.0, 1.0) * pow(max(dot(R, L), 0.0), 8.0);

	gl_FragColor = vec4(ambient + diffuse + specular, v4color[3]);

}
