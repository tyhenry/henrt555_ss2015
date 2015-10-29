#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359
#define TWO_PI 6.28318530718

uniform vec2 u_resolution;
uniform float u_time;

//identity matrix
mat3 matrix = mat3(vec3(1.0,0.0,0.0),
                   vec3(0.0,1.0,0.0),
                   vec3(0.0,0.0,1.0));

mat3 scaleMatrix(vec2 f){

    return mat3(vec3(f.x,0.0,0.0),
                vec3(0.0,f.y,0.0),
                vec3(0.0,0.0,1.0));
}

vec3 scale(vec2 f, vec3 pos){
    return scaleMatrix(f) * pos;
}



mat3 translationMatrix(vec2 f){

    return mat3(vec3(1.0,0.0,0.0),
                vec3(0.0,1.0,0.0),
                vec3(f.x,f.y,1.0));
}

vec3 translate(vec2 f, vec3 pos){
    return translationMatrix(f) * pos;
}

mat3 rotationMatrix(float a){

    return mat3(vec3(cos(a),-sin(a),0.0),
                vec3(sin(a),cos(a),0.0),
                vec3(0.0,0.0,1.0));
}

vec3 rotate(float a, vec3 pos){
    return rotationMatrix(a) * pos;
}

//plot a line on y using value 0-1f
float plot(vec2 st, float pct, float smoothing){
  return smoothstep(pct-smoothing, pct, st.y) -
    smoothstep(pct, pct+smoothing, st.y);
}

// Reference to
// http://thndl.com/square-shaped-shaders.html

float shapeDistField(vec2 st, float sides){

  float dist = 0.0;

  // Remap the space to -1. to 1.
  st = st *2.-1.;

  // Angle and radius from the current pixel
  float a = atan(st.x,st.y)+PI;
  float r = TWO_PI/float(sides);

  // Shaping function that modulate the distance
  dist = cos(floor(.5+a/r)*r-a)*length(st);

  return 1.-dist; //invert
}

float equilTriangle(vec2 st, float radius, float smoothing){

	//st.y += 0.1; //fix centering

	float dist = shapeDistField(st, 3.); //generate triangle field
	float pct = smoothstep(radius-smoothing*0.5, radius+smoothing*0.5, dist);
	return pct;
}

float squeezedEquilTriangle(vec2 st, float radius, float smoothing, vec2 scalar){
	vec3 pos = vec3(st, 1.);
	pos = translate(vec2(-.5),pos); //move to center
	pos = scale(scalar, pos); //scale
	pos = translate(vec2(0.5), pos); //move back
	return equilTriangle(pos.xy, radius, smoothing);
}

float box(in vec2 _st, in vec2 _size, in float smoothing){
    _size = vec2(0.5) - _size*0.5;
    vec2 uv = smoothstep(_size,
                        _size+vec2(smoothing),
                        _st);
    uv *= smoothstep(_size,
                    _size+vec2(smoothing),
                    vec2(1.0)-_st);
    return uv.x*uv.y;
}

float trapezoid(vec2 st, vec2 size, float smoothing, float stretch){
  vec3 pos = vec3(st, 1.);
  pos = translate(vec2(-.5),pos); //move to center
  pos = scale(vec2(1.,.8),pos);
  pos = rotate(PI*0.333, pos); //rotate
  pos = scale(vec2(1.-stretch,1.),pos);
  pos = rotate(-PI*0.333, pos); //rotate
  pos = translate(vec2(0.5), pos); //move back
  return box(pos.xy, size, smoothing);
}

//GOLAN CUBIC BEZIER:

// Helper functions:
float slopeFromT (float t, float A, float B, float C){
  float dtdx = 1.0/(3.0*A*t*t + 2.0*B*t + C); 
  return dtdx;
}

float xFromT (float t, float A, float B, float C, float D){
  float x = A*(t*t*t) + B*(t*t) + C*t + D;
  return x;
}

float yFromT (float t, float E, float F, float G, float H){
  float y = E*(t*t*t) + F*(t*t) + G*t + H;
  return y;
}

float cubicBezier(float x, float a, float b, float c, float d){

  float y0a = 0.00; // initial y
  float x0a = 0.00; // initial x 
  float y1a = b;    // 1st influence y   
  float x1a = a;    // 1st influence x 
  float y2a = d;    // 2nd influence y
  float x2a = c;    // 2nd influence x
  float y3a = 1.00; // final y 
  float x3a = 1.00; // final x 

  float A =   x3a - 3.*x2a + 3.*x1a - x0a;
  float B = 3.*x2a - 6.*x1a + 3.*x0a;
  float C = 3.*x1a - 3.*x0a;   
  float D =   x0a;

  float E =   y3a - 3.*y2a + 3.*y1a - y0a;    
  float F = 3.*y2a - 6.*y1a + 3.*y0a;             
  float G = 3.*y1a - 3.*y0a;             
  float H =   y0a;

  // Solve for t given x (using Newton-Raphelson), then solve for y given t.
  // Assume for the first guess that t = x.
  float currentt = x;
  int nRefinementIterations = 5;
  for (int i=0; i < nRefinementIterations; i++){
    float currentx = xFromT (currentt, A,B,C,D); 
    float currentslope = slopeFromT (currentt, A,B,C);
    currentt -= (currentx - x)*(currentslope);
    currentt = clamp(currentt, 0.,1.);
  } 

  float y = yFromT (currentt,  E,F,G,H);
  return y;

}

float squiggle(vec2 st, float mag, float smoothing){

  st *= 10.;
  float y = cubicBezier(st.x, 1.-mag, mag, mag, 1.-mag);
  for (int i=1; i < 10; i+=2){
    y -= cubicBezier(st.x-float(i), mag, 1.-mag, 1.-mag, mag);
    y += cubicBezier(st.x-float(i+1), 1.-mag, mag, mag, 1.-mag);
  }
  //y *= .1;
  st *= .1;
  y = plot(st, y, smoothing);
  return y;
}

void main(){
    vec2 st = gl_FragCoord.xy/u_resolution.xy;

    
    vec3 color = vec3(squiggle(st, sin(u_time)*0.3+0.5, 0.004));

    gl_FragColor = vec4( color, 1.0);
}