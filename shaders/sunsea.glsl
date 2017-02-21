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
precision highp float;
#endif

#extension GL_OES_standard_derivatives : enable

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

#define M_PI (3.14159265358979)
#define GRAVITY (9.80665)
#define EPS (1e-3)
#define USE_ROUGHNESS_TEXTURE
#define WAVENUM (32)

const float kSensorWidth = 36e-3;
const float kSensorDist = 18e-3;

const vec2 wind = vec2(0.0, 1.0);
const float kOceanScale = 20.0;

struct Ray
{
vec3 o;
    vec3 dir;
};

struct HitInfo
{
	vec3 pos;
    vec3 normal;
    float dist;
    Ray ray;
};

float rand(vec2 n) {
    return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

float rand(vec3 n)
{
    return fract(sin(dot(n, vec3(12.9898, 4.1414, 5.87924))) * 43758.5453);
}

float Noise2D(vec2 p)
{
    vec2 e = vec2(0.0, 1.0);
    vec2 mn = floor(p);
    vec2 xy = fract(p);

    float val = mix(
        mix(rand(mn + e.xx), rand(mn + e.yx), xy.x),
        mix(rand(mn + e.xy), rand(mn + e.yy), xy.x),
        xy.y
    );

    val = val * val * (3.0 - 2.0 * val);
    return val;
}

float Noise3D(vec3 p)
{
    vec2 e = vec2(0.0, 1.0);
    vec3 i = floor(p);
    vec3 f = fract(p);

    float x0 = mix(rand(i + e.xxx), rand(i + e.yxx), f.x);
    float x1 = mix(rand(i + e.xyx), rand(i + e.yyx), f.x);
    float x2 = mix(rand(i + e.xxy), rand(i + e.yxy), f.x);
    float x3 = mix(rand(i + e.xyy), rand(i + e.yyy), f.x);

    float y0 = mix(x0, x1, f.y);
    float y1 = mix(x2, x3, f.y);

    float val = mix(y0, y1, f.z);

    val = val * val * (3.0 - 2.0 * val);
    return val;
}

float SmoothNoise(vec3 p)
{
    float amp = 1.0;
    float freq = 1.0;
    float val = 0.0;

    for (int i = 0; i < 4; i++)
    {
        amp *= 0.5;
        val += amp * Noise3D(freq * p - float(i) * 11.7179);
        freq *= 2.0;
    }

    return val;
}

float Pow5(float x)
{
    return (x * x) * (x * x) * x;
}

// GGX Distribution
// Ref: https://www.cs.cornell.edu/~srm/publications/EGSR07-btdf.pdf
float DTerm(vec3 l, float HDotN, float alpha2)
{
    float HDotN2 = HDotN * HDotN;
    float x = (1.0 - (1.0 - alpha2) * HDotN2);
    float D = alpha2 / (M_PI * x * x);
    return D;
}

// Smith Joint Masking-Shadowing Function
// Ref: https://hal.inria.fr/hal-00942452v1/document
float GTerm(float LDotN, float VDotN, float alpha2)
{
    float tanThetaLN2 = 1.0 / (LDotN * LDotN) - 1.0;
    float tanThetaVN2 = 1.0 / (VDotN * VDotN) - 1.0;

    float lambdaL = 0.5 * sqrt(1.0 + alpha2 * tanThetaLN2) - 0.5;
    float lambdaV = 0.5 * sqrt(1.0 + alpha2 * tanThetaVN2) - 0.5;

    return 1.0 / (1.0 + lambdaL + lambdaV);
}

// Schlick approx
// Ref: https://en.wikipedia.org/wiki/Schlick's_approximation
float FTerm(float LDotH, float f0)
{
    return f0 + (1.0 - f0) * Pow5(1.0 - LDotH);
}

float TorranceSparrowBRDF(vec3 v, vec3 l, HitInfo hit, float roughness)
{
    vec3 h = normalize(l + v);

    float VDotN = dot(v, hit.normal);
    float LDotN = dot(l, hit.normal);
    float HDotN = dot(h, hit.normal);
    float LDotH = dot(l, h);

    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;

    float D = DTerm(l, HDotN, alpha2);
    float G = GTerm(LDotN, VDotN, alpha2);
    float F = FTerm(LDotH, 0.95);
    return (D * G * F) / (4.0 * abs(VDotN) * abs(LDotN));
}

struct OceanInfo
{
    float height;
    vec3 normal;
};

float OceanHeight(vec2 p)
{
    float height = 0.0;
    vec2 grad = vec2(0.0, 0.0);
    float t = time;

    float windNorm = length(wind);
    float windDir = atan(wind.y, wind.x);

    for (int i = 1; i < WAVENUM; i++)
    {
        float rndPhi = windDir + asin(2.0 * rand(vec2(0.141 * float(i), 0.1981)) - 1.0);
        // float kNorm = 2.0 * M_PI * (rand(vec2(0.81765 * float(i), 0.873)) * float(WAVENUM)) / kOceanScale;
        float kNorm = 2.0 * M_PI * float(i) / kOceanScale;
        vec2 kDir = vec2(cos(rndPhi), sin(rndPhi));
        vec2 k = kNorm * kDir;
        float l = (windNorm * windNorm) / GRAVITY;
        float amp = exp(-0.5 / (kNorm * kNorm * l * l)) / (kNorm * kNorm);
        float omega = sqrt(GRAVITY * kNorm + 0.01 * sin(p.x));
        float phi = 2.0 * M_PI * rand(vec2(0.6814 * float(i), 0.7315));

        vec2 p2 = p;
        p2 -= amp * k * cos(dot(k, p2) - omega * t + phi);
        height += amp * sin(dot(k, p2) - omega * t + phi);
    }
	// return PNoise(p - t);
    return height;
}

vec3 OceanNormal(vec2 p, vec3 camPos)
{
    vec2 e = vec2(0, 1.0 * EPS);
    float l = 20.0 * distance(vec3(p.x, 0.0, p.y), camPos);
    e.y *= l;

    float hx = OceanHeight(p + e.yx) - OceanHeight(p - e.yx);
    float hz = OceanHeight(p + e.xy) - OceanHeight(p - e.xy);
    return normalize(vec3(-hx, 2.0 * e.y, -hz));
}

bool RayMarchOcean(Ray ray, out HitInfo hit) {
    vec3 rayPos = ray.o;
    float dl = rayPos.y / abs(ray.dir.y);
    rayPos += ray.dir * dl;
    hit.pos = rayPos;
    hit.normal = OceanNormal(rayPos.xz, ray.o);
    hit.dist = length(rayPos - ray.o);
    return true;
}

#define CLOUD_ITER (32)
vec3 RayMarchCloud(Ray ray, vec3 sunDir, vec3 bgColor)
{
    float cloudHeight = 100.0;

    vec3 rayPos = ray.o;
    rayPos += ray.dir * (cloudHeight - rayPos.y) / ray.dir.y;

    float c = clamp(dot(sunDir, -ray.dir), 0.0, 1.0);

    float dl = 1.0;
    float scatter = 0.0;
    vec3 t = bgColor;
    for(int i = 0; i < CLOUD_ITER; i++) {
        rayPos += dl * ray.dir;
        float dens = SmoothNoise(vec3(0.01, 0.005, 0.05) * rayPos - vec3(0, 0.2 * time, 1.0 * time)) *
            smoothstep(0.0, 1.0, SmoothNoise(vec3(0.003, 0.001, 0.005) * rayPos)) * smoothstep(cloudHeight, cloudHeight + 1.0, rayPos.y);
        t -= 0.01 * t * dens * dl;
        t += 0.02 * dens * dl;
	}
    return t;
}

vec3 BGColor(vec3 dir, vec3 sunDir) {
    vec3 color = vec3(0);

    color += mix(
        vec3(0.094, 0.2266, 0.3711),
        vec3(0.988, 0.6953, 0.3805),
       	clamp(0.0, 1.0, dot(sunDir, dir) * dot(sunDir, dir)) * smoothstep(-0.25, 0.1, sunDir.y)
    );

    dir.x += 0.01 * sin(312.47 * dir.y + time) * exp(-40.0 * dir.y);
    dir = normalize(dir);

    color += smoothstep(0.995, 1.0, dot(sunDir, dir));
	return color;
}

void main( void )
{
	vec2 uv = ( gl_FragCoord.xy / resolution.xy ) * 2.0 - 1.0;
	float aspect = resolution.y / resolution.x;

    // Camera settings
	vec3 camPos = vec3(0, 0.5 + 0.1 * (0.5 * sin(2.0 * time/10000.0) + 0.5), -10.0 * time/10000.0);
    vec3 camDir = vec3(0.002 * (rand(vec2(time/100.0, 0.0)) - 0.5), 0.002 * (rand(vec2(time/100.00, 0.1)) - 0.5), -1);
	vec3 camTarget = vec3(camPos + camDir);

    vec3 up = vec3(0.2 * (SmoothNoise(vec3(0.1 * time, 0.0, 0.0)) - 0.5), 1.0, 0.0);

	vec3 camForward = normalize(camTarget - camPos);
	vec3 camRight = cross(camForward, up);
	vec3 camUp = cross(camRight, camForward);

    Ray ray;
    ray.o = camPos;
    ray.dir = normalize(
        kSensorDist * camForward +
        kSensorWidth * 0.5 * uv.x * camRight +
        kSensorWidth * 0.5 * aspect * uv.y * camUp
    );

    vec3 sunDir = normalize(vec3(0, -0.1 + step(mouse.x, 0.5) * 0.2 * mouse.y + 0.2 * step(0.5, mouse.x)  * (0.5 * sin(10.0 * time/100.0) + 0.5), -1));

    vec3 color = vec3(0);
	HitInfo hit;
    float l = 1.0;
    if (ray.dir.y < 0.0 && RayMarchOcean(ray, hit)) {
        vec3 baseColor = vec3(0.0, 0.2648, 0.4421) * dot(-ray.dir, vec3(0, 1, 0));

        vec3 refDir = reflect(ray.dir, hit.normal);
        refDir.y = abs(refDir.y);
        l = (0.0 - camPos.y) / ray.dir.y;
        // float roughness = clamp(0.0, 1.0, 1.0 - 1.0 / (0.1 * l));
        // float brdf = TorranceSparrowBRDF(-ray.dir, sunDir, hit, roughness) * clamp(dot(sunDir, hit.normal), 0.0, 1.0);
        // float brdf2 = TorranceSparrowBRDF(-ray.dir, refDir, hit, roughness) * clamp(dot(refDir, hit.normal), 0.0, 1.0);

        color = baseColor + BGColor(refDir, sunDir) * FTerm(dot(refDir, hit.normal), 0.5);
    } else {
        vec3 bgColor = BGColor(ray.dir, sunDir);
        if (ray.dir.y > 0.0)
        {
            color += RayMarchCloud(ray, sunDir, bgColor);
        }
        l = (100.0 - camPos.y) / ray.dir.y;
    }

    color = mix(color, BGColor(ray.dir, sunDir), 1.0 - exp(-0.0001 * l));
    color = smoothstep(0.2, 0.9, color);
    color *= 1.0 - 0.01 * rand(vec2(uv) + time);
	gl_FragColor = vec4(color, 1.0);
}
[Parameters]
vec2 mouse = vec2(320, 200);
vec2 resolution = vec2(640, 480);
