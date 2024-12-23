#include "httplib.h"
#include"nlohmann/json.hpp"
#include"schoolMap.h"
int main() {
    using namespace httplib;
    using json = nlohmann::json;
    Server svr;
    SchoolMap school_map;
    
    svr.Get("/getArractions", [&](const Request& req, Response& res) {
        json attractions_json = school_map.getVisibleAttractions();
        res.set_content(attractions_json.dump(), "application/json");
        });

    svr.Post("/addArractions", [&](const Request& req, Response& res) {
		json j = json::parse(req.body);
		auto name = j.at("name").get<std::string>();
		auto info = j.at("info").get<std::string>();
		auto position = j.at("position").get<std::array<int, 2>>();
		auto tag = j.at("tag").get<int>();
		auto visable = j.at("visable").get<bool>();
		school_map.addAttraction( name, info, position, tag, visable);
		res.set_content("OK", "text/plain");
        });

    svr.Post("/deleteArractions", [&](const Request& req, Response& res) {
		school_map.deleteAttraction(std::stoi(req.body));
        });



    svr.listen("127.0.0.1", 1234);
    return 0;
}
