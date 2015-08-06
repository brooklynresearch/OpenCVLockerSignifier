//
//  Locker.h
//  LockerSignifier
//
//  Created by BrooklynResearch on 7/20/15.
//
//

#ifndef __LockerSignifier__Locker__
#define __LockerSignifier__Locker__

//#include <stdio.h>
#include "ofMain.h"

#endif /* defined(__LockerSignifier__Locker__) */

class Locker {
    
    public:
    
        Locker();
        Locker(int _col, int _row, string _ipAddress);
        ~Locker();
    
        void setVars(int _col, int _row, string _ipAddress);
        void setColumn(int _col);
        void setRow(int _row);
        void setIP(string _ipAddress);
        void setPort(int _portNum);
        void setColor(ofColor _setColor);
    
        string getIP();
        ofColor getColor();
    
        int column;
        int row;
        bool oversized;
        int currentState;
        string ipAddress;
        int portNum;
        ofColor currentColor;
    
        long long lockMillis;
        long long unlockMillis;
    
        enum states {DEFAULT, LOCK, UNLOCK};
};