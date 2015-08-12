#!/bin/sh
if [ $(ps ax | grep -v grep | grep "OpenCVLockerSignifier.app" | wc -l) -eq 0 ]
then
       open /Users/stillness/Documents/of_v0.8.4_osx_release/apps/myApps/OpenCVLockerSignifier/bin/OpenCVLockerSignifier.app
fi

if [ $(ps ax | grep -v grep | grep "oschttp.py" | wc -l) -eq 0 ]
then
       python /Users/stillness/Documents/of_v0.8.4_osx_release/apps/myApps/OpenCVLockerSignifier/python/lockerServe/oschttp.py 
fi