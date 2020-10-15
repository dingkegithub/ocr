//
// Created by dk on 12/13/19.
//

#include "config.h"
#include "json.hpp"
#include <fstream>
#include <boost/filesystem.hpp>

using std::ifstream;
using std::string;
using namespace boost;


namespace cfg {

    config_t parse(const string &file) {

        filesystem::path p {file};
        if (!filesystem::exists(p)) {
            fprintf(stderr, "filed not exist %s", p.string().c_str());
            exit(-2);
        }

        string exe_path = filesystem::initial_path<filesystem::path>().string();

        config_t cfg_info;

        ifstream s(file, std::ifstream::in);
        if (!s) {
            assert(s);
        }

        nlohmann::json js;
        s >> js;
        cfg_info.acr = {
               .model = js["acr"]["model"].get<string>(),
               .mean = js["acr"]["mean"].get<string>(),
               .deploy = js["acr"]["deploy"].get<string>(),
               .word = js["acr"]["word"].get<string>(),
               .gpu_enable = js["acr"]["gpu_enable"].get<bool>(),
               .gpu_number = js["acr"]["gpu_number"].get<int>()
        };

        cfg_info.env = {
                .process_num = js["env"]["process_num"].get<int>(),
                .listen_port = js["env"]["listen_port"].get<int>()
        };

//        cfg_info.log = {
//                .level = js["log"]["level"].get<int>(),
//                .size = js["log"]["size"].get<int>(),
//                .name = js["log"]["name"].get<string>(),
//                .dir = exe_path
//        };
        cfg_info.log.level = js["log"]["level"].get<int>();
        cfg_info.log.size = js["log"]["size"].get<int>();
        cfg_info.log.name = js["log"]["name"].get<string>();
        cfg_info.log.dir = exe_path;

        return cfg_info;
    }
}
