#pragma once

#include "ofMain.h"
#include "LoVid.h"
#include "ofxJSON.h"
#include "ofxLovid.h"
#include "ofxPatterns.h"
#include "vineThread.h"
#include "ofxKinect.h"
#include "ofxOpenCv.h"

// debug mode on? (switch in-app with 'd')
#define DEBUG false

// MAIN MODES - sensors on? otherwise, use keyboard/mouse
#define ARDUINO_ON false
#define KINECT_ON false

#define SOUND_ON true
// sound .aif files must be in bin/data

// download vine files on start?
#define VINE_START false

// do regular vine updating? (switch with 'v')
#define VINE_UPDATE false

// vine hashtag to load
#define HASHTAG "lovidkorea3"

// # of seconds to wait before
// trying to load new video file
// in bin/data after auto-detection
#define LOADWAIT 10

// kinect depth thresholding
#define KINECT_NEAR_THRESHOLD 255
#define KINECT_FAR_THRESHOLD 217

//ARDUINO

// check this against arduino ide
#define ARDUINO_SERIAL_CAP "/dev/tty.usbmodem1451"
#define ARDUINO_SERIAL_BB "/dev/tty.usbmodem141411"

// total number of sensors/headers
#define NUM_SENSORS 15



class ofApp : public ofBaseApp{
    
public:
    
    void setup();
    void update();
    void draw();
    void exit();
    
    // kinect
    //-----------------------------
    void kinectSetup();
    void kinectUpdate();
    void kinectDraw(ofVec2f pos);
    
    // arduino
    //-----------------------------
    void arduinoSetup();
    void arduinoUpdate(int _serial);
    
    // sounds
    //-----------------------------
    void soundSetup(); // loads sound files
    void soundUpdate(); // updates effects
    void soundSwitch(); // called when beam break
    
    
    // video effect adjustment
    //-----------------------------
    void processSensorVals(); //processes current sensorVals[] array
    void adjustEffects(char _head); //adjusts effects for 'F'-'N' headers
    
    // add or remove video from draw
    //-------------------------------------
    void addDrawVid(); // move a qVids[] vid to dVids[] and set it to .play()
    void stopDrawVid(); // .stop() oldest dVids[] vid and move it to qVids[]
    
    // check for or load new HDD files
    //-------------------------------------
    void checkForNewFile(); // checks folder for new videos
    void loadNewFile(); // loads new video into noDrawVids vector
    
    // change selected video & draw order
    //-------------------------------------
    void drawSelection(int selection); // draws a box around the selected video
    void newDrawTop(int selection); // draw vid on top
    void eraseDrawOrder(int selection); // erase selection from draw order
    
    bool isLogVerbose(); // utility to check ofLogLevel for debug
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    bool debug = DEBUG;
    
    /*--------------------
     // Videos
     --------------------*/
    
    vector<string> vidPaths; // paths of all vids
    vector<LoVid> qVids; // queued vids
    vector<LoVid> dVids; // drawing vids
    vector<int> dVidsOrder; // draw order for dVids
    
    /*--------------------
     // Shaders
     --------------------*/
    
    ofxLovid fxLovid;
    ofxPatterns fxPatterns;
    
    
    /*--------------------
     // JSON - Vine
     --------------------*/
    
    ofxJSONElement json;
    int records;
    
    string hashtag = HASHTAG;
    string vidUrl;
    ofHttpResponse resp;
    
    
    /*--------------------
     // Kinect
     --------------------*/
    
    ofxKinect kinect;
    
    ofxCvColorImage kinectColorImg; //color image from kinect
    
    ofxCvGrayscaleImage kinectGrayImage; // grayscale depth image
    ofxCvGrayscaleImage kinectThreshNear; // the near thresholded image
    ofxCvGrayscaleImage kinectThreshFar; // the far thresholded image
    
    ofxCvContourFinder contourFinder;
    
    int nearThreshold = KINECT_NEAR_THRESHOLD; // 0-255 grayscale
    int farThreshold = KINECT_FAR_THRESHOLD; // 0-255 grayscale
    
    bool drawKinect = false;
    bool kinectCentroid = false;
    bool kinectGotCentroid = false;
    
    ofVec2f centroid = ofVec2f(0.0,0.0);
    
    
    /*--------------------
     // Arduino
     --------------------*/
    
    ofSerial serial_cap;
    ofSerial serial_bb;
    int	nBytesRead;	// how much did we read?
    
    char arduinoHeader = 'X'; // currrent header - X is default/no data
    int arduinoValue = 0; // current value - 0 is default/no data
    // values:
    // IR CODES: 2000 (A1), 3000 (B2), 4000 (C3), 5000 (D4), 6000 (E5), 65000 (beam break)
    // Capacitive Sensors: 200 (min) - 1500 (max)
    
    int sensorVals [NUM_SENSORS] = {}; // stores latest sensor value
    bool beamBroken [5] = {false,false,false,false,false}; // stores latest beam break state
    
    
    /*--------------------
     // Vine Thread
     --------------------*/
    
    VineThread vineThread;
    bool bVineThread = VINE_UPDATE;
    
    
    /*--------------------
     // Directory Operations
     --------------------*/
    
    ofDirectory dir;
    int dirSize; // previous directory size (# of files)
    vector<ofFile> files;
    bool bNewVid = false; // marker to indicate there's a new video file to load
    string newVidPath;
    float loadVidWait = 0; // countdown in seconds to wait until loading new file (if file is being copied
    float foundVidTime = 0; // time at which video was found
    
    
    /*--------------------
     //FX adjustments
     --------------------*/
    
    int vidSelect = 0; // dVids[] selection for effect
    int selectDrawCnt = 0; // countdown for drawSelection
    int fxSelect = 1; // effect adjustment selector
    // 1 = size
    // 2 = tint
    // 3 = saturation
    // 4 = contrast
    // 5 = pixelation
    // 6 = glitch
    int fxCount = 0;
    
    /*--------------------
     // Pattern Adjustments
     --------------------*/
    int patternSelect = 0; //0-9: 10 patterns
    
    
    /*--------------------
     // Sound
     --------------------*/
    
    vector<ofSoundPlayer> sounds;
    int soundTrack = 0;
    float soundSpeed = 1.0; //1.0 - 2.0, avg video effect
    float soundVolume = 0.5; //0.0 - 1.0 (0.5 normal), # dVids/# dVids + qVids
    
    
};
