#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    numRows = 9;
    numColumns = 17;
    dispState = 2;
    
    // Chase colors
    CGreen = ofColor(120, 190, 32);
    COrange = ofColor(199, 108, 61);
    
    // display vars mockup
    marginX = 0;
    marginY = 0;
    dispWidth = 90;
    dispHeight = 90;
    vidOriginX = 10;
    vidOriginY = 10;
    manualSwitch = true;
    
    // lock time variables
    checkInDuration = 45000;
    pickUpDuration = 45000;
    
    string tempIPs[] = {"192.168.0.121", "192.168.0.153", "192.168.0.107", "192.168.0.104", "192.168.0.180"};
    
    // temp initialize lockers
    for(int col_i = 0 ; col_i < numColumns ; ++col_i){
        vector<Locker* > tempLockerCol;
        for(int row_i = 0; row_i < numRows; ++row_i){
            Locker* tempLocker = new Locker(col_i, row_i, "192.168.0.126");
            tempLockerCol.push_back(tempLocker);
            
            if(col_i == 0 && row_i < 5){
                tempLocker->setIP(tempIPs[row_i]);
            }
            else if(col_i == 1 && row_i == 0){
                tempLocker->setIP("192.168.0.82");
            }
            else {
                tempLocker->setIP("192.168.0.144");
            }
        }
        
        lockers.push_back(tempLockerCol);
    }
    
    // final lockers
//    for(int col_i = 0 ; col_i < numColumns ; ++col_i){
//        vector<Locker* > tempLockerCol;
//        for(int row_i = 0; row_i < numRows; ++row_i){
//            Locker* tempLocker = new Locker(col_i, row_i, piIPAddresses[numColumns]);
//            tempLockerCol.push_back(tempLocker);
//        }
//        
//        lockers.push_back(tempLockerCol);
//    }
    

    for(int i = 0; i < lockers.size(); ++i){
        for(int j = 0; j < lockers[i].size(); ++j){
            ofLog() << lockers[i][j]->ipAddress << endl;
        }
    }
    
    sourceVid.loadMovie("movies/fingers.mov");
    sourceVid.play();
//    vidOriginX = sourceVid.getWidth();
    
    // camera feed
    sourceCam.setDeviceID(0);
    sourceCam.setDesiredFrameRate(30);
    sourceCam.initGrabber(160, 120);
//    vidOriginX = sourceCam.getWidth();
    
    ofLog() << ofGetSystemTime() << endl;
    
    // initialize sender
    sender.setup(HOST, 54321);
    
    // initalize receiver
    receiver.setup(LISTEN_PORT);
    
    ofSetColor(255,255,255);
    ofBackground(0,0,0);
    ofSetFrameRate(10);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // process through received OSC messages
    processMessages();
    
    long long currentTime = ofGetSystemTime();
    // in milliseconds
    
    if(manualSwitch){
        
    }
    else {
        int switcher = currentTime % 10000; // change every 100 seconds
        if(switcher < 100){
            ++dispState;
            dispState = dispState % 6;
        }
    }
    
    switch(dispState){
        case 0:
            updateCam();
            break;
        case 1:
            updateVid();
            break;
        case 2:
            colorSweep();
            break;
        case 3:
            concentrics(8, 4);
            break;
        case 4:
            colorStripes();
            break;
        case 5:
            randomSet();
            break;
    }
    
    // Possibly override of pixel data functions
    displayOpen();
    
    // Send OSC color data
    sendMessages();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255,255,255);
