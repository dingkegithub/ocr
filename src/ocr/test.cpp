#include "ocr.h"
#include <string>
#include <iostream>

int main(int argc, char** argv) {
    std::string temp_path = std::string(argv[1]);
    OCR *ocr = OCR::getInstance()->set_template(temp_path);
    ocr->display_switch = true;
    //ocr->test(std::string(argv[2]));
    ocr->recognise(std::string(argv[2]));
    return 0;
}
