#!/bin/bash

 install_name_tool -change @rpath/libboost_system.dylib  /Users/dkos/Public/cpplib/lib/libboost_system.dylib ocr
 install_name_tool -change @rpath/libboost_filesystem.dylib /Users/dkos/Public/cpplib/lib/libboost_filesystem.dylib ocr
 install_name_tool -change @rpath/libopencv_core.4.4.dylib /Users/dkos/Public/cpplib/lib/libopencv_core.dylib ocr
 install_name_tool -change @rpath/libopencv_highgui.4.4.dylib /Users/dkos/Public/cpplib/lib/libopencv_highgui.dylib ocr
 install_name_tool -change @rpath/libopencv_imgproc.4.4.dylib /Users/dkos/Public/cpplib/lib/libopencv_imgproc.dylib ocr
 install_name_tool -change @rpath/libopencv_features2d.4.4.dylib /Users/dkos/Public/cpplib/lib/libopencv_features2d.dylib ocr
 install_name_tool -change @rpath/libopencv_imgcodecs.4.4.dylib /Users/dkos/Public/cpplib/lib/libopencv_imgcodecs.dylib ocr
 install_name_tool -change @rpath/libboost_regex.dylib /Users/dkos/Public/cpplib/lib/libboost_regex.dylib ocr
