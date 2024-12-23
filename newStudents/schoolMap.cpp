#include "schoolMap.h"

SchoolMap::SchoolAttraction::SchoolAttraction(int id, const std::string& name, const std::string& info,
    const std::array<int, 2>& position, int tag, bool visable)
    : id(id), name(name), info(info), position(position), tag(tag), visable(visable) {}

nlohmann::json SchoolMap::SchoolAttraction::to_json() const {
    return nlohmann::json{
        {"id", id},
        {"name", name},
        {"info", info},
        {"position", position},
        {"tag", tag},
        {"visable", visable}
    };
}

SchoolMap::SchoolAttraction SchoolMap::SchoolAttraction::from_json(const nlohmann::json& j) {
    return SchoolAttraction(
        j.at("id").get<int>(),
        j.at("name").get<std::string>(),
        j.at("info").get<std::string>(),
        j.at("position").get<std::array<int, 2>>(),
        j.at("tag").get<int>(),
        j.at("visable").get<bool>()
    );
}

SchoolMap::SchoolMap() {
    loadFromFile();
}

void SchoolMap::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << ". Creating a new file." << std::endl;
        saveToFile(filename);
        return ;
    }

    nlohmann::json j;
    file >> j;

    for (const auto& item : j) {
        SchoolAttraction attraction = SchoolAttraction::from_json(item);
        attractions[attraction.id] = attraction;
        if (attraction.id > current_id) {
            current_id = attraction.id;
        }
    }
}

void SchoolMap::saveToFile(const std::string& filename) {
    if (attractions.empty()) {
        std::ofstream file(filename);
        file << "{}"; // 写入一个空的 JSON 对象
        return;
    }

    nlohmann::json j;
    for (const auto& attraction : attractions) {
        j.push_back(attraction.second.to_json());
    }

    std::ofstream file(filename);
    file << j.dump(4);
}

nlohmann::json SchoolMap::getVisibleAttractions() const {
    nlohmann::json result;
    result["count"] = 0;
    result["results"] = nlohmann::json::array();

    for (const auto& [id, attraction] : attractions) {
        if (attraction.visable) {
            result["results"].push_back(attraction.to_json());
            result["count"] = result["count"].get<int>() + 1;
        }
    }

    return result;
}

void SchoolMap::addAttraction(const std::string& name, const std::string& info,
    const std::array<int, 2>& position, int tag, bool visable) {
    int id = ++current_id;
    SchoolAttraction newAttraction(id, name, info, position, tag, visable);
    attractions[id] = newAttraction;
	std::cout << "addAttraction" << std::endl;
    saveToFile();
}

void SchoolMap::deleteAttraction(int id) {
    attractions.erase(id);
    saveToFile();
}
