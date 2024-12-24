#include "httplib.h"
#include"nlohmann/json.hpp"
#include"schoolMap.h"
int main() {
    using namespace httplib;
    using json = nlohmann::json;
    Server svr;
    SchoolMap school_map;


    auto ret = svr.set_mount_point("/", "./index.html");

    svr.set_default_headers({
        { "Access-Control-Allow-Origin" , "*" },
        { "Access-Control-Allow-Methods", "POST, GET, PUT, OPTIONS, DELETE"},
        { "Access-Control-Max-Age", "3600"},
        { "Access-Control-Allow-Headers", "*"},
        { "Content-Type", "application/json;charset=utf-8"}
        });
    
    svr.Get("/getArractions", [&](const Request& req, Response& res) {
        json attractions_json = school_map.getVisibleAttractions();
        res.set_content(attractions_json.dump(), "application/json");
        });

    svr.Post("/addVisableArractions", [&](const Request& req, Response& res) {
		json j = json::parse(req.body);
		auto name = j.at("name").get<std::string>();
		auto info = j.at("info").get<std::string>();
		auto position = j.at("position").get<std::array<double, 2>>();
		auto tag = j.at("tag").get<int>();
		auto visable = j.at("visable").get<bool>();
		school_map.addAttraction( name, info, position, tag, visable);
		res.set_content("OK", "text/plain");
        });

    svr.Post("/deleteArractions", [&](const Request& req, Response& res) {
        json j = json::parse(req.body);
        school_map.deleteAttraction(j.at("id").get<int>());
        res.set_content("OK", "text/plain");
        });

    svr.Post("/updateArractions", [&](const Request& req, Response& res) {
        json j = json::parse(req.body);
        auto id = j.at("id").get<int>();
        auto name = j.at("name").get<std::string>();
        auto info = j.at("info").get<std::string>();
        auto position = j.at("position").get<std::array<double, 2>>();
        auto tag = j.at("tag").get<int>();
        auto visable = j.at("visable").get<bool>();
        school_map.updateAttraction(id, name, info, position, tag, visable);
        res.set_content("OK", "text/plain");
        });

    svr.listen("127.0.0.1", 1234);
    return 0;
}
