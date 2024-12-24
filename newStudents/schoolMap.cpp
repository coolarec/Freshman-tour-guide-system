#include "schoolMap.h"
 // _____       _ _   _       _ _               _       _              _                   _                  
 //|_   _|     (_) | (_)     | (_)             | |     | |            | |                 | |                 
 //  | |  _ __  _| |_ _  __ _| |_ _______    __| | __ _| |_ __ _   ___| |_ _ __ _   _  ___| |_ _   _ _ __ ___ 
 //  | | | '_ \| | __| |/ _` | | |_  / _ \  / _` |/ _` | __/ _` | / __| __| '__| | | |/ __| __| | | | '__/ _ \
 // _| |_| | | | | |_| | (_| | | |/ /  __/ | (_| | (_| | || (_| | \__ \ |_| |  | |_| | (__| |_| |_| | | |  __/
 //|_____|_| |_|_|\__|_|\__,_|_|_/___\___|  \__,_|\__,_|\__\__,_| |___/\__|_|   \__,_|\___|\__|\__,_|_|  \___|
 //                                                                                                           
//初始化点和边的信息，实现景点和道路的相关方法
SchoolMap::SchoolAttraction::SchoolAttraction(int id, const std::string& name, const std::string& info,
    const std::array<double, 2>& position, int tag, bool visable)
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
        j.at("position").get<std::array<double, 2>>(),
        j.at("tag").get<int>(),
        j.at("visable").get<bool>()
    );
}
SchoolMap::SchoolPath::SchoolPath(int id, int from, int to, double length, bool walk, bool sharebike, bool bus)
    : id(id),from(from),to(to),length(length),walk(walk),sharebike(sharebike),bus(bus) {}

nlohmann::json SchoolMap::SchoolPath::to_json()const {
    return nlohmann::json{
        {"id",id},
        {"from",from},
        {"to",to},
        {"walk",walk},
        {"sharebike",sharebike},
        {"bus",bus}
    };
}
SchoolMap::SchoolPath SchoolMap::SchoolPath::from_json(const nlohmann::json& j) {
    return SchoolPath{
        j.at("id").get<int>(),
        j.at("from").get<int>(),
        j.at("to").get<int>(),
        j.at("length").get<double>(),
        j.at("walk").get<bool>(),
        j.at("sharebike").get<bool>(),
        j.at("bus").get<bool>()
    };
}
SchoolMap::SchoolMap() {
    loadFromFile();
}


 // ______ _ _                                   _   _                 
 //|  ____(_) |                                 | | (_)                
 //| |__   _| | ___    ___  _ __   ___ _ __ __ _| |_ _  ___  _ __  ___ 
 //|  __| | | |/ _ \  / _ \| '_ \ / _ \ '__/ _` | __| |/ _ \| '_ \/ __|
 //| |    | | |  __/ | (_) | |_) |  __/ | | (_| | |_| | (_) | | | \__ \
 //|_|    |_|_|\___|  \___/| .__/ \___|_|  \__,_|\__|_|\___/|_| |_|___/
 //                        | |                                         
 //                        |_|                                         
//实现从文件中读取景点和道路信息
void SchoolMap::loadFromFile(const std::string& attractionFilename, const std::string& pathFilename ) {
    std::ifstream attracttion(attractionFilename);
    std::ifstream path(pathFilename);

    if (!attracttion.is_open()||!path.is_open()) {
        std::cerr << "Unable to open file: " << attractionFilename << ". Creating a new file." << std::endl;
        saveToFile();
    }
    nlohmann::json j;
    attracttion >> j;
    std::cout << "asdasdasdsaasd\n";
    for (const auto& item : j) {
        SchoolAttraction attraction = SchoolAttraction::from_json(item);
        attractions[attraction.id] = attraction;
        if (attraction.id > current_attraction_id) {
            current_attraction_id = attraction.id;
        }
    }

    nlohmann::json k;
    path >>k;
    for (const auto& item : k) {
		SchoolPath path = SchoolPath::from_json(item);
		paths[path.id] = path;
        if (path.id > current_path_id) {
            current_path_id = path.id;
        }
    }
}

void SchoolMap::saveToFile(const std::string& attractionFilename,const std::string& pathFilename) {
    if (attractions.empty()) {
        std::ofstream file(attractionFilename);
        file << "{}"; // 写入一个空的 JSON 对象
    }
    else {
        nlohmann::json j;
        for (const auto& attraction : attractions) {
            j.push_back(attraction.second.to_json());
        }    
        std::ofstream attractionfile(attractionFilename);
        attractionfile << j.dump(4);
    }
    if (paths.empty()) {
        std::ofstream file(pathFilename);
        file << "[]"; // 写入一个空的 JSON 对象
        return;
    }
    else {
        nlohmann::json k;
        for (const auto& [id, path] : paths) {
            k.push_back(path.to_json());
        }
        std::ofstream pathfile(pathFilename);
        pathfile << k.dump(4);
    }
}

 //         _   _                  _   _                 
 //    /\  | | | |                | | (_)                
 //   /  \ | |_| |_ _ __ __ _  ___| |_ _  ___  _ __  ___ 
 //  / /\ \| __| __| '__/ _` |/ __| __| |/ _ \| '_ \/ __|
 // / ____ \ |_| |_| | | (_| | (__| |_| | (_) | | | \__ \
 ///_/    \_\__|\__|_|  \__,_|\___|\__|_|\___/|_| |_|___/
 //                                                      
 //                                                      
//实现获取景点信息的增删改查
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

nlohmann::json SchoolMap::getAllAttractions() const {
    nlohmann::json result;
    result["count"] = 0;
    result["results"] = nlohmann::json::array();

    for (const auto& [id, attraction] : attractions) {
        result["results"].push_back(attraction.to_json());
        result["count"] = result["count"].get<int>() + 1;
    }
    return result;
}

void SchoolMap::addAttraction(const std::string& name, const std::string& info,
    const std::array<double, 2>& position, int tag, bool visable) {
    int id = ++current_attraction_id;
    SchoolAttraction newAttraction(id, name, info, position, tag, visable);
    attractions[id] = newAttraction;
	std::cout << "addAttraction" << std::endl;
    saveToFile();
}

void SchoolMap::deleteAttraction(int id) {
    attractions.erase(id);
    saveToFile();
}

int SchoolMap::updateAttraction(int id, const std::string& name, const std::string& info,
	const std::array<double, 2>& position, int tag, bool visable) {
	if (attractions.find(id) == attractions.end()) {
		return -1;
	}
	SchoolAttraction newAttraction(id, name, info, position, tag, visable);
	attractions[id] = newAttraction;
	saveToFile();
	return 0;
}
