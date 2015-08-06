//
//  Locker.cpp
//  LockerSignifier
//
//  Created by BrooklynResearch on 7/20/15.
//
//

#include "Locker.h"

Locker::Locker(){
    
}

Locker::Locker(int _row, int _col, string _ipAddress)
{
    column = _col;
    row = _row;
    ipAddress = _ipAddress;
    currentState = 0;
    
}

Locker::~Locker(){

}

void Locker::setVars(int _row, int _col, string _ipAddress) {
    
    column = _col;
    row = _row;
    ipAddress = _ipAddress;
    
}

void Locker::setColumn(int _col){
    column = _col;
}

void Locker::setRow(int _row){
    row = _row;
}

void Locker::setIP(string _ipAddress) {
    ipAddress = _ipAddress;
}

void Locker::setPort(int _portNum) {
    portNum = _portNum;
}

void Locker::setColor(ofColor _setColor) {
    currentColor = _setColor;
}

string Locker::getIP() {
    return ipAddress;
}

ofColor Locker::getColor() {
    return currentColor;
}