//
//  easyShader.hpp
//  shaderFinalPrototype_ofxSyphon
//
//  Created by Tyler on 12/2/15.
//
//

#pragma once
#include "ofMain.h"
//#include "ofxGui.h"
#include "ofxDatGui.h"

class EasyShader {
public:
    
    // constructors
    EasyShader();
    EasyShader(string _path, ofVec2f _pos, ofVec2f _size);
    EasyShader(string _path);
    //EasyShader(float _pos, float _size);
    
    void update();
    void setTimeSpeed(float _timeSpeed);
    void updateFbo(); //must be called during draw loop
    void setTexture(ofTexture _tex, int nTex); //texture reference
    void draw();
    void draw3D();
    void drawPlane();
    void drawGui();
    void updateGui();
    
    void setPos(float x, float y);
    void setCPos(float x, float y); //set 2D center pos
    void setZPos(float x, float y); //set 3D position
    
    ofVec2f contains(float x, float y);
    // if x,y is within pos/size
    // returns point relative to pos
    // otherwise returns (-1,-1)
    
    bool insideInBox(float x, float y);
    bool insideOutBox(float x, float y);
    
    void resetPos();
    void resetSize();
    void scale(float scalar);
    
    void select();
    void deselect();
    bool isSelected();
    
    int nTexInputs = 0; //# of texture inputs shader wants
    int currentTexN = 0; //texture # that should be modified
    int getIncTexN(); //returns texture # that should be filled in by patch
    
    ofVec2f size;
    ofVec2f pos; //2D space
    ofVec3f zPos; //3D space, center - not working correctly right now
    ofVec2f origPos;
    ofVec2f origSize;
    
    vector<ofTexture> textures;
    ofFbo output;
    ofRectangle inBox, outBox;
    bool bOut = false;
    bool bIn = false;
    

    
    bool selected = false;
    
private:
    
    void init(string _path, ofVec2f _pos, ofVec2f _size); //common constructor code
    void findUniforms();
    
    void on2dPadEvent(ofxDatGui2dPadEvent e);
    
    string trim(const string& str,
                const string& whitespace = " \t")
    {
        const auto strBegin = str.find_first_not_of(whitespace);
        if (strBegin == string::npos)
            return ""; // no content
        
        const auto strEnd = str.find_last_not_of(whitespace);
        const auto strRange = strEnd - strBegin;
        
        return str.substr(strBegin, strRange);
        
    }
    
    string shaderPath;
    ofShader shader;
    ofBuffer shaderSource;
    
    struct Uniform {
        string type;
        string name;
    };
    vector<Uniform> uniforms;
    
    
    //gui
    
    ofxDatGui* gui;
    ofxDatGuiSlider* timeSpeed;
    ofxDatGuiSlider* mouseXSlider;
    ofxDatGuiSlider* mouseYSlider;
    //ofxDatGui2dPad* mousePad;
    float timeSpeedMapped = 1;
    float uMouseX = 0;
    float uMouseY = 0;

};