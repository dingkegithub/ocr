#!/bin/bash

 install_name_tool -change @rpath/libboost_system.dylib  /Users/dkos/Public/cpplib/lib/libboost_system.dylib bin_ocr
 install_name_tool -change @rpath/libboost_filesystem.dylib /Users/dkos/Public/cpplib/lib/libboost_filesystem.dylib bin_ocr
 install_name_tool -change @rpath/libopencv_core.4.4.dylib /Users/dkos/Public/cpplib/lib/libopencv_core.dylib bin_ocr
 install_name_tool -change @rpath/libopencv_highgui.4.4.dylib /Users/dkos/Public/cpplib/lib/libopencv_highgui.dylib bin_ocr
 install_name_tool -change @rpath/libopencv_imgproc.4.4.dylib /Users/dkos/Public/cpplib/lib/libopencv_imgproc.dylib bin_ocr
 install_name_tool -change @rpath/libopencv_features2d.4.4.dylib /Users/dkos/Public/cpplib/lib/libopencv_features2d.dylib bin_ocr
 install_name_tool -change @rpath/libopencv_imgcodecs.4.4.dylib /Users/dkos/Public/cpplib/lib/libopencv_imgcodecs.dylib bin_ocr
 install_name_tool -change @rpath/libboost_regex.dylib /Users/dkos/Public/cpplib/lib/libboost_regex.dylib bin_ocr
