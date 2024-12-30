#include "schoolMap.h"
// _____       _ _   _       _ _               _       _              _                   _                  
//|_   _|     (_) | (_)     | (_)             | |     | |            | |                 | |                 
//  | |  _ __  _| |_ _  __ _| |_ _______    __| | __ _| |_ __ _   ___| |_ _ __ _   _  ___| |_ _   _ _ __ ___ 
//  | | | '_ \| | __| |/ _` | | |_  / _ \  / _` |/ _` | __/ _` | / __| __| '__| | | |/ __| __| | | | '__/ _ \
// _| |_| | | | | |_| | (_| | | |/ /  __/ | (_| | (_| | || (_| | \__ \ |_| |  | |_| | (__| |_| |_| | | |  __/
//|_____|_| |_|_|\__|_|\__,_|_|_/___\___|  \__,_|\__,_|\__\__,_| |___/\__|_|   \__,_|\___|\__|\__,_|_|  \___|
//                                                                                                           
//��ʼ����ͱߵ���Ϣ��ʵ�־���͵�·����ط���

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
SchoolMap::SchoolPath::SchoolPath(int id, std::string name,int from, int to, double length, bool walk, bool sharebike, bool bus)
	: id(id),name(name), from(from),to(to),length(length),walk(walk),sharebike(sharebike),bus(bus) {}

nlohmann::json SchoolMap::SchoolPath::to_json()const {
	return nlohmann::json{
		{"id",id},
		{"name",name},
		{"from",from},
		{"length",length},
		{"to",to},
		{"walk",walk},
		{"sharebike",sharebike},
		{"bus",bus}
	};
}
SchoolMap::SchoolPath SchoolMap::SchoolPath::from_json(const nlohmann::json& j) {
	return SchoolPath{
		j.at("id").get<int>(),
		j.at("name").get<std::string>(),
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
//ʵ�ִ��ļ��ж�ȡ����͵�·��Ϣ
void SchoolMap::loadFromFile(const std::string& attractionFilename, const std::string& pathFilename ) {
	std::ifstream attracttionStream(attractionFilename);
	std::ifstream pathStream(pathFilename);
	// ����ļ������ڣ�����һ�����ļ�
	if (!attracttionStream.is_open()) {
		std::ofstream file(attractionFilename);
		file << "{}"; // д��һ���յ� JSON ����
	}
	if (!pathStream.is_open()) {
		std::ofstream file(pathFilename);
		file << "[]"; // д��һ���յ� JSON ����
	}
	//��ȡ���������Ϣ
	nlohmann::json j;
	attracttionStream >> j;
	//std::cout << j.dump(4) << std::endl;
	for (const auto& item : j) {
		SchoolAttraction attraction = SchoolAttraction::from_json(item);
		attractions[attraction.id] = attraction;
		if (attraction.id > current_attraction_id) {
			current_attraction_id = attraction.id;
		}
	}
	//��ȡ��·�����Ϣ
	nlohmann::json k;
	pathStream >> k;
	for (const auto& item : k) {
		SchoolPath path = SchoolPath::from_json(item);

		if (!attractions.count(path.from)||!attractions.count(path.to)) {
			continue;
		}
		//std::cout << path.to_json().dump() << std::endl;
		destinations[path.from].push_back(path);
		paths[path.id] = path;
		if (path.id > current_path_id) {
			current_path_id = path.id;
		}
	}
}

void SchoolMap::saveToFile(const std::string& attractionFilename,const std::string& pathFilename) {
	if (attractions.empty()) {
		std::ofstream file(attractionFilename);
		file << "{}"; // д��һ���յ� JSON ����
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
		file << "[]"; // д��һ���յ� JSON ����
	}
	else {
		nlohmann::json k;
		for (const auto& [id, path] : paths) {
			
			k.push_back(path.to_json());
		}
		std::ofstream pathfile(pathFilename);
		pathfile << k.dump(4);
	}
	loadFromFile();
}

//       _______ _______ _____            _____ _______ _____ ____  _   _  _____ 
//    /\|__   __|__   __|  __ \     /\   / ____|__   __|_   _/ __ \| \ | |/ ____|
//   /  \  | |     | |  | |__) |   /  \ | |       | |    | || |  | |  \| | (___  
//  / /\ \ | |     | |  |  _  /   / /\ \| |       | |    | || |  | | . ` |\___ \ 
// / ____ \| |     | |  | | \ \  / ____ \ |____   | |   _| || |__| | |\  |____) |
///_/    \_\_|     |_|  |_|  \_\/_/    \_\_____|  |_|  |_____\____/|_| \_|_____/ 
                                                                                                                       
//ʵ�ֻ�ȡ������Ϣ����ɾ�Ĳ�
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
// _____     _______ _    _  _____ 
//|  __ \ /\|__   __| |  | |/ ____|
//| |__) /  \  | |  | |__| | (___  
//|  ___/ /\ \ | |  |  __  |\___ \ 
//| |  / ____ \| |  | |  | |____) |
//|_| /_/    \_\_|  |_|  |_|_____/ 
//                  
//               
//ʵ�ֵ�·����ɾ�Ĳ�

//��ȡĳ���㸽��ֱ�ӵ��ĵص�
nlohmann::json SchoolMap::getPaths(int from) {
	nlohmann::json result;
	result["count"] = 0;
	result["results"] = nlohmann::json::array();
	for (const auto& path : destinations[from]) {
		result["results"].push_back(path.to_json());
		result["count"] = result["count"].get<int>() + 1;
	}
	return result;
}

//��ȡ������ͼ����·��
nlohmann::json SchoolMap::getAllPaths()const {
	nlohmann::json result;
	result["count"] = 0;
	result["results"] = nlohmann::json::array();
	for (auto [id, path] : paths) {
		result["results"].push_back(path.to_json());
		result["count"] = result["count"].get<int>() + 1;
	}
	return result;
}


//���ӵ�·��Ϣ ��������
void SchoolMap::addPath(std::string name,int from, int to, double length, bool walk, bool sharebike, bool bus) {
	int id = ++current_path_id;
	SchoolPath newPath(id, name,from,to, length, walk, sharebike, bus);
	paths[id] = newPath;
	destinations[from].push_back(newPath);
	saveToFile();
}

//ɾ����·��Ϣ
void SchoolMap::deletePath(int id) {
	paths.erase(id);
	saveToFile();
}
 
//�޸ĵ�·��Ϣ
void SchoolMap::updatePath(int id, std::string name, int from, int to, double length, bool walk, bool sharebike, bool bus) {
	if (paths.find(id) == paths.end()) {
		return;
	}
	SchoolPath newPath(id, name, from, to, length, walk, sharebike, bus);
	paths[id] = newPath;
	saveToFile();
}

// __  __ ______ _______ _    _  ____  _____   _____ 
//|  \/  |  ____|__   __| |  | |/ __ \|  __ \ / ____|
//| \  / | |__     | |  | |__| | |  | | |  | | (___  
//| |\/| |  __|    | |  |  __  | |  | | |  | |\___ \ 
//| |  | | |____   | |  | |  | | |__| | |__| |____) |
//|_|  |_|______|  |_|  |_|  |_|\____/|_____/|_____/ 
//                                                   
//ʵ�ֶ��Ż��汾�����·����ȡdij�㷨 
// 
                                                