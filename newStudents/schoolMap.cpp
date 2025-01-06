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
	//std::ifstream busStream("bus.json");
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
	attractions.clear();
	destinations.clear();
	paths.clear();
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
	loadFromFile();
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
	//std::cout << "addAttraction" << std::endl;

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

// 查找距离最短的路径
nlohmann::json SchoolMap::getShortestPath(int from, int to, bool walk, bool sharebike, bool bus) {
	nlohmann::json result;
	result["count"] = -1;

	//std::cout << "from" << std::endl;
	//std::cout << attractions.count(from)<<std::endl;
	//std::cout << "to" << std::endl;
	//std::cout << attractions.count(to) << std::endl;

	//std::cout << from << ' ' << to << ' ' << walk << ' ' << sharebike << bus << std::endl;

	if (!attractions.count(from) || !attractions.count(to)) {
		return result;
	}

	result["results"] = nlohmann::json::array();


	std::vector<double> time;
	std::vector<double> dist;
	std::vector<SchoolPath> pre;
	std::vector<bool> vis;
	std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<std::pair<double, int>>> q;
	time.resize(current_attraction_id + 1, 1e18);
	pre.resize(current_attraction_id + 1);
	vis.resize(current_attraction_id + 1, false);
	dist.resize(current_attraction_id + 1,1e18);
	time[from] = 0;
	dist[from] = 0;

	q.push({ 0,from });
	while (!q.empty()) {
		//std::cout << q.top().first << ' ' << q.top().second << std::endl;
		//std::cout << q.top().first << ' ' << q.top().second << std::endl;
		auto [d, u] = q.top();
		q.pop();
		if (vis[u]) {
			continue;
		}
		vis[u] = true;
		for (const auto path : destinations[u]) {
			//std::cout << "sssss1 :   " << path.id << std::endl;
			if (!((walk && path.walk) || (bus && path.bus) || (sharebike && path.sharebike))) {
				continue;
			}
			//std::cout <<"sssss2  :   "<< path.id << std::endl;
			double exceptTime = 1e18;
			if (walk && path.walk)
				exceptTime = std::min(exceptTime, path.length / 60);

			if (sharebike && path.sharebike)
				exceptTime = std::min(exceptTime, path.length / 200);

			if (bus && path.bus)
				exceptTime = std::min(exceptTime, path.length / 500);
			//std::cout << path.id << std::endl;
			
			if (dist[path.to] > dist[u] + path.length) {
				dist[path.to] = dist[u] + path.length;
				pre[path.to] = path;	
				time[path.to] = time[path.from] + exceptTime;
				q.push({ dist[path.to],path.to });
				//std::cout << q.top().second << std::endl;
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
	result["distance"] = dist[to];
	result["time"] = time[to];
	for (const auto& item : res) {
		result["results"].push_back(item.to_json());
	}
	return result;
}

//查找最快速的路径 walk速度为60 sharebike速度为200 bus速度为500
nlohmann::json SchoolMap::getFastestPath(int from, int to, bool walk, bool sharebike, bool bus) {
	nlohmann::json result;
	result["count"] = -1;
	
	if (!attractions.count(from) || !attractions.count(to)) {
		return result;
	}

	result["results"] = nlohmann::json::array();

	std::vector<double> time;
	std::vector<double> dist;
	std::vector<SchoolPath> pre;
	std::vector<bool> vis;
	std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<std::pair<double, int>>> q;
	time.resize(current_attraction_id+1, 1e18);
	pre.resize(current_attraction_id+1);
	vis.resize(current_attraction_id+1, false);
	dist.resize(current_attraction_id+1,1e18);
	time[from] = 0;
	dist[from] = 0;
	q.push({ 0,from });
	while (!q.empty()) {
		auto [d, u] = q.top();
		q.pop();
		if (vis[u]) {
			continue;
		}
		vis[u] = true;
		for (const auto path : destinations[u]) {
			if (!((walk && path.walk) || (bus && path.bus) || (sharebike && path.sharebike))) {
				continue;
			}
			//std::cout <<"sssss :   "<< path.id << std::endl;
			double minTime = 1e18;

			if (walk && path.walk)
				minTime = std::min(minTime, path.length / 60);

			if (sharebike && path.sharebike)
				minTime = std::min(minTime, path.length / 200);

			if (bus && path.bus)
				minTime = std::min(minTime, path.length / 500);
			
			//std::cout << path.id << std::endl;
			if (time[path.to] > time[u] + minTime) {
				dist[path.to] = dist[path.from]+path.length;
				time[path.to] = time[u] + minTime;
				pre[path.to] = path;
				q.push({ time[path.to],path.to });
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
	result["time"] = time[to];
	result["distance"] = dist[to];
	for (const auto& item : res) {
		result["results"].push_back(item.to_json());
	}
	return result;
}

//获取所有的路径
nlohmann::json SchoolMap::getAllPath(int from, int to, bool walk, bool sharebike, bool bus) {
	nlohmann::json result;
	result["count"] = -1;
	if (!attractions.count(from) || !attractions.count(to)) {
		return result;
	}
	result["results"] = nlohmann::json::array();
	//nlohmann::json tmp;
	std::vector<SchoolPath> tmp;
	std::vector<int> vis(current_attraction_id);
	result["count"]=0;
	std::function<void(int,double,double)> dfs = [&](int u,double distance,double time) {
		if (u == to) {
			result["count"] = result["count"]+1;
			nlohmann::json nPath;
			nPath["result"] = nlohmann::json::array();
			nPath["count"] = 0;
			
			for (auto path : tmp) {
				nPath["count"] = nPath["count"] + 1;
				nPath["result"].push_back(path.to_json());
			};
			nPath["distance"] = distance;
			nPath["time"] = time;

			result["results"].push_back(nPath);
			return;
		}
		for (auto const path : destinations[u]) {
			if (!(
				(walk && path.walk) || 
				(bus && path.bus) || 
				(sharebike && path.sharebike))) {
				continue;
			}
			if (vis[path.to])continue;
			double minTime = 1e18;

			if (walk && path.walk)
				minTime = std::min(minTime, path.length / 60);

			if (sharebike && path.sharebike)
				minTime = std::min(minTime, path.length / 200);

			if (bus && path.bus)
				minTime = std::min(minTime, path.length / 500);

			vis[path.to] = 1;
			tmp.push_back(path);
			dfs(path.to,distance+path.length,minTime);
			vis[path.to] = 0;
			tmp.pop_back();
		}
		return;
		};
	vis[from] = 1;
	dfs(from,0,0);
	return result;
}