FROM ubuntu:latest
LABEL maintainer="DingKe"
WORKDIR /ocr
ENV LD_LIBRARY_PATH /ocr/locallib/lib:/usr/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH
RUN mkdir -p /ocr/log; \
    apt-get update; \
    apt-get install -y autoconf automake libtool build-essential libtcmalloc-minimal4 && ln -s /usr/lib/libtcmalloc_minimal.so.4 /usr/lib/libtcmalloc_minimal.so
COPY locallib /ocr/locallib 
COPY ocr /ocr/
COPY resource/model /ocr/model
COPY src/setting/setting.json /ocr/
EXPOSE 18080
RUN ldconfig
ENTRYPOINT ["./ocr", "-c", "setting.json"]
