android-mobile-sdk-harness
==========================

* Install the Android SDK and NDK
* Run ./update.platform.sh to get the latest platform libraries and headers.
* Open an ADT Eclipse workspace, importing this repository as an Android project
* Set the NDK location in the ADT Eclipse workspace to where it was installed (Menu->ADT->Android->NDK)
* Bring up the project menu, from within Android Tools add native support to the project
* Build and debug from within ADT Eclipse
* build.sh can be used to generate the native library if you want to manually package the .apk