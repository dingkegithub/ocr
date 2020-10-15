//
// Created by dk on 12/14/19.
//

#ifndef _CLASSIFY_H_
#define _CLASSIFY_H_

#include "crow_all.h"
#include "crow_body.h"
#include <string>
#include <ctime>
#include "ocr.h"


namespace http_server {

    crow::json::wvalue gen_response(int error, const std::string &msg, const int &name = -1, float score = .0) {
        crow::json::wvalue resp;

        if (error != 200) {
            resp["status"] = error;
            resp["reason"] = msg;
            return resp;
        }

        resp["status"] = 200;
        resp["scores"] = score;
        resp["name"] = name;
        resp["reason"] = "ok";
        return resp;
    }

    void router(crow::SimpleApp &app) {
        CROW_ROUTE(app, "/ocr")
        .methods("POST"_method)
        ([](const crow::request &req) {
                    std::string content_type;
                    if (req.headers.count("Content-Type") == 1) {
                        content_type = req.headers.find("Content-Type")->second;
                    } else {
                        return gen_response(-1, "bad request header");
                    }

                    auto body = req.body;
                    clock_t b_s = clock();
                    form_data_t form = parse_img_body(content_type, body);
                    double b_el = (double)(clock() - b_s)/CLOCKS_PER_SEC;
                    std::cout << "body elapse time: " << b_el << "s" << std::endl;
                    if (form.field != "img") {
                        return gen_response(-2, "form field img need");
                    }

                    if (form.img.size() <= 0) {
                        return gen_response(-3, "invalid image");
                    }

                    clock_t cl_s = clock();
                    std::vector<uchar> imchar;
                    for (int i=0; i<form.img.size(); i++) {
                        imchar.push_back(form.img[i]);
                    }
                    int ocr_num = OCR::getInstance()->recognise(imchar);
                    double cl_el = double(clock() - cl_s) / CLOCKS_PER_SEC;
                    std::cout << "net elapse time" << cl_el << "s" << std::endl;

                    return gen_response(200, "ok", ocr_num);
                });
    }
}
#endif
