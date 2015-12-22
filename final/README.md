#final project

node-based shader vj tool with syphon output for projection mapping

built in openFrameworks 0.9 (with ofxDatGui + ofxSyphon addons)

this is a prototype/work in progress

video walkthrough:
https://youtu.be/rKQ59JQRiaw

this vj app is fully fragment shader based.  it allows you to drag and drop frag shaders into the app window and they will auto-draw to a 2d rectangle.
from there you can easily "patch" shaders to each other by routing input and output textures.
the app parses the shader file and automagically determines any custom uniforms beyond u_resolution, u_time, and u_mouse.  these custom uniforms are then adjustable on the fly through the gui (using normalized values).
the app also determines how many textures the shader supports as inputs so it will allow you to patch 2 or more textures into a single shader, if the shader calls for them.

also included is a set of starter texture modifcation shaders as a first attempt to simulate analog modular video synthesis.  these shaders can extract, adjust, and recombine color information from input textures.


to do:
- add support for custom vec2, vec3 uniforms
- add video and photo input
- fix texture resolution issue
- build full set of analog modular video synthesis shaders
(oscillators w/ various waveforms, filters, )
- improve gui
- selectable texture output to syphon
- multiple views/windows for preloading/adjusting shaders before draw
- support for 3d & in-app projection mapping
- vertex shader support
- bug fixes when modifying "patches" between shaders