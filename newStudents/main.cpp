#include "httplib.h"
#include"nlohmann/json.hpp"

#include"schoolMap.h"
int main() {
    using namespace httplib;
    using json = nlohmann::json;
    Server svr;
    SchoolMap school_map;
    //进行初始化操作，挂载首页，防止跨域问题
    auto ret = svr.set_mount_point("/", "./index.html");

    svr.set_default_headers({
        { "Access-Control-Allow-Origin" , "*" },
        { "Access-Control-Allow-Methods", "POST, GET, PUT, OPTIONS, DELETE"},
        { "Access-Control-Max-Age", "3600"},
        { "Access-Control-Allow-Headers", "*"},
        { "Content-Type", "application/json;charset=utf-8"}
        });
	svr.Options(R"(.*)", [&](const Request& req, Response& res) {
		res.status = 200;
		});

    //对景点信息进行操作，实现增删改查
    svr.Get("/getArractions", [&](const Request& req, Response& res) {
        json attractions_json = school_map.getVisibleAttractions();
        res.set_content(attractions_json.dump(), "application/json");
        });

    svr.Post("/addArractions", [&](const Request& req, Response& res) {
        json j = json::parse(req.body);
        //std::cout << j.dump() << std::endl;
        auto name = j.at("name").get<std::string>();
        auto info = j.at("info").get<std::string>();
        auto position = j.at("position").get<std::array<double, 2>>();
        auto tag = j.at("tag").get<int>();
        auto visable = j.at("visable").get<bool>();
        school_map.addAttraction(name, info, position, tag, visable);
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

    //对道路信息进行操作，实现增删改查
    svr.Post("/getPaths", [&](const Request& req, Response& res) {
        auto j = json::parse(req.body);
        int from = j.at("from").get<int>();
        //std::cout << req.body << std::endl;
        json paths_json = school_map.getPaths(from);
        res.set_content(paths_json.dump(), "application/json");
        });
    svr.Get("/getAllPaths", [&](const Request& req, Response& res) {
        res.set_content(school_map.getAllPaths().dump(), "application/json");
        });
    svr.Post("/addPath", [&](const Request& req, Response& res) {
        json j = json::parse(req.body);
        auto name = j.at("name").get<std::string>();
        auto from = j.at("from").get<int>();
        auto to = j.at("to").get<int>();
        auto length = j.at("length").get<double>();
        auto walk = j.at("walk").get<bool>();
        auto sharebike = j.at("sharebike").get<bool>();
        auto bus = j.at("bus").get<bool>();
        school_map.addPath(name, from, to, length, walk, sharebike, bus);
        res.set_content("OK", "text/plain");
        });
    svr.Post("/addBiPath", [&](const Request& req, Response& res) {
        json j = json::parse(req.body);
        auto name = j.at("name").get<std::string>();
        auto from = j.at("from").get<int>();
        auto to = j.at("to").get<int>();
        auto length = j.at("length").get<double>();
        auto walk = j.at("walk").get<bool>();
        auto sharebike = j.at("sharebike").get<bool>();
        auto bus = j.at("bus").get<bool>();
        school_map.addPath(name, from, to, length, walk, sharebike, bus);
        school_map.addPath(name, to, from, length, walk, sharebike, bus);
        res.set_content("OK", "text/plain");
        });
    svr.Post("/deletePath", [&](const Request& req, Response& res) {
        json j = json::parse(req.body);
        auto id = j.at("id").get<int>();
        school_map.deletePath(id);
        res.set_content("OK", "text/plain");
        });
    svr.Post("/updatePath", [&](const Request& req, Response& res) {
        json j = json::parse(req.body);
        auto id = j.at("id").get<int>();
        auto name = j.at("name").get<std::string>();
        auto from = j.at("from").get<int>();
        auto to = j.at("to").get<int>();
        auto length = j.at("length").get<double>();
        auto walk = j.at("walk").get<bool>();
        auto sharebike = j.at("sharebike").get<bool>();
        auto bus = j.at("bus").get<bool>();
        school_map.updatePath(id, name, from, to, length, walk, sharebike, bus);
        res.set_content("OK", "text/plain");
        });
    svr.Post("/getShorestPath", [&](const Request& req, Response& res) {
        json j = json::parse(req.body);
        auto from = j.at("from").get<int>();
        auto to = j.at("to").get<int>();

        auto walk = true, sharebike = true, bus = true;

        if (j.contains("walk"))
            walk = j.at("walk").get<bool>();
        if (j.contains("sharebike"))
            sharebike = j.at("sharebike").get<bool>();
        if (j.contains("bus"))
            bus = j.at("bus").get<bool>();

        auto path = school_map.getShortestPath(from, to, walk, sharebike, bus);

        res.set_content(path.dump(), "application/json");
        });
    svr.Post("/getFastestPath", [&](const Request& req, Response& res) {
        json j = json::parse(req.body);
        auto from = j.at("from").get<int>();
        auto to = j.at("to").get<int>();

        auto walk = true, sharebike = true, bus = true;

        if (j.contains("walk"))
            walk = j.at("walk").get<bool>();
        if (j.contains("sharebike"))
            sharebike = j.at("sharebike").get<bool>();
        if (j.contains("bus"))
            bus = j.at("bus").get<bool>();

        auto path = school_map.getFastestPath(from, to, walk, sharebike, bus);

        res.set_content(path.dump(), "application/json");
        });
    svr.Post("/getAllPath", [&](const Request& req, Response& res) {
        json j = json::parse(req.body);
        auto from = j.at("from").get<int>();
        auto to = j.at("to").get<int>();

        auto walk = true, sharebike = true, bus = true;

        if (j.contains("walk"))
            walk = j.at("walk").get<bool>();
        if (j.contains("sharebike"))
            sharebike = j.at("sharebike").get<bool>();
        if (j.contains("bus"))
            bus = j.at("bus").get<bool>();

        auto paths = school_map.getAllPath(from, to, walk, sharebike, bus);

        res.set_content(paths.dump(), "application/json");
        });
    //监听相关端口
    svr.listen("127.0.0.1", 1234);
    return 0;
}
