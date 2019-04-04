[VertexShader]
attribute vec4 coord;
varying vec2 texpos;

void main(void) {
  gl_Position = vec4(coord.xy, 0, 1);
  texpos = coord.zw;
}
[FragmentShader]
varying vec2 texpos;
uniform sampler2D tex;
uniform vec4 color;

void main(void) {
  gl_FragColor = vec4(1, 1, 1, texture2D(tex, texpos).a) * color;
}
