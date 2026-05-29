\# NarutoSenki317 Build Notes



\## Engine

\- cocos2d-x 3.17.2

\- Project migrated from cocos2d-x 2.2.6



\## Tested Environment

\- Windows

\- Android Studio

\- NDK: 24.0.8215888

\- CMake: 3.22.1

\- ABI tested: armeabi-v7a



\## Open Project

Open this folder in Android Studio:



proj.android



\## Important

`local.properties` is not committed.

Create it automatically from Android Studio or manually:



sdk.dir=D\\:\\\\sdk



\## Known Migration Fixes

\- Removed Enhance Game Service dependency from Cocos2dxHelper.java

\- Added Android macro in root CMakeLists.txt

\- Guarded desktop GLView include in AppDelegate.cpp

\- Replaced old cocos2dx JNI include path with:

&#x20; platform/android/jni/JniHelper.h

