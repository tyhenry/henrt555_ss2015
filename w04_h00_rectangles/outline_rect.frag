#ifdef GL_ES
precision mediump float;
#endif

#define PI 3.14159265359

uniform vec2 u_resolution;
uniform float u_time;

float rect(vec2 st, vec2 center, float w, float h, float smoothing){

	vec4 edges = vec4(center.x - w*0.5, center.y - h*0.5, center.x + w*0.5, center.y + h*0.5);
	
	float left = smoothstep(edges.x-smoothing,edges.x, st.x);
	float bottom = smoothstep(edges.y-smoothing,edges.y, st.y);
	float right = 1.0 - smoothstep(edges.z,edges.z+smoothing, st.x); //invert result
	float top = 1.0 - smoothstep(edges.w,edges.w+smoothing, st.y); //invert result

	return left * bottom * right * top;
}


float rectOutline(vec2 st, vec2 center, float w, float h, float smoothing, float lineWidth){

	float rectOutside = rect(st, center, w + lineWidth, h + lineWidth, smoothing);
	float rectInside = 1.0 - rect(st, center, w - lineWidth, h - lineWidth, smoothing); //invert colors

	return rectOutside * rectInside;

}

void main() {

	vec2 st = gl_FragCoord.xy/u_resolution.xy; //normalize coords
	vec3 color = vec3(0.0);

	float pct = rectOutline(st, vec2(0.6,0.6), 0.4, 0.3, 0.02, 0.02); //st, center.xy, w, h, smoothing

	color = vec3(pct);

	gl_FragColor = vec4(color,1.0);
}