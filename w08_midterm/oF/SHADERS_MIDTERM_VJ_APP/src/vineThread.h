//
//  vineThread.h
//  lovid_korea_test_10_vine_threaded
//
//  Created by Tyler Henry on 10/18/15.
//
//

#pragma once

#include "ofThread.h"
#include "ofxJSON.h"

class VineThread: public ofThread {
    
public:

    //Create VineThread object and initialize member variables
    VineThread(): hashtag(""), records(0)
    {
    
    }
    
    /// Start the thread.
    void start()
    {
        // Mutex blocking is set to true by default
        // It is rare that one would want to use startThread(false).
        startThread();
    }
    
    /// Signal the thread to stop.  After calling this method,
    /// isThreadRunning() will return false and the while loop will stop
    /// next time it has the chance to.
    void stop(){
        stopThread();
    }
    
    /// Our implementation of threadedFunction.
    void threadedFunction()
    {
        while(isThreadRunning())
        {
            // Attempt to lock the mutex.  If blocking is turned on,
            if(lock())
            {
                
                /*------------------------------------------
                // Check Vine API for new hashtagged videos
                ------------------------------------------*/
                
                /*----------------------------------------
                // ofxJSON - load vines
                ----------------------------------------*/
                
                string url = "https://api.vineapp.com/timelines/tags/" + hashtag;
                bool parsingSuccessful = json.open(url); // get/parse the JSON
                
                ofLogVerbose("[JSON]") << url;
                
                if (parsingSuccessful)
                {
                    ofLogVerbose("[JSON]") << json.getRawString(true); // print JSON
                    
                } else {
                    
                    ofLogNotice("[JSON]") << "Failed to parse JSON."; // notify of API failure
                    
                    return; // skip searching the JSON records
                }
                
                int nRecords = json["data"]["records"].size(); // new number of records - check against 'records'
                
                if (nRecords > records) { // check # records against what we have already
                    
                    ofLogVerbose("Check Vine") << (nRecords - records) << " new Vine API records, downloading...";
                    
                    // loop through new records and download
                    
                    int numNew = nRecords - records; //new records are at beginning of JSON
                    for (Json::ArrayIndex i = 0; i < numNew; ++i){
                        
                        vidUrl  = json["data"]["records"][i]["videoUrl"].asString(); // get video url
                        string vidPath = "vine" + ofToString(records + i) + ".mp4"; // set video file path
                        resp = ofSaveURLTo(vidUrl, vidPath); // download video to file
                        
                        ofLogVerbose("Vine video url: ") << vidUrl;
                        ofLogVerbose("Vine video path: ") << vidPath << endl;
                        
                        records = nRecords; // save new # of records
                    }
                    
                } else {
                    
                    ofLogVerbose("Check Vine") << "no new records, we've got " << records;
                    
                }
                
                
                // Unlock the mutex.  This is only
                // called if lock() returned true above.
                unlock();
                
                // Sleep for 1 second.
                sleep(1000);
            }
            else
            {
                // If we reach this else statement, it means that we could not
                // lock our mutex, and so we do not need to call unlock().
                // Calling unlock without locking will lead to problems.
                ofLogWarning("[VineThread] threadedFunction()") << "Unable to lock mutex.";
            }
        }
    }
    
    string hashtag;
    int records;
    
private:
    
    ofxJSONElement json;
    
    string vidUrl;
    ofHttpResponse resp;
    
    
};
