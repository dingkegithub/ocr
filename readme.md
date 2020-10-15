This is a action that caffe cpp web classify.

## 项目使用开源包
 
- **`json解析库:`**  https://github.com/nlohmann/json.git
- **`web框架:`** https://github.com/ipkn/crow.git
- **`log框架:`** https://github.com/google/glog.git
- **`c++第三方库:`** [boost-1.74.0](https://dl.bintray.com/boostorg/release/1.74.0/source/boost_1_74_0.tar.gz) 
- **`图片处理库:`** opencv-4.4.0
- **`python`** python 3.7.5 用于制作模板

## python 以来安装

```
$ pip3 install numpy
$ pip3 install matplotlib
$ pip3 install OpenCV-Python
```

## Ubuntu 为例进行第三方库安装

假设把库安装到当前项目目录的 locallib 下面

- 以来安装

```
$ sudo apt-get install -y autoconf automake libtool
$ apt-get install -y build-essential libtcmalloc-minimal4 && ln -s /usr/lib/libtcmalloc_minimal.so.4 /usr/lib/libtcmalloc_minimal.so
```

- opencv

```
$ unzip opencv-4.4.0.zip
$ mkdir build
$ cd build
$ cmake -D CMAKE_INSTALL_PREFIX=locallib ..
$ make 
$ make install
```

- boost

```
$ tar xf boost_1_73_0.tar.gz
$ cd boost_1_73_0
$ ./bootstrap.sh
$ ./b2
$ ./b2 install --prefix=locallib
```

- glog

```
$ git clone https://github.com/google/glog.git
$ cd glog/
$ mkdir build
$ cd build/
$ cmake -D CMAKE_INSTALL_PREFIX=locallib
$ make -j8
$ make install
```


## 编译

```
$: make
```

## 生成模板

```
$: make model
```

## 配置文件

```
{
  "env": {
    "process_num": 1,
    "listen_port": 18080
  },

  "log": {
    "level": 1,
    "size": 20,
    "dir": "/ocr/log",
    "name": "log"
  },

  "acr": {
    "model": "/ocr/model"
  }
}
```

## 本地运行

```
$ export LD_LIBRARY_PATH=./locallib/lib:/usr/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH
$ ldconfig
$: (./ocr --conf src/setting/setting.json &)
```

## docker 运行

```
$ make dock
$ docker run -it --name myocr -p 18090:18090 -t ocr
```

## 请求

客户端 post form-data， 设置 img 为实际图片

响应

```
{
    "reason": "ok",
    "name": 92,    # 识别结果
    "scores": 0,
    "status": 200
}
```

