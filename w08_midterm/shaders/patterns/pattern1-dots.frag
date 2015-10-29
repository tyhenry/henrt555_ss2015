#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359
#define TWO_PI 6.28318530718

uniform vec2 u_resolution;
uniform float u_time;


/// MATRIX TRANSFORMS

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

///// SHAPES /////

//// DIST FIELDS ////

float shapeDistField(vec2 st, float sides){
// Reference to
// http://thndl.com/square-shaped-shaders.html

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

//// CIRCLES ////

float circle(in vec2 st, in float radius, in float smoothing){
    vec2 center = st-vec2(0.5);
    return 1.-smoothstep(radius-(radius*smoothing),
                         radius+(radius*smoothing),
                         dot(center,center)*2.0);
}

float wobbleCircle(in vec2 st, in float radius, in float smoothing, in float offset){
	float pct = 0.;

	//middle circle
	pct = circle(st, radius, smoothing);

	//2nd circle
	pct += circle(vec2(st.x,st.y + offset), radius*(1.-offset*2.), smoothing);

	//3rd circle
	pct += circle(vec2(st.x + offset*0.5,st.y), radius*(1.-offset*2.), smoothing);

	//4th circle
	pct += circle(vec2(st.x - offset, st.y-offset*0.2), radius*(1.-offset*3.), smoothing);

	return clamp(pct, 0., 1.);
}

float wobbleCircleOutline(in vec2 st, in float radius, in float smoothing, in float offset, in float thickness){

	float pct = 0.;
	pct = wobbleCircle(st, radius+thickness*0.5, smoothing, offset);
	pct -= wobbleCircle(st, radius-thickness*0.5, smoothing, offset);
	return pct;
}


//// TRIANGLES ////

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


//// QUADS ////

float diamond(vec2 st, float smoothing, vec2 scalar){
  float radius = 0.5;
  vec3 pos = vec3(st, 1.);
  pos = translate(vec2(-.5),pos);
  pos = scale(scalar,pos);
  pos = translate(vec2(.5),pos);
  float pct = squeezedEquilTriangle(vec2(pos.x,pos.y-.2765), radius, smoothing, vec2(1.,2.26));
  pos = translate(vec2(-.5), pos);
  pos = rotate(PI,pos);
  pos = translate(vec2(.5,.446), pos);
  pct += squeezedEquilTriangle(pos.xy, radius, smoothing, vec2(1.,1.13));
  return pct;
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


//// MORE ////

float squeezeBox(vec2 st, vec2 size, float smoothing, float stretch){
  float pct = trapezoid(st, size, smoothing, stretch);
  st.x = 1. - st.x; //invert axis
  pct += trapezoid(st, size, smoothing, stretch);
  return pct;
}

float chevron(vec2 st, vec2 shrink){
  vec3 pos = vec3(st, 1.);
  pos = translate(vec2(-0.5),pos);
  pos = scale(vec2(0.45)*shrink,pos);
  pos = translate(vec2(0.5),pos);

  vec2 size = vec2(0.25);
  float smoothing = 0.001;
  float stretch = 0.5;

  vec3 pos1 = vec3(pos.xy, 1.);
  pos1 = translate(vec2(0.114,0.0),pos1);
  pos1 = translate(vec2(-0.5), pos1);
  pos1 = scale(vec2(2.),pos1);
  pos1 = rotate(PI*.37, pos1);
  pos1 = translate(vec2(0.5), pos1);
  float pct = trapezoid(pos1.xy, size, smoothing, stretch);
  vec3 pos2 = vec3(pos.xy, 1.);
  pos2.x = 1.-pos2.x;
  pos2 = translate(vec2(0.114,0.0),pos2);
  pos2 = translate(vec2(-0.5), pos2);
  pos2 = scale(vec2(2.),pos2);
  pos2 = rotate(PI*.37, pos2);
  pos2 = translate(vec2(0.5), pos2);
  pct += trapezoid(pos2.xy, size, smoothing, stretch);

  return pct;
}

float cross(vec2 st, vec2 size, vec2 intersection, float smoothing){
    vec2 pos = st;
    pos.y = st.y - intersection.y;
    float horizBox = box(pos, vec2(size.x,size.y), smoothing);
    pos = st;
    pos.x = st.x - intersection.x;
    float vertBox = box(pos, vec2(size.y,size.x), smoothing);

    return vertBox + horizBox;
}




///// LINES /////

//plot a line on y using value 0-1f
float plot(vec2 st, float pct, float smoothing){
	return smoothstep(pct-smoothing, pct, st.y) -
		smoothstep(pct, pct+smoothing, st.y);
		//0.05 controls width of line

}

///CUBIC BEZIER

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

//// BEZIER SQUIGGLY WAVE ////

float squiggle(vec2 st, float mag, float smoothing){

  st *= 10.;
  float y = cubicBezier(st.x, 1.-mag, mag, mag, 1.-mag);
  for (int i=1; i < 10; i+=2){
    y -= cubicBezier(st.x-float(i), mag, 1.-mag, 1.-mag, mag);
    y += cubicBezier(st.x-float(i+1), 1.-mag, mag, mag, 1.-mag);
  }
  y *= .1;
  st *= .1;
  y = plot(st, y, smoothing);
  return y;
}


//// PATTERNS ////

vec2 tile(in vec2 st, in vec2 zoom, out vec2 st_index){

  st *= zoom; // scale up by x and y

  st_index = floor(st); // remove decimals for tile indices

  return fract(st); // replicate .0-1. tile
}

vec2 brickTile(in vec2 st, in vec2 zoom, in vec2 offsetMod, in vec2 offset, out vec2 st_index){

  st *= zoom; // scale up by x and y

  st_index = floor(st); // remove decimals for tile indices

  st.x -= step(1., mod(st.y, offsetMod.y)) * offset.y;
  st.y -= step(1., mod(st.x, offsetMod.x)) * offset.x;

  return fract(st); // replicate .0-1. tile
}

void main(){
    vec2 st = gl_FragCoord.xy/u_resolution.xy;

    st -= 0.2;

    vec2 st_index;
    vec2 st_tile = tile(st, vec2(11.,21.), st_index);

    //vec2 st_tile = brickTile(st, vec2(11.), vec2(2.), vec2(sin(u_time)*0.1,cos(u_time)*0.1), st_index);

    st_tile.x -= step(1., mod(st_index.y, 2.)) * cos(u_time)*0.1;
    st_tile.y -= step(1., mod(st_index.x, 2.)) * sin(u_time)*0.1;

    float offset = sin(u_time+mod(st_index.x, 2.3))*.04*mod(st_index.y,2.8); //circle distortion

    float dots = 0.;

    st_tile *= vec2(2.,1);

    //if (step(1., mod(st)))
    
    //if ((mod(st_index.x, 2.) == 0.) && (mod(st_index.y, 2.) == 0.)){
      vec3 tilePos = vec3(st_tile, 1.);
      //tilePos = translate()
      dots = wobbleCircle(st_tile,0.01,2.5,offset*.4);
    //}

    // add some "randomness"
    float pulse = sin(u_time*0.4 + 100. * (st_index.x+1.8) * 2. / (st_index.y+1.) * 0.5) * 0.5 + 0.5;

    vec3 color = vec3(0.);
    //color.rg = st_tile.xy;

    color += vec3(dots * pulse);

    gl_FragColor = vec4( color, 1.0);
}