#pragma once

#include "ofMain.h"
//#include "ofxGui.h"
#include "ofxDatGui.h"
#include "ofxSyphon.h"
#include "easyShader.hpp"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void addShader(string path, ofVec2f pos, ofVec2f size);
    void addShader(string path);
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    vector<EasyShader> shaders;
    
    ofxSyphonServer syphon;
    
    int shaderDrag = -1;
    ofVec2f shaderDragPos = ofVec2f(-1,-1);
    
    int fboDrag = -1;
    
    struct Patch {
        int from;
        int to;
        int texN; //texture number in shader
    };
    vector<Patch> patches;
    
    int selectedShader = -1;
    bool bPatching = false;
    
    bool checkTextures = false;

};
