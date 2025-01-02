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
//SchoolMap::BusRoute::BusRoute(int id,std::vector<int> r):id(id){
//	route = r;
//}
//nlohmann::json SchoolMap::BusRoute::to_json(){
//	return{
//		{"id",id},
//		{"route",route}
//	};
//}

//SchoolMap::BusRoute SchoolMap::BusRoute::from_json(const nlohmann::json& j) {
//	std::vector<int> r = j.at("route").get<std::vector<int>>();
//	return BusRoute{
//		j.at("id").get<int>(),
//		j.at("route").get<std::vector<int>>()
//	};
//}
//std::vector<int> SchoolMap::BusRoute::getRoute(int from, int to)const {
//	std::vector<int> res;
//	int i = 0;
//	while (i < route.size() && i != from)i++;
//	while (route[(++i)%route.size()] != to) {
//		res.push_back(route[i]);
//	}
//	return res;
//}
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
void SchoolMap::loadFromFile(const std::string& attractionFilename, const std::string& pathFilename) {
	std::ifstream attracttionStream(attractionFilename);
	std::ifstream pathStream(pathFilename);
	std::ifstream busStream("bus.json");
	// 如果文件不存在，创建一个新文件
	if (!attracttionStream.is_open()) {
		std::ofstream file(attractionFilename);
		file << "{}"; // 写入一个空的 JSON 对象
	}
	if (!pathStream.is_open()) {
		std::ofstream file(pathFilename);
		file << "[]"; // 写入一个空的 JSON 对象
	}
	//获取景点相关信息
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
	//获取道路相关信息
	nlohmann::json k;
	pathStream >> k;
	for (const auto& item : k) {
		SchoolPath path = SchoolPath::from_json(item);
		if (!attractions.count(path.from)||!attractions.count(path.to)) {
			continue;
		}
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

//       _______ _______ _____            _____ _______ _____ ____  _   _  _____ 
//    /\|__   __|__   __|  __ \     /\   / ____|__   __|_   _/ __ \| \ | |/ ____|
//   /  \  | |     | |  | |__) |   /  \ | |       | |    | || |  | |  \| | (___  
//  / /\ \ | |     | |  |  _  /   / /\ \| |       | |    | || |  | | . ` |\___ \ 
// / ____ \| |     | |  | | \ \  / ____ \ |____   | |   _| || |__| | |\  |____) |
///_/    \_\_|     |_|  |_|  \_\/_/    \_\_____|  |_|  |_____\____/|_| \_|_____/ 
                                                                                                                       
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
// _____     _______ _    _  _____ 
//|  __ \ /\|__   __| |  | |/ ____|
//| |__) /  \  | |  | |__| | (___  
//|  ___/ /\ \ | |  |  __  |\___ \ 
//| |  / ____ \| |  | |  | |____) |
//|_| /_/    \_\_|  |_|  |_|_____/ 
//                  
//               
//实现道路的增删改查

//获取某个点附近直接到的地点
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

//获取整个地图所有路径
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


//增加道路信息 单向增加
void SchoolMap::addPath(std::string name,int from, int to, double length, bool walk, bool sharebike, bool bus) {
	int id = ++current_path_id;
	SchoolPath newPath(id, name,from,to, length, walk, sharebike, bus);
	paths[id] = newPath;
	destinations[from].push_back(newPath);
	saveToFile();
}

//删除道路信息
void SchoolMap::deletePath(int id) {
	paths.erase(id);
	saveToFile();
}
 
//修改道路信息
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
//实现堆优化版本的最短路，采取dij算法 
// 
nlohmann::json SchoolMap::getShortestPath(int from, int to, bool walk, bool sharebike, bool bus) {
	nlohmann::json result;
	result["count"] = -1;
	result["results"] = nlohmann::json::array();
	if (!destinations.count(from) || !destinations.count(to)) {
		return result;
	}

	std::vector<double> dist;
	std::vector<SchoolPath> pre;
	std::vector<bool> vis;
	std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<std::pair<double, int>>> q;
	dist.resize(current_attraction_id, INT_MAX);
	pre.resize(current_attraction_id);
	vis.resize(current_attraction_id, false);
	dist[from] = 0;
	q.push({ 0,from });
	while (!q.empty()) {
		auto [d, u] = q.top();
		q.pop();
		if (vis[u]) {
			continue;
		}
		vis[u] = true;
		for (const auto& path : destinations[u]) {
			if (!walk && !path.walk)continue;
			if (!sharebike && !path.sharebike) continue;
			if (!bus && path.bus)continue;
			if (dist[path.to] > dist[u] + path.length) {
				dist[path.to] = dist[u] + path.length;
				pre[path.to] = path;
				q.push({ dist[path.to],path.to });
			}
		}
	}
	std::vector<SchoolPath> res;
	int u = to;
	while (pre[u].from != 0) {
		res.push_back(pre[u]);
		u = pre[u].from;
	}
	std::reverse(res.begin(), res.end());
	result["count"] = res.size();
	for (const auto& item : res) {
		result["results"].push_back(item.to_json());
	}
	return result;
}