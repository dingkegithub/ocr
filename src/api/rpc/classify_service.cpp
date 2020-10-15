//
// Created by dk on 12/12/19.
//

#include "classify_service.h"

#include <regex>
#include "acr.h"

using std::regex;
using std::string;

namespace rpc_server {

    Status ClassifyService::Recognise(
            ServerContext *context,
            const Request *request,
            Response *response) {

        string file_path = request->path();
        string img_contents = request->img();

        Prediction p;
        if (file_path.size() > 4) {
            p = Acr::instance()->predict(file_path);
        } else {
            p = Acr::instance()->predict(img_contents, true);
        }

        std::smatch match;
        std::regex rg(".+/(.+)");

        if (std::regex_match(p.first, match, rg)) {

            if (match.size() != 2) {
                response->set_name("");
                response->set_score(1);
            } else {
                response->set_name(match[1]);
                response->set_score(p.second);
            }
        }

        return Status::OK;
    }

}
