#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform float u_time;

float box(in vec2 _st, in vec2 _size){
    _size = vec2(0.5) - _size*0.5;
    vec2 uv = smoothstep(_size,
                        _size+vec2(0.001),
                        _st);
    uv *= smoothstep(_size,
                    _size+vec2(0.001),
                    vec2(1.0)-_st);
    return uv.x*uv.y;
}

float cross(in vec2 _st, float _size){
    return  box(_st, vec2(_size,_size/4.)) + 
            box(_st, vec2(_size/4.,_size));
}


//"wave" shaping function
float waveFlow(float x){

    float y = smoothstep(0.,0.4,x);
    y -= smoothstep(0.6,1.,x);
    return y;
}

//"wave" rotate function
float waveTunnel(float x){

    float y = smoothstep(0.,0.4,x);
    return y;
}



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


void main(){
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    vec3 color = vec3(0.0);

    // Show the coordinates of the space on the background
    // color = vec3(st.x,st.y,0.0);

    vec3 pos = vec3(st,1.0);

    float timeKeeper = fract(u_time * 0.1);
    float scaleAmt = waveFlow(timeKeeper)*0.9+0.01;
    float rotateAmt = waveTunnel(timeKeeper);

    pos = translate(vec2(-.5), pos);
    pos = scale(vec2(scaleAmt*6.), pos);
    pos = rotate(scaleAmt*10., pos);
    pos = translate(vec2(.5), pos);
    pos = translate(vec2(-scaleAmt), pos);




    // Add the shape on the foreground
    color += vec3(cross(pos.xy,0.25));

    gl_FragColor = vec4(color,1.0);
}