//    sourceVid.draw(0, 0, sourceVid.getWidth(), sourceVid.getHeight());
    
    switch(dispState){
        case 0:
            sourceCam.draw(0, 0);
            break;
        case 1:
            sourceVid.draw(0, 0);
            break;
    }
    
    ofImage tempImg;
    tempImg.setFromPixels(sourceVid.getPixelsRef());
    for(int i = 0; i < lockers.size(); ++i){
        for(int j = 0; j < lockers[i].size(); ++j){
            ofSetColor(lockers[i][j]->getColor());
            ofRect(i*dispWidth+marginX+vidOriginX, j*dispHeight+marginY+vidOriginY, dispWidth, dispHeight);
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case OF_KEY_LEFT:
            dispState = dispState - 1;
            if (dispState < 0){
                dispState = 5;
            }
            break;
        case OF_KEY_RIGHT:
            dispState = (dispState + 1) % 6;
            break;
            
        case 'm':
        case 'M':
            manualSwitch = !manualSwitch;
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    ofLog() << dragInfo.files[0] << endl;
    string fileExt = dragInfo.files[0].substr(dragInfo.files[0].find_last_of(".") + 1);
    
    
    if( fileExt == "mov" || fileExt == "MOV" || fileExt == "mpeg" || fileExt == "mpg" || fileExt == "mp4")
    {
        sourceVid.loadMovie(dragInfo.files[0]);
    }
    else {
        ofLog() << "not a usable file" << endl;
    }
}

// Camera functions

void ofApp::setupCam() {
    
}

void ofApp::updateCam() {

    sourceCam.update();
    ofImage tempImg;
    
    tempImg.setFromPixels(sourceCam.getPixelsRef());
    pixellate(tempImg);
}

void ofApp::drawCam() {
    
    
}


// Video functions

void ofApp::setupVid() {
    
}

void ofApp::updateVid() {
    
    sourceVid.update();
    ofImage tempImg;
    
    tempImg.setFromPixels(sourceVid.getPixelsRef());
    pixellate(tempImg);
}

void ofApp::drawVid() {
    
}

void ofApp::pixellate(ofImage tempImg){
    
    for(int i = 0; i < lockers.size(); ++i){
        for(int j = 0; j < lockers[i].size(); ++j){
            
            ofColor tempColor = tempImg.getColor(i*10, j*10);
            
            ofImage croppedImg;
            
            croppedImg.cropFrom(tempImg, (tempImg.width / lockers.size() ) * i, (tempImg.height / lockers[i].size() ) * j, tempImg.width / lockers.size(), tempImg.height / lockers[i].size());
            ofxCvColorImage colorImg;
            colorImg.setFromPixels(croppedImg.getPixelsRef());
            
            IplImage* iplImg = colorImg.getCvImage();
            CvScalar mean; CvScalar stdv;
            cvAvgSdv(iplImg, &mean, &stdv);
            
            tempColor.r = (int) mean.val[0];
            tempColor.g = (int) mean.val[1];
            tempColor.b = (int) mean.val[2];
            
            lockers[i][j]->setColor(tempColor);
        }
    }
    
}

// algorithmic shows

void ofApp::colorSweep() {
    
    long long currentTime = ofGetSystemTime();
    
    int switcher = (currentTime % 2550) / 10;
    
    // does a color wheel
    for(int i = 0; i < lockers.size(); ++i){
        for(int j = 0; j < lockers[i].size(); ++j){
            
            lockers[i][j]->setColor(wheel((j*lockers.size()+i+switcher)%255));
        }
    }
}

void ofApp::concentrics(int _x, int _y) {
    
    long long currentTime = ofGetSystemTime();
    
    ofColor colors[4] = {ofColor(1,121,193), ofColor(185, 224, 247), ofColor(0,57,250), ofColor(0, 65, 94)};
    
    
    int switcher = (currentTime % 10000) / 200;
    
    for(int i = 0; i < lockers.size(); ++i) {
        for(int j = 0; j < lockers[i].size(); ++j){
            lockers[i][j]->setColor(colors[(abs(i - _x) + abs(j - _y) + 50 - switcher) % 4]);
//            if( ((abs(i - _x) +abs(j - _y) + switcher) % 2 == 0)){
//                lockers[i][j]->setColor(ofColor(255,255,255));
//            }
//            else{
//                lockers[i][j]->setColor(ofColor(0,0,0));
//            }
            
        }
    }
}

void ofApp::mosaic() {
    
}

void ofApp::colorStripes() {
    
    long long currentTime = ofGetSystemTime();
    
    ofColor colors[4] = {ofColor(1,121,193), ofColor(185, 224, 247), ofColor(0,57,250), ofColor(0, 65, 94)};
    
    int switcher = (currentTime % 10000) / 200;
    
    // horizontal sweep
    //    for(int i = 0; i < lockers.size(); ++i) {
    //        for(int j = 0; j < lockers[i].size(); ++j){
    //            lockers[i][j]->setColor(colors[(switcher+i)%4]);
    //        }
    //    }
    
    // vertical sweep
    for(int i = 0; i < lockers.size(); ++i) {
        for(int j = 0; j < lockers[i].size(); ++j){
            lockers[i][j]->setColor(colors[(lockers[i].size() - j + switcher)%4]);
        }
    }
}

void ofApp::randomSet(){
    long long currentTime = ofGetSystemTime();
    
    ofColor colors[4] = {ofColor(1,121,193), ofColor(185, 224, 247), ofColor(0,57,250), ofColor(0, 65, 94)};
    
    int switcher = (currentTime % 300);
    
    ofLog() << "switcher is: " << ofToString(switcher) << endl;
    
    for(int i = 0; i < lockers.size(); ++i) {
        for(int j = 0; j < lockers[i].size(); ++j){
            if(switcher   >  200){
                lockers[i][j]->setColor(colors[(rand())%4]);
            }
            
        }
    }
}

ofColor ofApp::wheel(int WheelPos) {
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
        return ofColor(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if(WheelPos < 170) {
        WheelPos -= 85;
        return ofColor(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return ofColor(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void ofApp::displayOpen() {

    long long currentTime = ofGetSystemTime();

    for(int i = 0; i < lockers.size(); ++i){
        for(int j = 0; j < lockers[i].size(); ++j){
            
            if(lockers[i][j]->currentState == 1){
                
                ofLog() << "currentTime " << ofToString(currentTime) << ", lockmillis " << ofToString(lockers[i][j]->lockMillis) << " and the difference is " << ofToString(currentTime - lockers[i][j]->lockMillis) << endl;
                if(currentTime - lockers[i][j]->lockMillis > checkInDuration){
                    
                    ofLog() << "locker " << ofToString(i) << ", " << ofToString(j) << " is now closing" << endl;
                    lockers[i][j]->currentState = 0;
                    ofxOscMessage lock;
                    sender.setup(lockers[i][j]->ipAddress, PORT);
                    lock.setAddress("/lock");
                    lock.addIntArg(j);
                    lock.addIntArg(1); // lock
                    sender.sendMessage(lock);
                }
                else{
                    ofColor tempColor;
                    
                    int switcher = (currentTime % 500);
                    
                    if (switcher > 250) {
                        tempColor = ofColor(1,121,193);
                        switch(lockers[i][j]->deviceID){
                            case 0:
                                tempColor = ofColor(1,121,193);
                                break;
                            case 1:
                                tempColor = CGreen;
                                break;
                            case 2:
                                tempColor = COrange;
                                break;
                            default:
                                break;
                        }
                    }
                    else {
                        tempColor = ofColor(185, 224, 247);
                    }
                    
                    ofLog() << "locker " << ofToString(i) << ", " << ofToString(j) << " is open" << endl;
                    lockers[i][j]->setColor(tempColor);
                    
                    ofxOscMessage lock;
                    sender.setup(lockers[i][j]->ipAddress, PORT);
                    lock.setAddress("/lock");
                    lock.addIntArg(j);
                    lock.addIntArg(0); //unlock
                    sender.sendMessage(lock);
                }
                
            }
        }
    }
}

void ofApp::processMessages() {
    
    // OSC Receiving
    while(receiver.hasWaitingMessages()){
        // get the next message
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        int row = m.getArgAsInt32(0);
        int col = m.getArgAsInt32(1);
        int state = m.getArgAsInt32(2);
        int deviceID = m.getArgAsInt32(3);
        
        ofLog() << "message with address: " << m.getAddress() << " " << m.getArgAsInt32(0) << " " << m.getArgAsInt32(1) << " " << m.getArgAsInt32(2) << " " << m.getArgAsInt32(3) << endl;
        
//        int row = m.getArgAsInt32(0);
//        int col = m.getArgAsInt32(1);
//        int state = m.getArgAsInt32(2);
        
        row = 1;
        col = 0;
//        if(state == 1){
        
            lockers[col][row]->lockMillis = ofGetSystemTime();
            lockers[col][row]->currentState = 1;
            lockers[col][row]->deviceID = deviceID;
//        }
        
    }
}

void ofApp::sendMessages() {
    
    // send OSC Messages
    for(int i = 0; i < numColumns; ++i){
        for(int j = 0; j < numRows; ++j){
            // temp
            
            // send color state
            ofxOscMessage m;
            ofColor tempColor = lockers[i][j]->getColor();
//            ofLog() << "row " << ofToString(i) << " col " << ofToString(j) << endl;
            
            if(i == 1){
                lockers[i][j]->setIP("192.168.0.82");
                sender.setup(lockers[i][j]->ipAddress, 9998);
                m.setAddress("/led");
                m.addIntArg(j);
                m.addIntArg(tempColor.r);
                m.addIntArg(tempColor.g);
                m.addIntArg(tempColor.b);
                sender.sendMessage(m);
            }
            
            else {
                sender.setup(lockers[i][j]->ipAddress, PORT);
                m.setAddress("/led");
                m.addIntArg(84);
                m.addIntArg(tempColor.r);
                m.addIntArg(tempColor.g);
                m.addIntArg(tempColor.b);
                sender.sendMessage(m);
            }
            
            
            
            
            // final
            /*
            ofxOscMessage m;
            ofColor tempColor = lockers[i][j]->getColor();
            //            ofLog() << "row " << ofToString(i) << " col " << ofToString(j) << endl;
            sender.setup(lockers[i][j]->ipAddress, PORT);
            m.setAddress("/" + ofToString(j));
            m.addIntArg(tempColor.r);
            m.addIntArg(tempColor.g);
            m.addIntArg(tempColor.b);
            sender.sendMessage(m);
        
             */
        }
    }
}
