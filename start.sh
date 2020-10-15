#!/bin/bash
 export LD_LIBRARY_PATH=./locallib/lib:/usr/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH
ldconfig
(./ocr --conf src/setting/setting.json &)
