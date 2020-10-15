//
// Created by dk on 12/14/19.
//

#ifndef _CROW_BODY_H
#define _CROW_BODY_H

#include <iostream>
#include <string>
#include <boost/regex.hpp>
using std::string;

typedef struct form_data_s {
    string field;
    string file;
    string img;
    string type;
} form_data_t;

form_data_t parse_img_body(const string &content_type, const string &body) {

    form_data_t form {
        .field = "",
        .file = "",
        .img = "",
        .type = ""
    };

    int status = 0;
    string::const_iterator start = body.begin();
    string::const_iterator content_start;

    boost::smatch r;
    boost::regex b_regex {"multipart/form-data; boundary=(.+)$"};
    if (!boost::regex_search(content_type, r, b_regex)) {
        return form;
    }

    std::string bs = "--" + r[1];
    std::string be = bs + "--";

    boost::regex type_regex{"^Content-Type: image/(jpeg|jpg|png)"};
    boost::regex pos_regex{"^Content-Disposition: form-data; name=\"(.+)\"; filename=\"(.+)\"[;]{0,1}.*"};
    for (string::const_iterator it=body.begin(); it!=body.end(); ++it) {
        if (*it == '\n') {

            string content(start, it-1);

            if (content == bs) {
                start = it+1;
                status = 1;
                continue;
            }

            boost::smatch res;

            if (status == 1) {
                if (boost::regex_search(content, res, pos_regex)) {
                    form.field = res[1];
                    form.file = res[2];
                    status = 2;
                }
                start = it + 1;
                continue;
            }

            if (status == 2 && !content.empty()) {
                start = it + 1;
                continue;
            }

            if (status == 2 && content.empty()) {
                status = 3;
                start = it + 1;
                content_start = it + 1;
                continue;
            }

            if (status == 3 && content.size() >= be.size()) {
                std::string eb = string(content.end() - be.size(), content.end());
                if (eb == be) {
                    form.img = string(content_start, body.end() - 2 - be.size() );
                }
            }
        }
    }

    return form;
}

#endif
