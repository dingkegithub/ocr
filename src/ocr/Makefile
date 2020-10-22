WORK_DIR=`pwd`
target:
	g++ -g -std=c++11 -fpermissive -w \
		${WORK_DIR}/ocr.cpp \
		${WORK_DIR}/test.cpp \
		-o bin_ocr \
		-I ${WORK_DIR} \
		-I /Users/dkos/Public/cpplib/include/opencv4 \
		-I /Users/dkos/Public/cpplib/include \
		-L /Users/dkos/Public/cpplib/lib \
		-lopencv_core            \
		-lopencv_highgui         \
		-lopencv_imgproc         \
		-lopencv_core            \
		-lopencv_features2d      \
		-lopencv_imgcodecs       \
		-lboost_filesystem       \
		-lboost_system           \
		-lpthread

#install_name_tool -change @rpath/libboost_system.dylib  /Users/dkos/Public/cpplib/lib/libboost_system.dylib bin_ocr
#install_name_tool -change @rpath/libboost_filesystem.dylib /Users/dkos/Public/cpplib/lib/libboost_filesystem.dylib bin_ocr


#  g++ -std=c++11 -Wall -O -g -I XXX -L XXX -lxxxx xx.c
#  g++ -std=c++11 -Wall -O -g -I ../opensource/include/opencv4 -I ../opensource/include -I ./ -c ocr.cpp
#
#
#  ar rcs libxxx.a xxx.o
#   ar rcs libocr.a ocr.o
