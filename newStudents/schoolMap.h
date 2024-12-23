#pragma once
#include "nlohmann/json.hpp"
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <array>

class SchoolMap {
    struct SchoolAttraction {
        int id; // 景点编号
        std::string name; // 景点名称
        std::string info; // 景点信息介绍
        std::array<int, 2> position; // 景点坐标
        int tag; // 景点标签，是否为教学楼，体育馆等
        bool visable; // 景点是否可见

        SchoolAttraction(int id, const std::string& name, const std::string& info,
            const std::array<int, 2>& position, int tag, bool visable);
		SchoolAttraction() = default;
        nlohmann::json to_json() const;
        static SchoolAttraction from_json(const nlohmann::json& j);
    };

    std::map<int, SchoolAttraction> attractions;
    std::map<int, std::vector<int>> adjList;

public:
	int current_id = 0;
    SchoolMap();
    void loadFromFile(const std::string& filename = "schoolmap.dat");
    void saveToFile(const std::string& filename = "schoolmap.dat");
    nlohmann::json getVisibleAttractions() const;
    void addAttraction(const std::string& name, const std::string& info,
        const std::array<int, 2>& position, int tag, bool visable);
    void deleteAttraction(int id);
};
