//
// Created by dk on 12/14/19.
//

#include "base64.h"


string Base64::encode(const char* data, int dsize) {

    //code table
    const char encodeTable[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    int lineLength=0;
    string strEncode;
    unsigned char encodeTmp[4]={0};

    for (int i=0;i<(int)(dsize / 3);i++) {

        encodeTmp[1] = *data++;
        encodeTmp[2] = *data++;
        encodeTmp[3] = *data++;
        strEncode+= encodeTable[encodeTmp[1] >> 2];
        strEncode+= encodeTable[((encodeTmp[1] << 4) | (encodeTmp[2] >> 4)) & 0x3F];
        strEncode+= encodeTable[((encodeTmp[2] << 2) | (encodeTmp[3] >> 6)) & 0x3F];
        strEncode+= encodeTable[encodeTmp[3] & 0x3F];

        if(lineLength+=4,lineLength==76) {
            strEncode+="\r\n";
            lineLength=0;
        }
    }

    int Mod=dsize % 3;
    if (Mod==1) {
        encodeTmp[1] = *data++;
        strEncode+= encodeTable[(encodeTmp[1] & 0xFC) >> 2];
        strEncode+= encodeTable[((encodeTmp[1] & 0x03) << 4)];
        strEncode+= "==";
    } else if (Mod==2) {

        encodeTmp[1] = *data++;
        encodeTmp[2] = *data++;
        strEncode+= encodeTable[(encodeTmp[1] & 0xFC) >> 2];
        strEncode+= encodeTable[((encodeTmp[1] & 0x03) << 4) | ((encodeTmp[2] & 0xF0) >> 4)];
        strEncode+= encodeTable[((encodeTmp[2] & 0x0F) << 2)];
        strEncode+= "=";
    }

    return strEncode;
}

string Base64::decode(const char* data, int dsize, int &osize) {

    //decode table
    const char decodeTable[] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            62, // '+'
            0, 0, 0,
            63, // '/'
            52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
            0, 0, 0, 0, 0, 0, 0,
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
            13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
            0, 0, 0, 0, 0, 0,
            26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
            39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
    };

    string strDecode;
    int nValue;
    int i= 0;
    while (i < dsize) {

        if (*data != '\r' && *data!='\n') {

            nValue = decodeTable[*data++] << 18;
            nValue += decodeTable[*data++] << 12;
            strDecode+=(nValue & 0x00FF0000) >> 16;
            osize++;

            if (*data != '=') {
                nValue += decodeTable[*data++] << 6;
                strDecode+=(nValue & 0x0000FF00) >> 8;
                osize++;
                if (*data != '=') {
                    nValue += decodeTable[*data++];
                    strDecode+=nValue & 0x000000FF;
                    osize++;
                }
            }
            i += 4;
        } else {

            data++;
            i++;
        }
    }

    return strDecode;
}
