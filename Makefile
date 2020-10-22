DEP_LIB=/Users/dkos/Public/cpplib
target:
	c++ -g -std=c++11 -fpermissive -w                      \
                        src/main.cpp                       \
                        src/config/config.cpp              \
                        src/option/option.cpp              \
                        src/utils/log/log.cpp              \
                        src/utils/encrypt/base64.cpp       \
                        src/ocr/ocr.cpp                    \
                        -o ocr                             \
                        -I src/api/http                    \
                        -I src/config                      \
                        -I src/ocr                         \
                        -I src/option                      \
                        -I src/utils                       \
                        -I src/utils/crow                  \
                        -I src/utils/log                   \
                        -I src/utils/encrypt               \
                        -I src/utils/json                  \
                        -I /usr/include                    \
                        -I /usr/local/include              \
                        -I ${DEP_LIB}/include              \
                        -I ${DEP_LIB}/include/opencv4      \
                        -L ${DEP_LIB}/lib                  \
                        -L /usr/lib                        \
                        -L /usr/local/lib                  \
                        -L /usr/lib/x86_64-linux-gnu       \
                        -lopencv_core                      \
                        -lopencv_highgui                   \
                        -lopencv_imgproc                   \
                        -lopencv_core                      \
                        -lopencv_features2d                \
                        -lopencv_imgcodecs                 \
                        -lglog                             \
                        -lboost_system                     \
                        -lboost_filesystem                 \
                        -lboost_regex                      \
                        -lpthread

.PHONY:dock
dock:
	docker built -t dkocr:latest .

.PHONY:model
model:
	python3 tools/templat_make/main.py resource/source_imgs resource/model

.PHONY:dep
dep:
	sudo apt-get install -y autoconf automake libtool
	sudo apt-get install build-essential libtcmalloc-minimal4 && sudo ln -s /usr/lib/libtcmalloc_minimal.so.4 /usr/lib/libtcmalloc_minimal.so

#.PHONY:proto
#proto:
#	protoc -I src/common/proto --cpp_out=src/common/proto/ acr.proto
#	protoc -I src/common/proto --grpc_out=src/common/proto/ --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` acr.proto
