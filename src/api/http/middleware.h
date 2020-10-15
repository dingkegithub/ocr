#include <utility>

//
// Created by dk on 12/14/19.
//

#ifndef _CLASSIFY_MIDDLEWARE_H
#define _CLASSIFY_MIDDLEWARE_H

#include <string>


struct AcrMiddleWare {
    std::string message;
    acr::Acr* acr_cl;

    AcrMiddleWare() {
        message = "acr";
        acr_cl = acr::Acr::instance();
    }

    void setMessage(std::string newMsg) {
        message = std::move(newMsg);
    }

    struct context {
    };

    void before_handle(crow::request& req, crow::response& res, context& ctx) {
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx) {
    }
};

#endif
