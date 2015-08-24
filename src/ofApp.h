#pragma once

#include "ofMain.h"
#include "Locker.h"
#include "ofxOsc.h"
#include "ofxOpenCv.h"

#define HOST "localhost"
#define PORT 9998
#define LISTEN_PORT 9998


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void setupCam();
        void updateCam();
        void drawCam();
    
        void setupVid();
        void updateVid();
        void drawVid();
    
        void pixellate(ofImage tempImg);
    
        void colorSweep();
        void concentrics(int _x, int _y);
        void mosaic();
        void colorStripes();
        void randomSet();
        void randGradientColumn();
    
        ofColor wheel(int WheelPos);
    
        void displayOpen();
    
        void processMessages();
        void sendMessages();
		
        int numRows;
        int numColumns;
    
        // pi vars
        string piIPAddresses[17] = {"192.168.1.100", "192.168.1.101", "192.168.1.102", "192.168.1.103", "192.168.1.104", "192.168.1.105", "192.168.1.106", "192.168.1.107", "192.168.1.108", "192.168.1.109", "192.168.1.110", "192.168.1.111", "192.168.1.112", "192.168.1.113", "192.168.1.114", "192.168.1.115", "192.168.1.116"};
        string doubleLockers[4][4];
    
        // display variables
        int dispState;
        int numStates;
        bool manualSwitch;
        int checkInDuration;
        int pickUpDuration;
        long long stateChangeMillis;
    
        vector< vector < Locker* > > lockers;
        vector< ofPoint > unlocked;
        ofVideoPlayer sourceVid;
        ofVideoGrabber sourceCam;
    
        // Chase colors
        ofColor CGreen;
        ofColor COrange;
        ofColor CYellow;
    
        // display vars
        int marginX;
        int marginY;
        int dispWidth;
        int dispHeight;
        int vidOriginX;
        int vidOriginY;
    
        // Request listener
    
        // OSC Sender
        ofxOscSender sender;
    
        // OSC Receiver
        ofxOscReceiver receiver;
};
