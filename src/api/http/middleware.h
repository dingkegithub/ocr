#include <utility>

//
// Created by dk on 12/14/19.
//

#ifndef _CLASSIFY_MIDDLEWARE_H
#define _CLASSIFY_MIDDLEWARE_H

#include <string>


struct OcrMiddleWare {
    OcrMiddleWare() {
    }


    struct context {
    };

    void before_handle(crow::request& req, crow::response& res, context& ctx) {
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx) {
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Max-Age", "120");
        res.add_header("Access-Control-Allow-Methods", "POST");
    }
};

#endif
